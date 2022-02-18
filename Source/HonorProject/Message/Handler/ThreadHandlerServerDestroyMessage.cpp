#include "ThreadHandlerServerDestroyMessage.h"

ThreadHandlerServerDestroyMessage::ThreadHandlerServerDestroyMessage(std::shared_ptr<ServerDestroyMessage> Message)
	: m_World(nullptr)
	, m_GameInstance(nullptr)
	, m_Message(MoveTemp(Message))
{
}

ThreadHandlerServerDestroyMessage::ThreadHandlerServerDestroyMessage(ThreadHandlerServerDestroyMessage&& Other) noexcept
	: m_World(Other.m_World)
	, m_GameInstance(Other.m_GameInstance)
	, m_Message(MoveTemp(Other.m_Message))
{
	Other.m_World = nullptr;
	Other.m_GameInstance = nullptr;
}

void ThreadHandlerServerDestroyMessage::Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World)
{
	m_GameInstance = GameInstance;
	m_World = World;
}

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
