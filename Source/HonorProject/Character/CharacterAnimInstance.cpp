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

	AHonorProjectCharacter* PlayerCharacter = Cast<AHonorProjectCharacter>(TryGetPawnOwner());
	if (IsValid(PlayerCharacter))
		m_PlayerCharacter = PlayerCharacter;
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(m_PlayerCharacter))
	{	
		UCharacterMovementComponent* Movement = m_PlayerCharacter->GetCharacterMovement();
		if (Movement)
		{
			m_IsinAir = Movement->IsFalling();

			// 플레이어의 Velocity와 Forward, Right 벡터를 내적하여
			// Forward와 Right의 Speed를 계산한다.
			const FVector CharacterVelocity = m_PlayerCharacter->GetVelocity();
			const FVector CharacterForward = m_PlayerCharacter->GetActorForwardVector();
			const FVector CharacterRight = m_PlayerCharacter->GetActorRightVector();

			const float DotForward = UKismetMathLibrary::Dot_VectorVector(CharacterVelocity, CharacterForward);
			const float DotRight = UKismetMathLibrary::Dot_VectorVector(CharacterVelocity, CharacterRight);
			
			m_SpeedForward = DotForward;
			m_SpeedRight = DotRight;
		}

		m_IsCombatMode = m_PlayerCharacter->IsCombatMode();
	}
}

void UCharacterAnimInstance::AnimNotify_Weapon_Equip()
{
	if (IsValid(m_PlayerCharacter))
	{
		const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
		m_PlayerCharacter->GetWeaponMeshComponent()->AttachToComponent(m_PlayerCharacter->GetMesh(), rules, TEXT("S_Equip"));
	}
}

void UCharacterAnimInstance::AnimNotify_Weapon_Unequip()
{
	if (IsValid(m_PlayerCharacter))
	{
		const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
		m_PlayerCharacter->GetWeaponMeshComponent()->AttachToComponent(m_PlayerCharacter->GetMesh(), rules, TEXT("S_Unequip"));
	}
}

void UCharacterAnimInstance::AnimNotify_OnCollision_Start()
{
	if (IsValid(m_PlayerCharacter))
	{
		m_PlayerCharacter->SetAttackTraceTimer(true);
	}
}

void UCharacterAnimInstance::AnimNotify_OnCollision_End()
{
	if (IsValid(m_PlayerCharacter))
	{
		m_PlayerCharacter->SetAttackTraceTimer(false);
	}
}
