// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameMode.h"
#include "HonorProject/Character/CharacterController.h"

APlayGameMode::APlayGameMode()
	: m_ClientUniqueID(0)
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/HonorProejct/PlayRelevant/Character/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawnBPClass.Succeeded())
		DefaultPawnClass = PlayerPawnBPClass.Class;
	
	PlayerControllerClass = ACharacterController::StaticClass();

	for (size_t i = 0; i < static_cast<size_t>(EGameObjectType::MAX); ++i)
	{
		m_TypeOfAllObject.Add(static_cast<EGameObjectType>(i), TArray<AActor*>());
	}
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

AActor* APlayGameMode::GetGameObject(int ObjectID)
{
	if (false == m_AllObject.Contains(ObjectID))
	{
		return nullptr;
	}

	return m_AllObject.Find(ObjectID)->Actor;
}

TArray<AActor*> APlayGameMode::GetObjectGroup(EGameObjectType ObjectType)
{
	return m_TypeOfAllObject[ObjectType];
}

TArray<AActor*>& APlayGameMode::GetObjectGroupRef(EGameObjectType ObjectType)
{
	return m_TypeOfAllObject[ObjectType];
}

bool APlayGameMode::RegistObject(int ObjectID, EGameObjectType ObjectType, AActor* Actor)
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
	m_TypeOfAllObject[ObjectType].Add(Actor);
	return true;
}

bool APlayGameMode::UnregistObject(int ObjectID, EGameObjectType ObjectType, AActor* Actor)
{
	if (nullptr == Actor)
	{
		return false;
	}

	if (false == IsRegistered(ObjectID))
	{
		return false;
	}

	if (m_AllObject[ObjectID].Actor != Actor)
	{
		LOGSTRING(TEXT("메세지와 액터가 서로 다릅니다"))
	}

	m_AllObject.Remove(ObjectID);
	m_TypeOfAllObject[ObjectType].Remove(Actor);
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
