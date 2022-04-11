#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerPlayerUpdateMessage : public ThreadHandlerBase<PlayerUpdateMessage>
{
public:
	void Start() override;
};
