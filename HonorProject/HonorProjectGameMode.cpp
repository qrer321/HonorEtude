// Copyright Epic Games, Inc. All Rights Reserved.

#include "HonorProjectGameMode.h"
#include "Character/HonorProjectCharacter.h"

AHonorProjectGameMode::AHonorProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/HonorProejct/Character/BPThirdPersonCharacter"));
	if (PlayerPawnBPClass.Succeeded())
		DefaultPawnClass = PlayerPawnBPClass.Class;
}