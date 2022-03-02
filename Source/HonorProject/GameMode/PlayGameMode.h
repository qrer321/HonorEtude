// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "GameFramework/GameMode.h"
#include "HonorProject/Component/ObjectMessageComponent.h"
#include "HonorProject/Character/MasterAICharacter.h"
#include "PlayGameMode.generated.h"

struct ServerObject
{
	AActor*						Actor;
	UObjectMessageComponent*	Message;
};

UCLASS()
class HONORPROJECT_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APlayGameMode();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientData", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AMasterAICharacter>> m_AICharacterClasses;

	TMap<int, ServerObject>					m_AllObject;
	TMap<EGameObjectType, TArray<AActor*>>	m_TypeOfAllObject;

	int m_ClientUniqueID;

public:
	TSubclassOf<AMasterAICharacter> GetEnemyAIClasses(int EnemyType) const;
	AActor* GetGameObject(int ObjectID);
	int GetClientUniqueID() { return m_ClientUniqueID++; }

	TArray<AActor*> GetObjectGroup(EGameObjectType ObjectType);
	TArray<AActor*>& GetObjectGroupRef(EGameObjectType ObjectType);

	bool RegistObject(int ObjectID, EGameObjectType ObjectType, AActor* Actor);
	bool UnregistObject(int ObjectID, EGameObjectType ObjectType, AActor* Actor);
	bool IsRegistered(int ObjectID) const;

	bool PushObjectMessage(int ObjectID, std::shared_ptr<GameServerMessage> Message);
};
