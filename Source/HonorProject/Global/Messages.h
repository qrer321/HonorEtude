#pragma once
#include <memory>
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

class HONORPROJECT_API GameServerMessage : public std::enable_shared_from_this<GameServerMessage>
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

class LoginMessage : public GameServerMessage
{
public:
	std::string m_ID;
	std::string m_PW;

public:
	LoginMessage();
	~LoginMessage() override = default;

	LoginMessage(const LoginMessage& Other) = delete;
	LoginMessage(LoginMessage&& Other) noexcept = delete;

public:
	LoginMessage& operator=(const LoginMessage& Other) = delete;
	LoginMessage& operator=(LoginMessage&& Other) = delete;

public:
	int SizeCheck() override;
	void Serialize(GameServerSerializer& Serializer) override;
	void Deserialize(GameServerSerializer& Deserializer) override;
};

class LoginResultMessage : public GameServerMessage
{
public:
	EGameServerCode m_Code;

public:
	LoginResultMessage();
	~LoginResultMessage() override = default;

	LoginResultMessage(const LoginResultMessage& Other) = delete;
	LoginResultMessage(LoginResultMessage&& Other) noexcept = delete;

public:
	LoginResultMessage& operator=(const LoginResultMessage& Other) = delete;
	LoginResultMessage& operator=(LoginResultMessage&& Other) = delete;

public:
	int SizeCheck() override;
	void Serialize(GameServerSerializer& Serializer) override;
	void Deserialize(GameServerSerializer& Deserializer) override;
};