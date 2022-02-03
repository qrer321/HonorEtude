// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameServerSerializer.h"
#include <string>

enum class PacketType
{
	Login,
};

class HONORPROJECT_API GameServerPacket
{
private:
	PacketType		m_Type;
	unsigned int	m_Size;
	
public:
	GameServerPacket() = delete;
	GameServerPacket(PacketType Type);
	virtual ~GameServerPacket() = default;

public:
	virtual int SizeCheck() = 0;
	unsigned int DataSizeCheck(const std::string& Value);
	void Serialize(GameServerSerializer& Serializer);
	void Deserialize(GameServerSerializer& Deserializer);
};

class HONORPROJECT_API LoginPacket : public GameServerPacket
{
public:
	std::string m_ID;
	std::string m_PW;

public:
	LoginPacket();
	virtual ~LoginPacket() override = default;

public:
	virtual int SizeCheck() override;
	void Serialize(GameServerSerializer& Serializer);
	void Deserialize(GameServerSerializer& Deserializer);
};




