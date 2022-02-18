// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "HonorProjectCharacter.h"
#include "PlayerCharacter.h"

void UPlayerAnimInstance::AnimNotify_Weapon_Equip()
{
	if (IsValid(m_Character))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(m_Character);

		if (IsValid(PlayerCharacter))
		{
			const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
			PlayerCharacter->GetWeaponMeshComponent()->AttachToComponent(PlayerCharacter->GetMesh(), rules, TEXT("S_Equip"));			
		}
	}
}

void UPlayerAnimInstance::AnimNotify_Weapon_Unequip()
{
	if (IsValid(m_Character))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(m_Character);

		if (IsValid(PlayerCharacter))
		{
			const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
			PlayerCharacter->GetWeaponMeshComponent()->AttachToComponent(PlayerCharacter->GetMesh(), rules, TEXT("S_Unequip"));
		}
	}
}

void UPlayerAnimInstance::AnimNotify_OnCollision_Start()
{
	if (IsValid(m_Character))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(m_Character);

		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->SetAttackTraceTimer(true);
		}
	}
}

void UPlayerAnimInstance::AnimNotify_OnCollision_End()
{
	if (IsValid(m_Character))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(m_Character);

		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->SetAttackTraceTimer(false);
		}
	}
}