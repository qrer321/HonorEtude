#pragma once
#include "HonorProject/Global/GameInfo.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "../Messages.h"

class HONORPROJECT_API ThreadHandlerJoinResultMessage
{
private:
	UWorld*								m_World;
	UHonorProjectGameInstance*			m_GameInstance;
	std::shared_ptr<JoinResultMessage>	m_JoinResultMessage;

	public: // Default
	ThreadHandlerJoinResultMessage() = delete;
	explicit ThreadHandlerJoinResultMessage(std::shared_ptr<JoinResultMessage> ResultMessage);
	~ThreadHandlerJoinResultMessage() = default;

	ThreadHandlerJoinResultMessage(const ThreadHandlerJoinResultMessage& Other) = delete;
	ThreadHandlerJoinResultMessage(ThreadHandlerJoinResultMessage&& Other) noexcept;

	ThreadHandlerJoinResultMessage& operator=(const ThreadHandlerJoinResultMessage& Other) = delete;
	ThreadHandlerJoinResultMessage& operator=(ThreadHandlerJoinResultMessage&& Other) = delete;

public:
	void Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World);
	void Start();
};
