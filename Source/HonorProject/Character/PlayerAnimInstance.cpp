// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "HonorProjectCharacter.h"
#include "PlayerCharacter.h"

void UPlayerAnimInstance::AnimNotify_Weapon_Equip()
{
	if (false == IsValid(m_Character))
	{
		LOGSTRING(TEXT("Character Is Not Valid"));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(m_Character);
	if (false == IsValid(PlayerCharacter))
	{
		LOGSTRING(TEXT("Cast PlayerCharacter Is Not Valid"))
		return;
	}

	if (false == PlayerCharacter->IsLocallyControlled())
	{
		// Notify 함수를 호출한 캐릭터가
		// 해당 클라이언트의 컨트롤이 아닌 경우 return
		return;
	}

	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	PlayerCharacter->GetWeaponMeshComponent()->AttachToComponent(PlayerCharacter->GetMesh(), rules, TEXT("S_Equip"));
}

void UPlayerAnimInstance::AnimNotify_Weapon_Unequip()
{
	if (false == IsValid(m_Character))
	{
		LOGSTRING(TEXT("Character Is Not Valid"));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(m_Character);
	if (false == IsValid(PlayerCharacter))
	{
		LOGSTRING(TEXT("Cast PlayerCharacter Is Not Valid"))
		return;
	}

	if (false == PlayerCharacter->IsLocallyControlled())
	{
		// Notify 함수를 호출한 캐릭터가
		// 해당 클라이언트의 컨트롤이 아닌 경우 return
		return;
	}

	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	PlayerCharacter->GetWeaponMeshComponent()->AttachToComponent(PlayerCharacter->GetMesh(), rules, TEXT("S_Unequip"));
}

void UPlayerAnimInstance::AnimNotify_OnCollision_Start()
{
	if (false == IsValid(m_Character))
	{
		LOGSTRING(TEXT("Character Is Not Valid"));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(m_Character);
	if (false == IsValid(PlayerCharacter))
	{
		LOGSTRING(TEXT("Cast PlayerCharacter Is Not Valid"))
		return;
	}
	
	if (false == PlayerCharacter->IsLocallyControlled())
	{
		// Notify 함수를 호출한 캐릭터가
		// 해당 클라이언트의 컨트롤이 아닌 경우 return
		return;
	}
	
	PlayerCharacter->SetAttackTraceTimer(true);
}

void UPlayerAnimInstance::AnimNotify_OnCollision_End()
{
	if (false == IsValid(m_Character))
	{
		LOGSTRING(TEXT("Character Is Not Valid"));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(m_Character);
	if (false == IsValid(PlayerCharacter))
	{
		LOGSTRING(TEXT("Cast PlayerCharacter Is Not Valid"))
		return;
	}
	
	if (false == PlayerCharacter->IsLocallyControlled())
	{
		// Notify 함수를 호출한 캐릭터가
		// 해당 클라이언트의 컨트롤이 아닌 경우 return
		return;
	}
	
	PlayerCharacter->SetAttackTraceTimer(false);
}