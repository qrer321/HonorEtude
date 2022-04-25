#include "ThreadHandlerUDPReadyOKMessage.h"
#include "HonorProject/GameMode/PlayGameMode.h"

void ThreadHandlerUDPReadyOKMessage::Start()
{
	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(m_World));
	if (nullptr == PlayGameMode || false == PlayGameMode->IsValidLowLevel())
	{
		LOGSTRING(TEXT("GameMode Is Not Valid"));
		return;
	}
	
	PlayGameMode->PushObjectMessage(m_Message->m_ActorIndex, m_Message);
}
