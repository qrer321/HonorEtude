#include "ThreadHandlerChatMessage.h"
#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"

void ThreadHandlerChatMessage::Start()
{
	if (nullptr == m_World)
	{
		return;
	}

	if (nullptr == m_GameInstance)
	{
		return;
	}
	
	FString ID;
	FString Message;

	UClientBlueprintFunctionLibrary::UTF8ToFString(m_Message->m_ID, ID);
	UClientBlueprintFunctionLibrary::UTF8ToFString(m_Message->m_Message, Message);

	m_GameInstance->m_ChatWindow->AddMessage(ID, Message);
}
