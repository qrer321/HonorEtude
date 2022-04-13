#include "ThreadHandlerPlayerDestroyMessage.h"

void ThreadHandlerPlayerDestroyMessage::Start()
{
	m_GameInstance->CloseConnect();

	if (m_World->GetName() != TEXT("LoginLevel"))
	{
		UGameplayStatics::OpenLevel(m_World, TEXT("LoginLevel"));
		return;
	}
}
