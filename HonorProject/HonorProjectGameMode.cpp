// Copyright Epic Games, Inc. All Rights Reserved.

#include "HonorProjectGameMode.h"
#include "Character/HonorProjectCharacter.h"

AHonorProjectGameMode::AHonorProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/HonorProejct/Character/BPThirdPersonCharacter.BPThirdPersonCharacter_C'"));
	if (PlayerPawnBPClass.Succeeded())
		DefaultPawnClass = PlayerPawnBPClass.Class;
}