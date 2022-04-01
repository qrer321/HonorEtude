#pragma once
#include "ThreadHandlerBase.h"

class HONORPROJECT_API ThreadHandlerCharacterListMessage : public ThreadHandlerBase<CharacterListMessage>
{
public:
	void Start() override;
};
