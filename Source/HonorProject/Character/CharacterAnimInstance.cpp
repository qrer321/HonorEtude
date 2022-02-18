// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "HonorProjectCharacter.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	m_Character = nullptr;
	m_IsinAir = false;
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	AHonorProjectCharacter* Character = Cast<AHonorProjectCharacter>(TryGetPawnOwner());
	if (IsValid(Character))
		m_Character = Character;
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(m_Character))
	{	
		UCharacterMovementComponent* Movement = m_Character->GetCharacterMovement();
		if (Movement)
		{
			m_IsinAir = Movement->IsFalling();

			// 플레이어의 Velocity와 Forward, Right 벡터를 내적하여
			// Forward와 Right의 Speed를 계산한다.
			const FVector CharacterVelocity = m_Character->GetVelocity();
			const FVector CharacterForward = m_Character->GetActorForwardVector();
			const FVector CharacterRight = m_Character->GetActorRightVector();

			const float DotForward = UKismetMathLibrary::Dot_VectorVector(CharacterVelocity, CharacterForward);
			const float DotRight = UKismetMathLibrary::Dot_VectorVector(CharacterVelocity, CharacterRight);
			
			m_SpeedForward = DotForward;
			m_SpeedRight = DotRight;
		}

		m_IsCombatMode = m_Character->IsCombatMode();
	}
}