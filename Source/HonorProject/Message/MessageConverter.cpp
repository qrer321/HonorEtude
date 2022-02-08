#include "MessageConverter.h"
#include "GameServerSerializer.h"

MessageConverter::MessageConverter(const TArray<uint8>& Buffer)
	: m_Buffer(Buffer)
{
	GameServerSerializer Serializer = GameServerSerializer(Buffer);
	MessageType ID;

	memcpy_s(&ID, sizeof(MessageType), &Buffer[0], sizeof(MessageType));
	switch (ID)
	{
	case MessageType::Login:
		m_Message = std::make_shared<LoginMessage>();
		break;
	case MessageType::LoginResult:
		m_Message = std::make_shared<LoginResultMessage>();
		break;
	default:
		break;
	}

	m_Message->Deserialize(Serializer);
}

MessageConverter::MessageConverter(MessageConverter&& Other) noexcept
	: m_Buffer(Other.m_Buffer)
	, m_Message(MoveTempIfPossible(Other.m_Message))
{
}

MessageType MessageConverter::GetMessageID() const
{
	return m_Message->GetType();
}

uint32_t MessageConverter::GetMessageID_UINT() const
{
	return static_cast<uint32_t>(GetMessageID());
}

std::shared_ptr<GameServerMessage> MessageConverter::GetServerMessage()
{
	return m_Message;
}
