#include "Messages.h"

/////////////////////////////// GameServerMessage ///////////////////////////////
GameServerMessage::GameServerMessage(MessageID ID)
	: m_ID(ID)
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
	Serializer << static_cast<int>(m_ID);
	Serializer << SizeCheck();
}

void GameServerMessage::Deserialize(GameServerSerializer& Deserializer)
{
	int ID;
	Deserializer >> ID;
	m_ID = static_cast<MessageID>(ID);
	Deserializer >> m_Size;
}


/////////////////////////////// LoginPacket ///////////////////////////////
LoginPacket::LoginPacket()
	: GameServerMessage(MessageID::Login)
{
}

int LoginPacket::SizeCheck()
{
	return static_cast<int>(DataSizeCheck(m_ID) + DataSizeCheck(m_PW));
}

void LoginPacket::Serialize(GameServerSerializer& serializer)
{
	GameServerMessage::Serialize(serializer);
	serializer << m_ID;
	serializer << m_PW;
}

void LoginPacket::Deserialize(GameServerSerializer& deserializer)
{
	GameServerMessage::Deserialize(deserializer);
	deserializer >> m_ID;
	deserializer >> m_PW;
}


/////////////////////////////// LoginResultPacket ///////////////////////////////
LoginResultPacket::LoginResultPacket()
	: GameServerMessage(MessageID::LoginResult)
	, m_Code(EGameServerCode::MAX)
{
}

int LoginResultPacket::SizeCheck()
{
	return static_cast<int>(DataSizeCheck(m_Code));
}

void LoginResultPacket::Serialize(GameServerSerializer& Serializer)
{
	GameServerMessage::Serialize(Serializer);
	Serializer << static_cast<int>(m_Code);
}

void LoginResultPacket::Deserialize(GameServerSerializer& Deserializer)
{
	GameServerMessage::Deserialize(Deserializer);

	int Value;
	Deserializer >> Value;
	m_Code = static_cast<EGameServerCode>(Value);
}
