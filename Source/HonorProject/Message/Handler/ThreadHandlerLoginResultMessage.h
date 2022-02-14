#pragma once
#include "HonorProject/Global/GameInfo.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "../Messages.h"

class HONORPROJECT_API ThreadHandlerLoginResultMessage
{
private:
	UWorld*								m_World;
	UHonorProjectGameInstance*			m_GameInstance;
	std::shared_ptr<LoginResultMessage>	m_LoginResultMessage;

	public: // Default
	ThreadHandlerLoginResultMessage() = delete;
	explicit ThreadHandlerLoginResultMessage(std::shared_ptr<LoginResultMessage> ResultMessage);
	~ThreadHandlerLoginResultMessage() = default;

	ThreadHandlerLoginResultMessage(const ThreadHandlerLoginResultMessage& Other) = delete;
	ThreadHandlerLoginResultMessage(ThreadHandlerLoginResultMessage&& Other) noexcept;

	ThreadHandlerLoginResultMessage& operator=(const ThreadHandlerLoginResultMessage& Other) = delete;
	ThreadHandlerLoginResultMessage& operator=(ThreadHandlerLoginResultMessage&& Other) = delete;

public:
	void Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World);
	void Start();
};
