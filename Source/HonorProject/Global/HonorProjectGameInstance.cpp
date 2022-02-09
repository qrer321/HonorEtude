// Fill out your copyright notice in the Description page of Project Settings.


#include "HonorProjectGameInstance.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "../Message/MessageConverter.h"

ClientRecvThread::ClientRecvThread(FSocket* RecvSocket, TQueue<TSharedPtr<GameServerMessage>>* MessageQueue)
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
}

uint32 ClientRecvThread::Run()
{
	UE_LOG(LogTemp, Log, TEXT("Recv Start"));

	while (true)
	{
		TArray<uint8> RecvData;
		RecvData.SetNum(1024);
		
		int32 RecvDataSize = 0;
		if (false == m_RecvSocket->Recv(RecvData.GetData(), RecvData.Num(), RecvDataSize))
		{
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
	if (nullptr == m_ClientSocket)
		return;

	m_ClientSocket->Close();
	m_ClientSocket = nullptr;
}

bool UHonorProjectGameInstance::Send(const TArray<uint8>& Data)
{
	if (0 == Data.Num())
	{
		return false;
	}

	if (nullptr == m_ClientSocket)
	{
		return false;
	}

	int32 DataSendSize = 0;
	return m_ClientSocket->Send(Data.GetData(), Data.Num(), DataSendSize);
}