// Fill out your copyright notice in the Description page of Project Settings.


#include "HonorProjectGameInstance.h"
#include "Sockets.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../Message/MessageConverter.h"
#include "../Message/Handler/ThreadHandlerServerDestroyMessage.h"


////////////////////////////////////// TCP Receive Thread //////////////////////////////////////
ClientRecvThread_TCP::ClientRecvThread_TCP(FSocket* RecvSocket, ISocketSubsystem* SocketSubsystem, TQueue<std::shared_ptr<GameServerMessage>>* MessageQueue)
{
	if (nullptr == RecvSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("RecvSocket Is Nullptr"));
	}

	if (nullptr == SocketSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("SocketSubsystem Is Nullptr"));
	}

	if (nullptr == MessageQueue)
	{
		UE_LOG(LogTemp, Error, TEXT("MessageQueue Is Nullptr"));
	}

	m_RecvSocket = RecvSocket;
	m_SocketSubsystem = SocketSubsystem;
	m_MessageQueue = MessageQueue;
	m_IsThreadRunnable = true;
}

uint32 ClientRecvThread_TCP::Run()
{
	UE_LOG(LogTemp, Log, TEXT("Receive Thread Start"));

	while (m_IsThreadRunnable)
	{
		std::vector<uint8> RecvData;
		RecvData.resize(1024);
		
		int32 RecvDataSize = 0;
		if (false == m_RecvSocket->Recv(&RecvData[0], RecvData.size(), RecvDataSize))
		{
			auto ConnectionState = m_RecvSocket->GetConnectionState();

			switch (ConnectionState)
			{
			case SCS_NotConnected:
				break;
			case SCS_Connected:
				{
					// 클라이언트는 켜져있는데 서버가 종료되었을 때.
					// RecvSocket은 아직 Connected 상태라고 보고 해당 타입을 반환한다.
					// ServerDestroyMessage 클래스는 아무것도 구현되어있지 않은 빈 ServerMessage 클래스이다.
					// 이후 서버가 직접 Disconnect 할때에도 동일한 메시지를 보낼 것이다.
					ServerDestroyMessage DestroyMessage;
					GameServerSerializer Serializer;
					DestroyMessage.Serialize(Serializer);

					MessageConverter Converter = MessageConverter(Serializer.GetData());
					m_MessageQueue->Enqueue(Converter.GetServerMessage());
					return 0;
				}
				break;
			case SCS_ConnectionError:
				// 직접 클라이언트를 종료했을 때
				break;
			default:
				break;
			}

			break;
		}

		MessageConverter Converter = MessageConverter(RecvData);
		m_MessageQueue->Enqueue(MoveTemp(Converter.GetServerMessage()));
	}
	
	return 0;
}

void ClientRecvThread_TCP::Stop()
{
	FRunnable::Stop();
}

void ClientRecvThread_TCP::Exit()
{
	FRunnable::Exit();
}

void ClientRecvThread_TCP::Close()
{
	m_IsThreadRunnable = false;
}

////////////////////////////////////// UDP Receive Thread //////////////////////////////////////
ClientRecvThread_UDP::ClientRecvThread_UDP(FSocket* RecvSocket, ISocketSubsystem* SocketSubsystem, TQueue<std::shared_ptr<GameServerMessage>>* MessageQueue)
{
	if (nullptr == RecvSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("RecvSocket Is Nullptr"));
	}

	if (nullptr == SocketSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("SocketSubsystem Is Nullptr"));
	}

	if (nullptr == MessageQueue)
	{
		UE_LOG(LogTemp, Error, TEXT("MessageQueue Is Nullptr"));
	}

	m_RecvSocket = RecvSocket;
	m_SocketSubsystem = SocketSubsystem;
	m_MessageQueue = MessageQueue;
	m_IsThreadRunnable = true;
}

uint32 ClientRecvThread_UDP::Run()
{
	UE_LOG(LogTemp, Log, TEXT("Receive Thread Start"));
	
	const FIPv4Endpoint Endpoint = FIPv4Endpoint();
	TSharedRef<FInternetAddr> InternetAddr = Endpoint.ToInternetAddr(); 

	while (m_IsThreadRunnable)
	{
		std::vector<uint8> RecvData;
		RecvData.resize(1024);
		int32 RecvDataSize = 0;
		
		if (false == m_RecvSocket->RecvFrom(&RecvData[0], RecvData.size(), RecvDataSize, InternetAddr.Get()))
		{
			const FString SocketError = m_SocketSubsystem->GetSocketError(m_SocketSubsystem->GetLastErrorCode());
			UE_LOG(LogTemp, Error, TEXT("%s"), *SocketError);
			
			break;
		}

		MessageConverter Converter = MessageConverter(RecvData);
		m_MessageQueue->Enqueue(MoveTemp(Converter.GetServerMessage()));
	}
	
	return 0;
}

void ClientRecvThread_UDP::Stop()
{
	FRunnable::Stop();
}

void ClientRecvThread_UDP::Exit()
{
	FRunnable::Exit();
}

void ClientRecvThread_UDP::Close()
{
	m_IsThreadRunnable = false;
}

UHonorProjectGameInstance::UHonorProjectGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterTableAsset(TEXT("DataTable'/Game/HonorProejct/PlayRelevant/Character/Data/DTCharacterInfo.DTCharacterInfo'"));
	if (CharacterTableAsset.Succeeded())
		m_CharacterInfoTable = CharacterTableAsset.Object;

	m_ClientMode = false;
	m_LoginProcess = false;
}

UHonorProjectGameInstance::~UHonorProjectGameInstance()
{
}

void UHonorProjectGameInstance::Init()
{
	Super::Init();
}

void UHonorProjectGameInstance::BeginDestroy()
{
	Super::BeginDestroy();

	CloseConnect();
}

const FCharacterTableInfo* UHonorProjectGameInstance::FindCharacterInfo(const FString& Name) const
{
	return m_CharacterInfoTable->FindRow<FCharacterTableInfo>(*Name, "");
}

std::shared_ptr<GameServerMessage> UHonorProjectGameInstance::PopClientMessage()
{
	std::shared_ptr<GameServerMessage> Message;
	m_MessageQueue.Dequeue(Message);

	return Message;
}

void UHonorProjectGameInstance::PushClientMessage(std::shared_ptr<GameServerMessage> Message)
{
	m_MessageQueue.Enqueue(Message);
}

bool UHonorProjectGameInstance::ClientThreadCheck()
{
	if (false == FPlatformProcess::SupportsMultithreading())
	{
		UE_LOG(LogTemp, Error, TEXT("Is Not Support MultiThreading"));
		return false;
	}

	return true;
}

bool UHonorProjectGameInstance::ServerConnect_TCP(const FString& IPString, const FString& PortString)
{
	if (false == ClientThreadCheck())
	{
		UE_LOG(LogTemp, Error, TEXT("Client Did Not Support MultiThreading"));
		return false;
	}

	CloseConnect();

	m_SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (nullptr == m_SocketSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Get SocketSubsystem Is Failed"));
		return false;
	}

	m_TCPClientSocket = m_SocketSubsystem->CreateSocket(NAME_Stream, "ClientSocket_TCP");
	if (nullptr == m_TCPClientSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("Create TCP Client Socket Is Failed"));
		return false;
	}

	m_TCPClientSocket->SetNoDelay(true);

	FIPv4Address::Parse(IPString, m_ConnectAddress);
	m_TCPPort = static_cast<uint16>(FCString::Atoi(*PortString));
	m_TCPEndPoint = FIPv4Endpoint(m_ConnectAddress, m_TCPPort);

	if (false == m_TCPClientSocket->Connect(m_TCPEndPoint.ToInternetAddr().Get()))
	{
		const FString SocketError = m_SocketSubsystem->GetSocketError(m_SocketSubsystem->GetLastErrorCode());
		UE_LOG(LogTemp, Error, TEXT("%s"), *SocketError);

		m_TCPClientSocket->Close();
		m_TCPClientSocket = nullptr;
		return false;
	}

	m_TCPRecvThread = new ClientRecvThread_TCP(m_TCPClientSocket, m_SocketSubsystem, &m_MessageQueue);
	m_TCPRunnableThread = FRunnableThread::Create(m_TCPRecvThread, TEXT("TCP Recv Thread"));
	
	return true;
}

bool UHonorProjectGameInstance::ServerConnect_UDP(const FString& PortString)
{
	if (nullptr == m_SocketSubsystem)
	{
		return false;
	}

	m_UDPClientSocket = m_SocketSubsystem->CreateSocket(NAME_DGram, "ClientSocket_UDP");
	if (nullptr == m_UDPClientSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("Create UDP Client Socket Is Failed"));
		return false;
	}
	
	m_UDPPort = static_cast<uint16>(FCString::Atoi(*PortString));
	m_UDPEndPoint = FIPv4Endpoint(m_ConnectAddress, m_UDPPort);

	m_UDPClientSocket->Bind(m_UDPEndPoint.ToInternetAddr().Get());
	m_UDPRecvThread = new ClientRecvThread_UDP(m_UDPClientSocket, m_SocketSubsystem, &m_MessageQueue);
	m_UDPRunnableThread = FRunnableThread::Create(m_UDPRecvThread, TEXT("UDP Recv Thread"));

	return true;
}

bool UHonorProjectGameInstance::Send(const std::vector<uint8>& Data)
{
	if (0 == Data.size())
	{
		return false;
	}

	if (nullptr == m_TCPClientSocket)
	{
		return false;
	}

	int32 DataSendSize = 0;
	return m_TCPClientSocket->Send(&Data[0], Data.size(), DataSendSize);
}

bool UHonorProjectGameInstance::SendTo(const std::vector<uint8>& Data)
{
	if (0 == Data.size())
	{
		return false;
	}

	if (nullptr == m_TCPClientSocket)
	{
		return false;
	}

	int32 DataSendSize = 0;
	return m_UDPClientSocket->SendTo(&Data[0], Data.size(), DataSendSize, m_UDPEndPoint.ToInternetAddr().Get());
}

void UHonorProjectGameInstance::CloseConnect()
{
	if (nullptr != m_TCPRecvThread)
	{
		m_TCPRecvThread->Close();
	}
	
	if (nullptr == m_TCPClientSocket)
	{
		return;
	}

	m_LoginProcess = false;
	
	m_TCPClientSocket->Close();
	m_TCPClientSocket = nullptr;

	if (nullptr == m_UDPClientSocket)
	{
		return;
	}
	m_UDPClientSocket->Close();
	m_UDPClientSocket = nullptr;
}