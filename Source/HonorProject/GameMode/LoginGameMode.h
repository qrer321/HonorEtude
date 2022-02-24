// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Global/GameInfo.h"
#include "GameFramework/GameMode.h"
#include "HonorProject/Component/MessageComponent.h"
#include "LoginGameMode.generated.h"

UCLASS()
class HONORPROJECT_API ALoginGameMode : public AGameMode
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMessageComponent* m_MessageComponent;

public:
	ALoginGameMode();

public:
	virtual void Tick(float DeltaSeconds) override;
};
