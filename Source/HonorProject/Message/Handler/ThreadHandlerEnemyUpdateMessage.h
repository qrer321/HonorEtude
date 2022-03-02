#pragma once
#include "HonorProject/Global/GameInfo.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "../Messages.h"

class HONORPROJECT_API ThreadHandlerEnemyUpdateMessage
{
private:
	UWorld*									m_World;
	UHonorProjectGameInstance*				m_GameInstance;
	std::shared_ptr<EnemyUpdateMessage>	m_Message;

	public: // Default
	ThreadHandlerEnemyUpdateMessage() = delete;
	explicit ThreadHandlerEnemyUpdateMessage(std::shared_ptr<EnemyUpdateMessage> Message);
	~ThreadHandlerEnemyUpdateMessage() = default;

	ThreadHandlerEnemyUpdateMessage(const ThreadHandlerEnemyUpdateMessage& Other) = delete;
	ThreadHandlerEnemyUpdateMessage(ThreadHandlerEnemyUpdateMessage&& Other) noexcept;

	ThreadHandlerEnemyUpdateMessage& operator=(const ThreadHandlerEnemyUpdateMessage& Other) = delete;
	ThreadHandlerEnemyUpdateMessage& operator=(ThreadHandlerEnemyUpdateMessage&& Other) = delete;

public:
	void Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World);
	void Start();
};
