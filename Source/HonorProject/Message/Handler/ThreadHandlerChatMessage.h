#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerChatMessage	: public ThreadHandlerBase<ChatMessage>
{
public:
	void Start() override;
};
