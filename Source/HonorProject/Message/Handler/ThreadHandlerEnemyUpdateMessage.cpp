#include "ThreadHandlerEnemyUpdateMessage.h"

#include "HonorProject/GameMode/PlayGameMode.h"

void ThreadHandlerEnemyUpdateMessage::Start()
{
	if (nullptr == m_World)
	{
		return;
	}

	if (nullptr == m_GameInstance)
	{
		return;
	}

	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(m_World));
	if (nullptr == PlayGameMode || false == PlayGameMode->IsValidLowLevel())
	{
		LOGSTRING(TEXT("GameMode Is Not Valid"));
		return;
	}

	TSubclassOf<AMasterAICharacter> AICharacter = PlayGameMode->GetEnemyAIClass(m_Message->m_EnemyType);
	if (nullptr == AICharacter || false == AICharacter->IsValidLowLevel())
	{
		LOGSTRING(TEXT("AICharacter Is Not Valid"));
		return;
	}

	// 해당 캐릭터가 존재하지 않는다면
	if (false == PlayGameMode->IsRegistered(m_Message->m_ActorIndex))
	{
		FTransform CharacterTransform = FTransform(m_Message->m_Pos);

		AMasterAICharacter* NewEnemyCharacter = m_World->SpawnActorDeferred<AMasterAICharacter>(AICharacter.Get(), CharacterTransform);
		NewEnemyCharacter->SetObjectType(EGameObjectType::Enemy);
		NewEnemyCharacter->SetActorIndex(m_Message->m_ActorIndex);
		NewEnemyCharacter->FinishSpawning(CharacterTransform);

		PlayGameMode->RegistObject(m_Message->m_ActorIndex, EGameObjectType::Enemy, NewEnemyCharacter);
		PlayGameMode->PushObjectMessage(m_Message->m_ActorIndex, m_Message);
		return;
	}

	PlayGameMode->PushObjectMessage(m_Message->m_ActorIndex, m_Message);
}
