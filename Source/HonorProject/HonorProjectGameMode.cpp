// Copyright Epic Games, Inc. All Rights Reserved.

#include "HonorProjectGameMode.h"
#include "Character/PlayerCharacter.h"
#include "Character/CharacterController.h"

AHonorProjectGameMode::AHonorProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/HonorProejct/Character/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Succeeded())
		DefaultPawnClass = PlayerPawnBPClass.Class;
	
	PlayerControllerClass = ACharacterController::StaticClass();
}
