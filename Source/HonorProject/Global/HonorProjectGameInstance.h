// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>

#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "../Message/Messages.h"
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

class ClientRecvThread : public FRunnable
{
public:
	ClientRecvThread(FSocket* RecvSocket, TQueue<TSharedPtr<GameServerMessage>>* MessageQueue);

private:
	FSocket* m_RecvSocket;
	TQueue<TSharedPtr<GameServerMessage>>* m_MessageQueue;

public:
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
};

UCLASS()
class HONORPROJECT_API UHonorProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHonorProjectGameInstance();
	virtual ~UHonorProjectGameInstance() override;

private:
	ClientRecvThread*	m_RecvThread;
	FRunnableThread*	m_RunnableThread;

	ISocketSubsystem*	m_SocketSubsystem;
	FSocket*			m_ClientSocket;

	TQueue<TSharedPtr<GameServerMessage>> m_MessageQueue;

private:
	UPROPERTY()
	UDataTable*			m_CharacterInfoTable;

public:
	virtual void Init() override;
	virtual void BeginDestroy() override;
	const FCharacterTableInfo* FindCharacterInfo(const FString& Name) const;

public:
	TQueue<TSharedPtr<GameServerMessage>>& GetMessageQueue() { return m_MessageQueue; }
	
public:
	bool ClientThreadCheck();
	bool ServerConnect(const FString& IPString, const FString& PortString);
	void CloseConnect();

	bool Send(const TArray<uint8>& Data);
};