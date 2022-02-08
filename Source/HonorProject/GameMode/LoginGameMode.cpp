// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginGameMode.h"
#include "../HonorProjectGameInstance.h"

ALoginGameMode::ALoginGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

void ALoginGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());

	if (false == GameInstance->GetMessageQueue().IsEmpty())
	{
		// 
	}
}
