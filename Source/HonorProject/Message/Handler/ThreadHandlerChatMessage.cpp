#include "ThreadHandlerChatMessage.h"
#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"

ThreadHandlerChatMessage::ThreadHandlerChatMessage(std::shared_ptr<ChatMessage> Message)
	: m_World(nullptr)
	, m_GameInstance(nullptr)
	, m_Message(MoveTemp(Message))
{
}

ThreadHandlerChatMessage::ThreadHandlerChatMessage(ThreadHandlerChatMessage&& Other) noexcept
	: m_World(Other.m_World)
	, m_GameInstance(Other.m_GameInstance)
	, m_Message(MoveTemp(Other.m_Message))
{
	Other.m_World = nullptr;
	Other.m_GameInstance = nullptr;
}

void ThreadHandlerChatMessage::Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World)
{
	m_GameInstance = GameInstance;
	m_World = World;
}

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
