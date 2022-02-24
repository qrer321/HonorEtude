// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameMode.h"
#include "HonorProject/Character/CharacterController.h"

APlayGameMode::APlayGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/HonorProejct/PlayRelevant/Character/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawnBPClass.Succeeded())
		DefaultPawnClass = PlayerPawnBPClass.Class;
	
	PlayerControllerClass = ACharacterController::StaticClass();
}

TSubclassOf<AMasterAICharacter> APlayGameMode::GetEnemyAIClasses(int EnemyType) const
{
	if (0 > EnemyType)
	{
		return nullptr;
	}

	if (EnemyType >= m_AICharacterClasses.Num())
	{
		return nullptr;
	}

	return m_AICharacterClasses[EnemyType];
}

bool APlayGameMode::RegisterObject(int ObjectID, AActor* Actor)
{
	if (nullptr == Actor)
	{
		return false;
	}

	if (true == IsRegistered(ObjectID))
	{
		return false;
	}

	UObjectMessageComponent* MessageComponent = Actor->FindComponentByClass<UObjectMessageComponent>();
	if (nullptr == MessageComponent || false == MessageComponent->IsValidLowLevel())
	{
		LOGSTRING(TEXT("AnimInstance Is Not Valid"));
		return false;
	}

	m_AllObject.Add(ObjectID, { Actor, MessageComponent });
	return true;
}

bool APlayGameMode::IsRegistered(int ObjectID) const
{
	return m_AllObject.Contains(ObjectID);
}

bool APlayGameMode::PushObjectMessage(int ObjectID, std::shared_ptr<GameServerMessage> Message)
{
	if (nullptr == Message)
	{
		return false;
	}

	if (false == IsRegistered(ObjectID))
	{
		return false;
	}

	m_AllObject[ObjectID].Message->EnQueue(Message);
	return true;
}
