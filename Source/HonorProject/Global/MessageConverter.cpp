#include "MessageConverter.h"
#include "GameServerSerializer.h"

MessageConverter::MessageConverter(const TArray<uint8>& Buffer)
	: m_Buffer(Buffer)
{
	GameServerSerializer Serializer = GameServerSerializer(Buffer);
	MessageID ID;

	memcpy_s(&ID, sizeof(MessageID), &Buffer[0], sizeof(MessageID));
	std::shared_ptr<GameServerMessage> Packet = nullptr;
	switch (ID)
	{
	case MessageID::Login:
		Packet = std::make_shared<LoginMessage>();
		break;
	case MessageID::LoginResult:
		Packet = std::make_shared<LoginResultMessage>();
		break;
	default:
		break;
	}

	Packet->Deserialize(Serializer);
}

MessageConverter::MessageConverter(MessageConverter&& Other) noexcept
	: m_Buffer(std::move(Other.m_Buffer))
	, m_Message(MoveTempIfPossible(Other.m_Message))
{
}

MessageID MessageConverter::GetMessageID() const
{
	return m_Message->GetID();
}

uint32_t MessageConverter::GetMessageID_UINT() const
{
	return static_cast<uint32_t>(GetMessageID());
}

std::shared_ptr<GameServerMessage> MessageConverter::GetServerMessage()
{
	return m_Message;
}
