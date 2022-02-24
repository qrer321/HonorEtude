#pragma once
#include "HonorProject/Global/GameInfo.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "../Messages.h"

class HONORPROJECT_API ThreadHandlerAIUpdateMessage
{
private:
	UWorld*									m_World;
	UHonorProjectGameInstance*				m_GameInstance;
	std::shared_ptr<AIUpdateMessage>	m_Message;

	public: // Default
	ThreadHandlerAIUpdateMessage() = delete;
	explicit ThreadHandlerAIUpdateMessage(std::shared_ptr<AIUpdateMessage> Message);
	~ThreadHandlerAIUpdateMessage() = default;

	ThreadHandlerAIUpdateMessage(const ThreadHandlerAIUpdateMessage& Other) = delete;
	ThreadHandlerAIUpdateMessage(ThreadHandlerAIUpdateMessage&& Other) noexcept;

	ThreadHandlerAIUpdateMessage& operator=(const ThreadHandlerAIUpdateMessage& Other) = delete;
	ThreadHandlerAIUpdateMessage& operator=(ThreadHandlerAIUpdateMessage&& Other) = delete;

public:
	void Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World);
	void Start();
};
