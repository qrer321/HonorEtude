// Fill out your copyright notice in the Description page of Project Settings.


#include "HonorProjectCharacter.h"

// Sets default values
AHonorProjectCharacter::AHonorProjectCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHonorProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHonorProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHonorProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHonorProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHonorProjectCharacter, m_CharacterInfo);
}

float AHonorProjectCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Damage = Damage - m_CharacterInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;
	
	return Damage;
}
