#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerUDPReadyOKMessage : public ThreadHandlerBase<UDPReadyOKMessage>
{
public:
	void Start() override;
};
