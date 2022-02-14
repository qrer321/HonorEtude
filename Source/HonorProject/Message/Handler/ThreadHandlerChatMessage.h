#pragma once
#include "HonorProject/Global/GameInfo.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "../Messages.h"

class HONORPROJECT_API ThreadHandlerChatMessage
{
private:
	UWorld*							m_World;
	UHonorProjectGameInstance*		m_GameInstance;
	std::shared_ptr<ChatMessage>	m_Message;

	public: // Default
	ThreadHandlerChatMessage() = delete;
	explicit ThreadHandlerChatMessage(std::shared_ptr<ChatMessage> Message);
	~ThreadHandlerChatMessage() = default;

	ThreadHandlerChatMessage(const ThreadHandlerChatMessage& Other) = delete;
	ThreadHandlerChatMessage(ThreadHandlerChatMessage&& Other) noexcept;

	ThreadHandlerChatMessage& operator=(const ThreadHandlerChatMessage& Other) = delete;
	ThreadHandlerChatMessage& operator=(ThreadHandlerChatMessage&& Other) = delete;

public:
	void Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World);
	void Start();
};
