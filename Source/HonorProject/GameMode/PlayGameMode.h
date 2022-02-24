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

	TMap<int, ServerObject> m_AllObject;

public:
	TSubclassOf<AMasterAICharacter> GetEnemyAIClasses(int EnemyType) const;

	bool RegisterObject(int ObjectID, AActor* Actor);
	bool IsRegistered(int ObjectID) const;

	bool PushObjectMessage(int ObjectID, std::shared_ptr<GameServerMessage> Message);
};
