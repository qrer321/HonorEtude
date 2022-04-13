#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerPlayerDestroyMessage : public ThreadHandlerBase<PlayerDestroyMessage>
{
public:
	void Start() override;
};
