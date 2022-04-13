#include "ThreadHandlerPlayerUpdateMessage.h"
#include "HonorProject/GameMode/PlayGameMode.h"

void ThreadHandlerPlayerUpdateMessage::Start()
{
	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(m_World));
	if (nullptr == PlayGameMode || false == PlayGameMode->IsValidLowLevel())
	{
		LOGSTRING(TEXT("GameMode Is Not Valid"));
		return;
	}

	TSubclassOf<APlayerCharacter> Character = PlayGameMode->GetPlayerClass();
	if (nullptr == Character || false == Character->IsValidLowLevel())
	{
		LOGSTRING(TEXT("Player Character Is Not Valid"));
		return;
	}

	// 해당 캐릭터가 존재하지 않는다면
	if (false == PlayGameMode->IsRegistered(m_Message->m_Datum.m_ActorIndex))
	{
		FTransform CharacterTransform = FTransform(m_Message->m_Datum.m_Pos);

		APlayerCharacter* NewPlayerCharacter = m_World->SpawnActorDeferred<APlayerCharacter>(Character.Get(), CharacterTransform);
		NewPlayerCharacter->SetObjectType(EGameObjectType::Player);
		NewPlayerCharacter->SetActorIndex(m_Message->m_Datum.m_ActorIndex);
		NewPlayerCharacter->FinishSpawning(CharacterTransform);

		PlayGameMode->RegistObject(m_Message->m_Datum.m_ActorIndex, EGameObjectType::Player, NewPlayerCharacter);
	}

	PlayGameMode->PushObjectMessage(m_Message->m_Datum.m_ActorIndex, m_Message);
}
