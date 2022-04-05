#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerSelectCharacterResultMessage : public ThreadHandlerBase<SelectCharacterResultMessage>
{
public:
	void Start() override;
};
