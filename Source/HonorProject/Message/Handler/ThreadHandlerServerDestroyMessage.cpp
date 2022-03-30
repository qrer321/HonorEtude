#include "ThreadHandlerServerDestroyMessage.h"

void ThreadHandlerServerDestroyMessage::Start()
{
	if (nullptr == m_World)
	{
		return;
	}

	if (nullptr == m_GameInstance)
	{
		return;
	}

	if (m_World->GetName() != TEXT("LoginLevel"))
	{
		UGameplayStatics::OpenLevel(m_World, TEXT("LoginLevel"));
		PrintViewport(10.f, FColor::Yellow, TEXT("서버가 종료되어 LoginLevel로 돌아갑니다."));
		return;
	}
}
