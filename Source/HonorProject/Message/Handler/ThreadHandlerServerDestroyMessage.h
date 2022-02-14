#pragma once
#include "HonorProject/Global/GameInfo.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "../Messages.h"

class HONORPROJECT_API ThreadHandlerServerDestroyMessage
{
private:
	UWorld*									m_World;
	UHonorProjectGameInstance*				m_GameInstance;
	std::shared_ptr<ServerDestroyMessage>	m_Message;

	public: // Default
	ThreadHandlerServerDestroyMessage() = delete;
	explicit ThreadHandlerServerDestroyMessage(std::shared_ptr<ServerDestroyMessage> Message);
	~ThreadHandlerServerDestroyMessage() = default;

	ThreadHandlerServerDestroyMessage(const ThreadHandlerServerDestroyMessage& Other) = delete;
	ThreadHandlerServerDestroyMessage(ThreadHandlerServerDestroyMessage&& Other) noexcept;

	ThreadHandlerServerDestroyMessage& operator=(const ThreadHandlerServerDestroyMessage& Other) = delete;
	ThreadHandlerServerDestroyMessage& operator=(ThreadHandlerServerDestroyMessage&& Other) = delete;

public:
	void Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World);
	void Start();
};
