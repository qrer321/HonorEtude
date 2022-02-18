// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "CharacterAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class HONORPROJECT_API UPlayerAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AnimNotify_Weapon_Equip();

	UFUNCTION()
	void AnimNotify_Weapon_Unequip();

	UFUNCTION()
	void AnimNotify_OnCollision_Start();

	UFUNCTION()
	void AnimNotify_OnCollision_End();
};
