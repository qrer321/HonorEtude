// Fill out your copyright notice in the Description page of Project Settings.


#include "HonorProjectGameInstance.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "../Message/MessageConverter.h"
#include "../Message/Handler/ThreadHandlerServerDestroyMessage.h"

ClientRecvThread::ClientRecvThread(FSocket* RecvSocket, TQueue<std::shared_ptr<GameServerMessage>>* MessageQueue)
{
	if (nullptr == RecvSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("RecvSocket Error"));
	}

	if (nullptr == MessageQueue)
	{
		UE_LOG(LogTemp, Error, TEXT("MessageQueue Error"));
	}

	m_RecvSocket = RecvSocket;
	m_MessageQueue = MessageQueue;
	m_IsThreadRunnable = true;
}

uint32 ClientRecvThread::Run()
{
	UE_LOG(LogTemp, Log, TEXT("Recv Start"));

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

void ClientRecvThread::Stop()
{
	FRunnable::Stop();
}

void ClientRecvThread::Exit()
{
	FRunnable::Exit();
}

void ClientRecvThread::Close()
{
	m_IsThreadRunnable = false;
}

UHonorProjectGameInstance::UHonorProjectGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterTableAsset(TEXT("DataTable'/Game/HonorProejct/PlayRelevant/Character/Data/DTCharacterInfo.DTCharacterInfo'"));
	if (CharacterTableAsset.Succeeded())
		m_CharacterInfoTable = CharacterTableAsset.Object;
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

bool UHonorProjectGameInstance::ServerConnect(const FString& IPString, const FString& PortString)
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

	m_ClientSocket = m_SocketSubsystem->CreateSocket(NAME_Stream, "ClientSocket");
	if (nullptr == m_ClientSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("Create Client Socket Is Failed"));
		return false;
	}

	m_ClientSocket->SetNoDelay(true);

	FIPv4Address ConnectAddress;
	FIPv4Address::Parse(IPString, ConnectAddress);
	FIPv4Endpoint EndPoint = FIPv4Endpoint(ConnectAddress, static_cast<uint16>(FCString::Atoi(*PortString)));

	if (false == m_ClientSocket->Connect(EndPoint.ToInternetAddr().Get()))
	{
		const FString SocketError = m_SocketSubsystem->GetSocketError(m_SocketSubsystem->GetLastErrorCode());
		UE_LOG(LogTemp, Error, TEXT("%s"), *SocketError);

		m_ClientSocket->Close();
		m_ClientSocket = nullptr;
		return false;
	}

	m_RecvThread = new ClientRecvThread(m_ClientSocket, &m_MessageQueue);
	m_RunnableThread = FRunnableThread::Create(m_RecvThread, TEXT("Recv Thread"));
	
	return true;
}

void UHonorProjectGameInstance::CloseConnect()
{
	if (nullptr != m_RecvThread)
	{
		m_RecvThread->Close();
	}
	
	if (nullptr == m_ClientSocket)
	{
		return;
	}

	m_ClientSocket->Close();
	m_ClientSocket = nullptr;
}

bool UHonorProjectGameInstance::Send(const std::vector<uint8>& Data)
{
	if (0 == Data.size())
	{
		return false;
	}

	if (nullptr == m_ClientSocket)
	{
		return false;
	}

	int32 DataSendSize = 0;
	return m_ClientSocket->Send(&Data[0], Data.size(), DataSendSize);
}
