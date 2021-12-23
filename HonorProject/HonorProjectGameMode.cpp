// Copyright Epic Games, Inc. All Rights Reserved.

#include "HonorProjectGameMode.h"
#include "Character/HonorProjectCharacter.h"

AHonorProjectGameMode::AHonorProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/BaseContent/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter_C'"));
	if (PlayerPawnBPClass.Succeeded())
		DefaultPawnClass = PlayerPawnBPClass.Class;
}
