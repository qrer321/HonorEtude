#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerServerDestroyMessage : public ThreadHandlerBase<ServerDestroyMessage>
{
public:
	void Start() override;
};
