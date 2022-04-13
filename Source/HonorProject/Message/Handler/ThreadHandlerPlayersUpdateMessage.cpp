#include "ThreadHandlerPlayersUpdateMessage.h"
#include "HonorProject/GameMode/PlayGameMode.h"

void ThreadHandlerPlayersUpdateMessage::Start()
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

	for (const FPlayerUpdateData& Datum : m_Message->m_Data)
	{
		std::shared_ptr<PlayerUpdateMessage> UpdateMessage = std::make_shared<PlayerUpdateMessage>();
		UpdateMessage->m_Datum = Datum;

		// 해당 캐릭터가 존재하지 않는다면
		if (false == PlayGameMode->IsRegistered(UpdateMessage->m_Datum.m_ActorIndex))
		{
			FTransform CharacterTransform = FTransform(UpdateMessage->m_Datum.m_Pos);

			APlayerCharacter* NewPlayerCharacter = m_World->SpawnActorDeferred<APlayerCharacter>(Character.Get(), CharacterTransform);
			NewPlayerCharacter->SetObjectType(EGameObjectType::Player);
			NewPlayerCharacter->SetActorIndex(UpdateMessage->m_Datum.m_ActorIndex);
			NewPlayerCharacter->FinishSpawning(CharacterTransform);

			PlayGameMode->RegistObject(UpdateMessage->m_Datum.m_ActorIndex, EGameObjectType::Player, NewPlayerCharacter);
			PlayGameMode->PushObjectMessage(UpdateMessage->m_Datum.m_ActorIndex, m_Message);
			return;
		}

		PlayGameMode->PushObjectMessage(UpdateMessage->m_Datum.m_ActorIndex, m_Message);
	}
}
