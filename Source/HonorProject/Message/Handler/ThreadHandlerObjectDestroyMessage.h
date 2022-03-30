#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerObjectDestroyMessage : public ThreadHandlerBase<ObjectDestroyMessage>
{
public:
	void Start() override;
};
