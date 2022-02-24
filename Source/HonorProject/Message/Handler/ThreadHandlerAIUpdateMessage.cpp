#include "ThreadHandlerAIUpdateMessage.h"

#include "HonorProject/GameMode/PlayGameMode.h"

ThreadHandlerAIUpdateMessage::ThreadHandlerAIUpdateMessage(std::shared_ptr<AIUpdateMessage> Message)
	: m_World(nullptr)
	, m_GameInstance(nullptr)
	, m_Message(MoveTemp(Message))
{
}

ThreadHandlerAIUpdateMessage::ThreadHandlerAIUpdateMessage(ThreadHandlerAIUpdateMessage&& Other) noexcept
	: m_World(Other.m_World)
	, m_GameInstance(Other.m_GameInstance)
	, m_Message(MoveTemp(Other.m_Message))
{
	Other.m_World = nullptr;
	Other.m_GameInstance = nullptr;
}

void ThreadHandlerAIUpdateMessage::Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World)
{
	m_GameInstance = GameInstance;
	m_World = World;
}

void ThreadHandlerAIUpdateMessage::Start()
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
		LOGSTRING(TEXT("GameMode Is Not Valid"));
		return;
	}

	TSubclassOf<AMasterAICharacter> AICharacter = PlayGameMode->GetEnemyAIClasses(m_Message->m_AIType);
	if (nullptr == AICharacter || false == AICharacter->IsValidLowLevel())
	{
		LOGSTRING(TEXT("AICharacter Is Not Valid"));
		return;
	}

	// 해당 캐릭터가 존재하지 않는다면
	AMasterAICharacter* NewAICharacter = nullptr;
	if (false == PlayGameMode->IsRegistered(m_Message->m_ObjectID))
	{
		FTransform CharacterTransform = FTransform(m_Message->m_Pos);

		NewAICharacter = m_World->SpawnActorDeferred<AMasterAICharacter>(AICharacter.Get(), CharacterTransform);
		NewAICharacter->FinishSpawning(CharacterTransform);
		
		PlayGameMode->RegisterObject(m_Message->m_ObjectID, NewAICharacter);
	}

	PlayGameMode->PushObjectMessage(m_Message->m_ObjectID, m_Message);
}
