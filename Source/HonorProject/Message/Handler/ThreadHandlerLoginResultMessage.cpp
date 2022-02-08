#include "ThreadHandlerLoginResultMessage.h"

ThreadHandlerLoginResultMessage::ThreadHandlerLoginResultMessage(std::shared_ptr<LoginResultMessage> ResultMessage)
	: m_World(nullptr)
	, m_GameInstance(nullptr)
	, m_LoginResultMessage(ResultMessage)
{
}

ThreadHandlerLoginResultMessage::ThreadHandlerLoginResultMessage(ThreadHandlerLoginResultMessage&& Other) noexcept
	: m_World(Other.m_World)
	, m_GameInstance(Other.m_GameInstance)
	, m_LoginResultMessage(std::move(Other.m_LoginResultMessage))
{
	Other.m_World = nullptr;
	Other.m_GameInstance = nullptr;
}

void ThreadHandlerLoginResultMessage::Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World)
{
	m_GameInstance = GameInstance;
	m_World = World;
}

void ThreadHandlerLoginResultMessage::Start()
{
	if (EGameServerCode::OK == m_LoginResultMessage->m_Code)
	{
		UGameplayStatics::OpenLevel(m_World, TEXT("PlayTestLevel"));
		return;
	}
}
