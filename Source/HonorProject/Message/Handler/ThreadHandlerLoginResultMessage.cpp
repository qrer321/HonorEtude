#include "ThreadHandlerLoginResultMessage.h"

void ThreadHandlerLoginResultMessage::Start()
{
	if (nullptr == m_World)
	{
		return;
	}

	if (nullptr == m_GameInstance)
	{
		return;
	}
	
	if (EGameServerCode::OK == m_Message->m_Code)
	{
		UGameplayStatics::OpenLevel(m_World, TEXT("PlayTestLevel"));
		//PrintViewport(10.f, FColor::Red, TEXT("Login Succeed"));
		return;
	}
}
