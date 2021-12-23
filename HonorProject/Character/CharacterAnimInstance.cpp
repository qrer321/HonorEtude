// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "HonorProjectCharacter.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	m_PlayerCharacter = nullptr;
	m_IsinAir = false;
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		AHonorProjectCharacter* PlayerCharacter = Cast<AHonorProjectCharacter>(PlayerController->GetPawn());
		if (IsValid(PlayerCharacter))
			m_PlayerCharacter = PlayerCharacter;
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const AHonorProjectCharacter* HonorCharacter = Cast<AHonorProjectCharacter>(TryGetPawnOwner());
	if (IsValid(HonorCharacter))
	{
		UCharacterMovementComponent* Movement = HonorCharacter->GetCharacterMovement();
		if (Movement)
		{
			m_IsinAir = Movement->IsFalling();
			m_Speed = HonorCharacter->GetVelocity().Size();
		}
	}
}
