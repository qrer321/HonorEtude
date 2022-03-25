#include "ThreadHandlerLoginResultMessage.h"

ThreadHandlerLoginResultMessage::ThreadHandlerLoginResultMessage(std::shared_ptr<LoginResultMessage> ResultMessage)
	: m_World(nullptr)
	, m_GameInstance(nullptr)
	, m_LoginResultMessage(MoveTemp(ResultMessage))
{
}

ThreadHandlerLoginResultMessage::ThreadHandlerLoginResultMessage(ThreadHandlerLoginResultMessage&& Other) noexcept
	: m_World(Other.m_World)
	, m_GameInstance(Other.m_GameInstance)
	, m_LoginResultMessage(MoveTemp(Other.m_LoginResultMessage))
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
	if (nullptr == m_World)
	{
		return;
	}

	if (nullptr == m_GameInstance)
	{
		return;
	}
	
	if (EGameServerCode::OK == m_LoginResultMessage->m_Code)
	{
		//UGameplayStatics::OpenLevel(m_World, TEXT("PlayTestLevel"));
		PrintViewport(10.f, FColor::Red, TEXT("Login Succeed"));
		return;
	}
}
