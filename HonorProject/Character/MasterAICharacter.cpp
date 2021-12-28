// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterAICharacter.h"

// Sets default values
AMasterAICharacter::AMasterAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_TargetDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Target Decal"));
	m_TargetDecal->SetupAttachment(GetMesh());
	m_TargetDecal->SetVisibility(false);
}

// Called when the game starts or when spawned
void AMasterAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMasterAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMasterAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

