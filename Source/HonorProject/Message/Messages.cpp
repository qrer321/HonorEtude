#include "Messages.h"

/////////////////////////////// GameServerMessage ///////////////////////////////
GameServerMessage::GameServerMessage(MessageType Type)
	: m_Type(Type)
	, m_Size(-1)
{
}

int GameServerMessage::SizeCheck()
{
	return -1;
}

unsigned int GameServerMessage::DataSizeCheck(const std::string& Value)
{
	return 4 + static_cast<unsigned int>(Value.size());
}

template <typename Type>
unsigned int GameServerMessage::DataSizeCheck(Type Value)
{
	return sizeof(Value);
}

void GameServerMessage::Serialize(GameServerSerializer& Serializer)
{
	Serializer << static_cast<int>(m_Type);
	Serializer << SizeCheck();
}

void GameServerMessage::Deserialize(GameServerSerializer& Deserializer)
{
	int Type;
	Deserializer >> Type;
	m_Type = static_cast<MessageType>(Type);
	Deserializer >> m_Size;
}


/////////////////////////////// LoginPacket ///////////////////////////////
LoginMessage::LoginMessage()
	: GameServerMessage(MessageType::Login)
{
}

int LoginMessage::SizeCheck()
{
	return static_cast<int>(DataSizeCheck(m_ID) + DataSizeCheck(m_PW));
}

void LoginMessage::Serialize(GameServerSerializer& Serializer)
{
	GameServerMessage::Serialize(Serializer);
	Serializer << m_ID;
	Serializer << m_PW;
}

void LoginMessage::Deserialize(GameServerSerializer& Deserializer)
{
	GameServerMessage::Deserialize(Deserializer);
	Deserializer >> m_ID;
	Deserializer >> m_PW;
}


/////////////////////////////// LoginResultPacket ///////////////////////////////
LoginResultMessage::LoginResultMessage()
	: GameServerMessage(MessageType::LoginResult)
	, m_Code(EGameServerCode::MAX)
{
}

int LoginResultMessage::SizeCheck()
{
	return static_cast<int>(DataSizeCheck(m_Code));
}

void LoginResultMessage::Serialize(GameServerSerializer& Serializer)
{
	GameServerMessage::Serialize(Serializer);
	Serializer << static_cast<int>(m_Code);
}

void LoginResultMessage::Deserialize(GameServerSerializer& Deserializer)
{
	GameServerMessage::Deserialize(Deserializer);

	int Value;
	Deserializer >> Value;
	m_Code = static_cast<EGameServerCode>(Value);
}
