// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginGameMode.h"
#include "../Global/HonorProjectGameInstance.h"

ALoginGameMode::ALoginGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	m_MessageComponent = CreateDefaultSubobject<UMessageComponent>(TEXT("MessageComponent"));
}

void ALoginGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
