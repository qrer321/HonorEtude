#pragma once
#include "HonorProject/Global/GameInfo.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "HonorProject/Message/Messages.h"
#include "HonorProject/Message/GameServerMessage.h"
#include "HonorProject/Message/ServerAndClient.h"
#include "HonorProject/Message/ServerToClient.h"
#include "HonorProject/Message/ClientToServer.h"

template <class MessageType>
class ThreadHandlerBase : public std::enable_shared_from_this<ThreadHandlerBase<MessageType>>
{
protected:
	UWorld*							m_World;
	UHonorProjectGameInstance*		m_GameInstance;
	std::shared_ptr<MessageType>	m_Message;
	
public: // Default
	ThreadHandlerBase() = default;
	virtual ~ThreadHandlerBase() = default;

	ThreadHandlerBase(const ThreadHandlerBase& Other) = delete;
	ThreadHandlerBase(ThreadHandlerBase&& Other) noexcept = default;

	ThreadHandlerBase& operator=(const ThreadHandlerBase& Other) = default;
	ThreadHandlerBase& operator=(ThreadHandlerBase&& Other) = delete;

public:
	void Initialize(std::shared_ptr<MessageType> Message, UHonorProjectGameInstance* GameInstance, UWorld* World)
	{
		m_Message = std::move(Message);
		m_GameInstance = GameInstance;
		m_World = World;
	}
	
	virtual void Start() = 0;
};
