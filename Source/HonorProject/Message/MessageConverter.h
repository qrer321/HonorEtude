#pragma once
#include <memory>
#include "GameServerMessage.h"

class MessageConverter
{
private:
	const std::vector<unsigned char>&	m_Buffer;
	std::shared_ptr<GameServerMessage>	m_Message;

public: // Default
	MessageConverter() = delete;
	explicit MessageConverter(const std::vector<unsigned char>& buffer);
	~MessageConverter() = default;

	MessageConverter(const MessageConverter& other) = delete;
	MessageConverter(MessageConverter&& other) noexcept;

	MessageConverter& operator=(const MessageConverter& other) = delete;
	MessageConverter& operator=(MessageConverter&& other) = delete;

public:
	[[nodiscard]] uint32_t GetMessageType() const { return m_Message->GetType(); }
	std::shared_ptr<GameServerMessage>& GetServerMessage() { return m_Message; }

	[[nodiscard]] bool IsValid() const { return nullptr != m_Message; }
};

inline MessageConverter::MessageConverter(MessageConverter&& other) noexcept
	: m_Buffer(other.m_Buffer)
	, m_Message(std::move(other.m_Message))
{
}