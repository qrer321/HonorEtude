#pragma once
#include <vector>
#include <string>

class HONORPROJECT_API GameServerSerializer
{
private:
	unsigned int	m_Offset;
	TArray<uint8>	m_Data;

public:
	GameServerSerializer();
	explicit GameServerSerializer(const TArray<uint8>& Data);
	~GameServerSerializer() = default;

	//GameServerSerializer(const GameServerSerializer& other) = delete;
	//GameServerSerializer(GameServerSerializer&& other) noexcept = delete;

	//GameServerSerializer& operator=(const GameServerSerializer& other) = delete;
	//GameServerSerializer& operator=(GameServerSerializer&& other) = delete;

public:
	void operator<<(const std::string& Value);
	void operator<<(const int Value);
	void operator<<(const unsigned int Value);

	void operator>>(std::string& Value);
	void operator>>(int& Value);
	void operator>>(unsigned int& Value);

public:
	const TArray<uint8>& GetData() { return m_Data; }

public:
	void Read(void* Data, unsigned int Size);
	void Write(const void* Data, unsigned int Size);
};

