#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerInsertSectionResultMessage : public ThreadHandlerBase<InsertSectionResultMessage>
{
public:
	void Start() override;
};
