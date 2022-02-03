// Fill out your copyright notice in the Description page of Project Settings.


#include "GameServerSerializer.h"

GameServerSerializer::GameServerSerializer()
	: m_Offset(0)
{
	m_Data.SetNum(1024);
}

void GameServerSerializer::Read(void* Data, unsigned int Size)
{
	memcpy_s(Data, Size, &m_Data[m_Offset], Size);
	m_Offset += Size;
}

void GameServerSerializer::Write(const void* Data, unsigned int Size)
{
	memcpy_s(&m_Data[m_Offset], Size, Data, Size);
	m_Offset += Size;
}

void GameServerSerializer::operator<<(const std::string& Value)
{
	operator<<(static_cast<int>(Value.size()));
	Write(reinterpret_cast<const void*>(&Value[0]), Value.size());
}

void GameServerSerializer::operator<<(const int Value)
{
	Write(&Value, sizeof(int));
}

void GameServerSerializer::operator<<(const unsigned int Value)
{
	Write(&Value, sizeof(unsigned int));
}

void GameServerSerializer::operator>>(std::string& Value)
{
	int Size;
	operator>>(Size);
	Value.resize(Size);
	Read(&Value[0], Size);
}

void GameServerSerializer::operator>>(int& Value)
{
	Read(&Value, sizeof(int));
}

void GameServerSerializer::operator>>(unsigned int& Value)
{
	Read(&Value, sizeof(unsigned int));
}


