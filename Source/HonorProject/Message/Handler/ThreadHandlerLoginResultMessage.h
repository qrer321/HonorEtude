#pragma once
#include "../../Global/GameInfo.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"

class HONORPROJECT_API ThreadHandlerLoginResultMessage
{
private:
	UWorld*							m_World;
	UHonorProjectGameInstance*		m_GameInstance;
	TSharedPtr<LoginResultMessage>	m_LoginResultMessage;

	public: // Default
	ThreadHandlerLoginResultMessage() = delete;
	explicit ThreadHandlerLoginResultMessage(TSharedPtr<LoginResultMessage> ResultMessage);
	~ThreadHandlerLoginResultMessage() = default;

	ThreadHandlerLoginResultMessage(const ThreadHandlerLoginResultMessage& Other) = delete;
	ThreadHandlerLoginResultMessage(ThreadHandlerLoginResultMessage&& Other) noexcept;

	ThreadHandlerLoginResultMessage& operator=(const ThreadHandlerLoginResultMessage& Other) = delete;
	ThreadHandlerLoginResultMessage& operator=(ThreadHandlerLoginResultMessage&& Other) = delete;

public:
	void Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World);
	void Start();
};
