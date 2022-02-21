// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

UCLASS()
class HONORPROJECT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCharacterAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AHonorProjectCharacter* m_Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_IsinAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_SpeedForward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_SpeedRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_IsCombatMode;
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	float GetCurrentMontageRemainingFriction() const;
};
