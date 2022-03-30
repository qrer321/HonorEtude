#include "ThreadHandlerObjectDestroyMessage.h"
#include "HonorProject/GameMode/PlayGameMode.h"

void ThreadHandlerObjectDestroyMessage::Start()
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
		LOGSTRING(TEXT("PlayGameMode Is Not Valid"));
		return;
	}

	PlayGameMode->PushObjectMessage(m_Message->m_ObjectID, m_Message);
}
