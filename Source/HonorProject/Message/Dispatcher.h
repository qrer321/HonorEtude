#pragma once
#include "../Global/GameInfo.h"
#include "Messages.h"
#include <memory>

using MessageHandler = TFunction<void(std::shared_ptr<GameServerMessage>)>;

class Dispatcher
{
private:
	TMap<unsigned int, MessageHandler> m_Handlers;

public: // Default
	Dispatcher() = default;
	~Dispatcher() = default;

	Dispatcher(const Dispatcher& Other) = delete;
	Dispatcher(Dispatcher&& Other) noexcept = delete;

	Dispatcher& operator=(const Dispatcher& Other) = delete;
	Dispatcher& operator=(Dispatcher&& Other) = delete;

public:
	bool GetHandler(MessageType Type, MessageHandler& Handler) const;
	bool GetHandler(unsigned int Type, MessageHandler& Handler) const;

public: // Member Function
	bool AddHandler(MessageType Type, const MessageHandler& Handler);
	bool AddHandler(unsigned int Type, const MessageHandler& Handler);
};