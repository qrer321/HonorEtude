// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>

#include "GameInfo.h"
#include "SocketSubsystem.h"
#include "Engine/GameInstance.h"
#include "HonorProject/Message/Messages.h"
#include "HonorProject/Message/ContentsStructure.h"
#include "HonorProject/UI/Play/ChatWindow.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "HonorProjectGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FCharacterTableInfo
	: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SPRecoverMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;
};

class ClientRecvThread_TCP : public FRunnable
{
public:
	ClientRecvThread_TCP(FSocket* RecvSocket, ISocketSubsystem* SocketSubsystem, TQueue<std::shared_ptr<GameServerMessage>>* MessageQueue);

private:
	FSocket*									m_RecvSocket;
	ISocketSubsystem*							m_SocketSubsystem;
	TAtomic<bool>								m_IsThreadRunnable;
	TQueue<std::shared_ptr<GameServerMessage>>* m_MessageQueue;

public:
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	void Close();
};

class ClientRecvThread_UDP : public FRunnable
{
public:
	ClientRecvThread_UDP(FSocket* RecvSocket, ISocketSubsystem* SocketSubsystem, TQueue<std::shared_ptr<GameServerMessage>>* MessageQueue);

private:
	FSocket*									m_RecvSocket;
	ISocketSubsystem*							m_SocketSubsystem;
	TAtomic<bool>								m_IsThreadRunnable;
	TQueue<std::shared_ptr<GameServerMessage>>* m_MessageQueue;

public:
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	void Close();
};

UCLASS()
class HONORPROJECT_API UHonorProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHonorProjectGameInstance();
	virtual ~UHonorProjectGameInstance() override;
	
private:
	// Server Member Variable
	ClientRecvThread_TCP*	m_TCPRecvThread;
	ClientRecvThread_UDP*	m_UDPRecvThread;
	FRunnableThread*		m_TCPRunnableThread;
	FRunnableThread*		m_UDPRunnableThread;

	ISocketSubsystem*		m_SocketSubsystem;
	FSocket*				m_TCPClientSocket;
	FSocket*				m_UDPClientSocket;

	FIPv4Address			m_ConnectAddress;
	FIPv4Endpoint 			m_TCPEndPoint;
	FIPv4Endpoint 			m_UDPEndPoint;
	int 					m_TCPPort;
	int 					m_UDPPort;

	TQueue<std::shared_ptr<GameServerMessage>> m_MessageQueue;

	bool m_ClientMode;

public:
	// Server Member Variable
	FString						m_UserID;
	FString						m_Nickname;
	TAtomic<bool>				m_LoginProcess;
	std::vector<FCharacterInfo>	m_CharactersInfo;
	FCharacterInfo				m_SelectCharacterInfo;
	uint64_t 					m_ActorIndex;
	uint64_t 					m_ThreadIndex;
	uint64_t 					m_SectionIndex;
	
	
	UPROPERTY()
	UListView*					m_CharacterListView;
	UPROPERTY()
	UChatWindow*				m_ChatWindow;

private:
	UPROPERTY()
	UDataTable*					m_CharacterInfoTable;

public:
	virtual void Init() override;
	virtual void BeginDestroy() override;
	
	const FCharacterTableInfo* FindCharacterInfo(const FString& Name) const;

	void PushClientMessage(std::shared_ptr<GameServerMessage> Message);
	std::shared_ptr<GameServerMessage> PopClientMessage();
	bool IsEmptyMessage() const { return m_MessageQueue.IsEmpty(); }
	
	bool ClientThreadCheck();
	bool ServerConnect_TCP(const FString& IPString, const FString& PortString);
	bool ServerConnect_UDP(const FString& PortString);
	void CloseConnect();

	bool Send(const std::vector<uint8>& Data);
	bool SendTo(const std::vector<uint8>& Data);

	FORCEINLINE bool GetClientMode() const { return m_ClientMode; }
	FORCEINLINE void SetClientMode(bool Mode) { m_ClientMode = Mode; }
};
