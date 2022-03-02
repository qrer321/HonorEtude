#include "ThreadHandlerObjectDestroyMessage.h"
#include "HonorProject/GameMode/PlayGameMode.h"

ThreadHandlerObjectDestroyMessage::ThreadHandlerObjectDestroyMessage(std::shared_ptr<ObjectDestroyMessage> Message)
	: m_World(nullptr)
	, m_GameInstance(nullptr)
	, m_Message(MoveTemp(Message))
{
}

ThreadHandlerObjectDestroyMessage::ThreadHandlerObjectDestroyMessage(ThreadHandlerObjectDestroyMessage&& Other) noexcept
	: m_World(Other.m_World)
	, m_GameInstance(Other.m_GameInstance)
	, m_Message(MoveTemp(Other.m_Message))
{
	Other.m_World = nullptr;
	Other.m_GameInstance = nullptr;
}

void ThreadHandlerObjectDestroyMessage::Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World)
{
	m_GameInstance = GameInstance;
	m_World = World;
}

void ThreadHandlerObjectDestroyMessage::Start()
{
	if (nullptr == m_World)
	{
		return;
	}

	if (nullptr == m_GameInstance)
	{
		return;
	}

	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(m_World));
	if (nullptr == PlayGameMode || false == PlayGameMode->IsValidLowLevel())
	{
		LOGSTRING(TEXT("PlayGameMode Is Not Valid"));
		return;
	}

	PlayGameMode->PushObjectMessage(m_Message->m_ObjectID, m_Message);
}
