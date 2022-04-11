#include "ThreadHandlerInsertSectionResultMessage.h"

void ThreadHandlerInsertSectionResultMessage::Start()
{
	if (EGameServerCode::SelectCharacterError == m_Message->m_Code)
	{
		m_GameInstance->m_LoginProcess = false;
		UGameplayStatics::OpenLevel(m_World, TEXT("LoginLevel"));
		return;
	}

	m_GameInstance->m_LoginProcess = false;
	m_GameInstance->m_ActorIndex = m_Message->m_ActorIndex;
	m_GameInstance->m_ThreadIndex = m_Message->m_ThreadIndex;
	m_GameInstance->m_SectionIndex = m_Message->m_SectionIndex;
	
	UGameplayStatics::OpenLevel(m_World, TEXT("PlayTestLevel"));
}