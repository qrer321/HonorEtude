#include "MessageConverter.h"

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
		Packet = std::make_shared<LoginPacket>();
		break;
	case MessageID::LoginResult:
		Packet = std::make_shared<LoginResultPacket>();
		break;
	default:
		break;
	}

	Packet->Deserialize(Serializer);
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
