#pragma once
#include "HonorProject/Global/GameInfo.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "../Messages.h"

class HONORPROJECT_API ThreadHandlerObjectDestroyMessage
{
private:
	UWorld*									m_World;
	UHonorProjectGameInstance*				m_GameInstance;
	std::shared_ptr<ObjectDestroyMessage>	m_Message;

	public: // Default
	ThreadHandlerObjectDestroyMessage() = delete;
	explicit ThreadHandlerObjectDestroyMessage(std::shared_ptr<ObjectDestroyMessage> Message);
	~ThreadHandlerObjectDestroyMessage() = default;

	ThreadHandlerObjectDestroyMessage(const ThreadHandlerObjectDestroyMessage& Other) = delete;
	ThreadHandlerObjectDestroyMessage(ThreadHandlerObjectDestroyMessage&& Other) noexcept;

	ThreadHandlerObjectDestroyMessage& operator=(const ThreadHandlerObjectDestroyMessage& Other) = delete;
	ThreadHandlerObjectDestroyMessage& operator=(ThreadHandlerObjectDestroyMessage&& Other) = delete;

public:
	void Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World);
	void Start();
};
