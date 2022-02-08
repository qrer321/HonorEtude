#include "Dispatcher.h"

bool Dispatcher::GetHandler(MessageType Type, MessageHandler& Handler) const
{
	return GetHandler(static_cast<unsigned int>(Type), Handler);
}

bool Dispatcher::GetHandler(unsigned int Type, MessageHandler& Handler) const
{
	auto FindData = m_Handlers.Find(Type);
	if (nullptr == FindData)
	{
		return false;
	}

	Handler = *FindData;
	return true;
}

bool Dispatcher::AddHandler(MessageType Type, const MessageHandler& Handler)
{
	return AddHandler(static_cast<unsigned int>(Type), Handler);
}

bool Dispatcher::AddHandler(unsigned int Type, const MessageHandler& Handler)
{
	m_Handlers.Add(Type, Handler);
	return true;
}