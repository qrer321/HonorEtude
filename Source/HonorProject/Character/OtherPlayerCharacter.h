// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "HonorProjectCharacter.h"
#include "OtherPlayerCharacter.generated.h"

UCLASS()
class HONORPROJECT_API AOtherPlayerCharacter : public AHonorProjectCharacter
{
	GENERATED_BODY()

public:
	AOtherPlayerCharacter();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
