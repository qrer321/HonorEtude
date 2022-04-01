#include "ThreadHandlerLoginResultMessage.h"

void ThreadHandlerLoginResultMessage::Start()
{
	if (EGameServerCode::OK == m_Message->m_Code)
	{
		return;
	}

	m_GameInstance->m_LoginProcess = false;
}
