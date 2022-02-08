#pragma once
#include "Messages.h"
#include <vector>
#include <memory>

class HONORPROJECT_API MessageConverter
{
private:
	const TArray<uint8>&				m_Buffer;
	std::shared_ptr<GameServerMessage>	m_Message;

public: // Default
	MessageConverter() = delete;
	explicit MessageConverter(const TArray<uint8>& Buffer);
	~MessageConverter() = default;

	MessageConverter(const MessageConverter& Other) = delete;
	MessageConverter(MessageConverter&& Other) noexcept;

	MessageConverter& operator=(const MessageConverter& Other) = delete;
	MessageConverter& operator=(MessageConverter&& Other) = delete;

public:
	MessageType GetMessageID() const;
	uint32_t GetMessageID_UINT() const;
	std::shared_ptr<GameServerMessage> GetServerMessage();
};

