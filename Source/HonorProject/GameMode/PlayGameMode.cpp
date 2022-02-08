// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameMode.h"
#include "../Character/CharacterController.h"

APlayGameMode::APlayGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/HonorProejct/PlayRelevant/Character/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawnBPClass.Succeeded())
		DefaultPawnClass = PlayerPawnBPClass.Class;
	
	PlayerControllerClass = ACharacterController::StaticClass();
}
