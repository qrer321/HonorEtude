#include "ThreadHandlerMoveLevelMessage.h"

#include "HonorProject/GameMode/PlayGameMode.h"

void ThreadHandlerMoveLevelMessage::Start()
{
	APlayGameMode* PGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(m_World));

	if (nullptr == PGameMode
		|| false == PGameMode->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Is Not Valid"));
		return;
	}

	PGameMode->PushObjectMessage(m_Message->m_ActorIndex, m_Message);
}
