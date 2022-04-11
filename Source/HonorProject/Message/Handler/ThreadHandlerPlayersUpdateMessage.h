#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerPlayersUpdateMessage : public ThreadHandlerBase<PlayersUpdateMessage>
{
public:
	void Start() override;
};
