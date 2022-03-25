#include "ThreadHandlerJoinResultMessage.h"

ThreadHandlerJoinResultMessage::ThreadHandlerJoinResultMessage(std::shared_ptr<JoinResultMessage> ResultMessage)
	: m_World(nullptr)
	, m_GameInstance(nullptr)
	, m_JoinResultMessage(MoveTemp(ResultMessage))
{
}

ThreadHandlerJoinResultMessage::ThreadHandlerJoinResultMessage(ThreadHandlerJoinResultMessage&& Other) noexcept
	: m_World(Other.m_World)
	, m_GameInstance(Other.m_GameInstance)
	, m_JoinResultMessage(MoveTemp(Other.m_JoinResultMessage))
{
	Other.m_World = nullptr;
	Other.m_GameInstance = nullptr;
}

void ThreadHandlerJoinResultMessage::Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World)
{
	m_GameInstance = GameInstance;
	m_World = World;	
}

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
