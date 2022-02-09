// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Global/GameInfo.h"
#include "GameFramework/GameMode.h"
#include "../Message/MessageComponent.h"
#include "LoginGameMode.generated.h"

UCLASS()
class HONORPROJECT_API ALoginGameMode : public AGameMode
{
	GENERATED_BODY()

private:
	UMessageComponent* m_MessageComponent;

public:
	ALoginGameMode();

public:
	virtual void Tick(float DeltaSeconds) override;
};
