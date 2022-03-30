#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerLoginResultMessage : public ThreadHandlerBase<LoginResultMessage>
{
public:
	void Start() override;
};
