#include "ThreadHandlerJoinResultMessage.h"

void ThreadHandlerJoinResultMessage::Start()
{
	if (nullptr == m_World)
	{
		return;
	}

	if (nullptr == m_GameInstance)
	{
		return;
	}

	// Join이 성공적으로 완료되었다면
	// 서버가 발송한 JoinResultMessage를 통해 현재 위치까지 오게된다.
	int a = 0;
}
