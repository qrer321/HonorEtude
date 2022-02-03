// Fill out your copyright notice in the Description page of Project Settings.


#include "GameServerPacket.h"

/////////////////////////////// GameServerPacket ///////////////////////////////
GameServerPacket::GameServerPacket(PacketType Type)
	: m_Type(Type)
	, m_Size(-1)
{
}

int GameServerPacket::SizeCheck()
{
	return -1;
}

unsigned int GameServerPacket::DataSizeCheck(const std::string& Value)
{
	return 4 + static_cast<unsigned int>(Value.size());
}

void GameServerPacket::Serialize(GameServerSerializer& Serializer)
{
	Serializer << static_cast<int>(m_Type);
	Serializer << SizeCheck();
}

void GameServerPacket::Deserialize(GameServerSerializer& Deserializer)
{
	int Type;
	Deserializer >> Type;
	m_Type = static_cast<PacketType>(Type);
	Deserializer >> m_Size;
}


/////////////////////////////// LoginPacket ///////////////////////////////
LoginPacket::LoginPacket()
	: GameServerPacket(PacketType::Login)
{
}

int LoginPacket::SizeCheck()
{
	return DataSizeCheck(m_ID) + DataSizeCheck(m_PW);
}

void LoginPacket::Serialize(GameServerSerializer& Serializer)
{
	GameServerPacket::Serialize(Serializer);
	Serializer << m_ID;
	Serializer << m_PW;
}

void LoginPacket::Deserialize(GameServerSerializer& Deserializer)
{
	GameServerPacket::Deserialize(Deserializer);
	Deserializer >> m_ID;
	Deserializer >> m_PW;
}