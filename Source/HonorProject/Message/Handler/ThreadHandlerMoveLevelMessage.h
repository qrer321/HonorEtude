#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerMoveLevelMessage : public ThreadHandlerBase<MoveLevelMessage>
{
public:
	void Start() override;
};
