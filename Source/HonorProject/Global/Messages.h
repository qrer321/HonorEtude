#pragma once
#include "GameServerSerializer.h"

enum class MessageID
{
	Login,
	LoginResult,
};

enum class EGameServerCode
{
	OK,
	LoginError,
	MAX,
};

class HONORPROJECT_API GameServerMessage
{
private: // Member Var
	MessageID		m_ID;
	unsigned int	m_Size;

public: // Default
	GameServerMessage() = delete;
	explicit GameServerMessage(MessageID ID);
	virtual ~GameServerMessage() = default;

	GameServerMessage(const GameServerMessage& Other) = delete;
	GameServerMessage(GameServerMessage&& Other) noexcept = delete;

public:
	GameServerMessage& operator=(const GameServerMessage& Other) = delete;
	GameServerMessage& operator=(GameServerMessage&& Other) = delete;

public:
	MessageID GetID() const { return m_ID; }

public: // Member Function
	virtual int SizeCheck() = 0;
	unsigned int DataSizeCheck(const std::string& Value);

	template<typename Type>
	unsigned int DataSizeCheck(Type Value);

	virtual void Serialize(GameServerSerializer& Serializer);
	virtual void Deserialize(GameServerSerializer& Deserializer);
};

class LoginPacket : public GameServerMessage
{
public:
	std::string m_ID;
	std::string m_PW;

public:
	LoginPacket();
	~LoginPacket() override = default;

	LoginPacket(const LoginPacket& Other) = delete;
	LoginPacket(LoginPacket&& Other) noexcept = delete;

public:
	LoginPacket& operator=(const LoginPacket& Other) = delete;
	LoginPacket& operator=(LoginPacket&& Other) = delete;

public:
	int SizeCheck() override;
	void Serialize(GameServerSerializer& Serializer) override;
	void Deserialize(GameServerSerializer& Deserializer) override;
};

class LoginResultPacket : public GameServerMessage
{
public:
	EGameServerCode m_Code;

public:
	LoginResultPacket();
	~LoginResultPacket() override = default;

	LoginResultPacket(const LoginResultPacket& Other) = delete;
	LoginResultPacket(LoginResultPacket&& Other) noexcept = delete;

public:
	LoginResultPacket& operator=(const LoginResultPacket& Other) = delete;
	LoginResultPacket& operator=(LoginResultPacket&& Other) = delete;

public:
	int SizeCheck() override;
	void Serialize(GameServerSerializer& Serializer) override;
	void Deserialize(GameServerSerializer& Deserializer) override;
};