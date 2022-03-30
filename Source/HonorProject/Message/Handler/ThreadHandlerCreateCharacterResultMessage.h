#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerCreateCharacterResultMessage : public ThreadHandlerBase<CreateCharacterResultMessage>
{
public:
	void Start() override;
};
