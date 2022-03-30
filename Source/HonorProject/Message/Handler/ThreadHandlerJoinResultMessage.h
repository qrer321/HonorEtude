#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerJoinResultMessage : public ThreadHandlerBase<JoinResultMessage>
{
public:
	void Start() override;
};
