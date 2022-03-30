#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerEnemyUpdateMessage : public ThreadHandlerBase<EnemyUpdateMessage>
{
public:
	void Start() override;
};
