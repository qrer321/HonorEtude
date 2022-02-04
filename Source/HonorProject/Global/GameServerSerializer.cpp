#include "GameServerSerializer.h"

GameServerSerializer::GameServerSerializer()
	: m_Offset(0)
{
	m_Data.SetNum(1024);
}

GameServerSerializer::GameServerSerializer(const TArray<uint8>& Data)
	: m_Offset(0)
	, m_Data(Data)
{
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
	Write(reinterpret_cast<const void*>(&Value[0]), static_cast<unsigned int>(Value.size()));
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
	int size;
	operator>>(size);
	Value.resize(size);
	Read(&Value[0], size);
}

void GameServerSerializer::operator>>(int& Value)
{
	Read(&Value, sizeof(int));
}

void GameServerSerializer::operator>>(unsigned int& Value)
{
	Read(&Value, sizeof(unsigned int));
}