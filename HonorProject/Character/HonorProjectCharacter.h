// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "CharacterController.h"
#include "CharacterAnimInstance.h"
#include "HonorProjectCharacter.generated.h"

UCLASS(config=Game)
class AHonorProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_SMSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller, meta = (AllowPrivateAccess = "true"))
	ACharacterController* m_CharacterController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UCharacterAnimInstance* m_AnimInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_EquipAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  Combat, meta = (AllowPrivateAccess = "true"), Replicated)
	class AMasterAICharacter* m_ClosestEnemy;
	
public:
	AHonorProjectCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  Combat, meta = (AllowPrivateAccess = "true"), Replicated)
	bool m_IsCombatMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  Combat, meta = (AllowPrivateAccess = "true"), Replicated)
	float m_ClosestEnemyDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  Combat, meta = (AllowPrivateAccess = "true"))
	EAttackDirection m_AttackDirection;

	FTimerHandle m_ToTargetRotateTimer;
	FTimerHandle m_DetectAttackDirectionTimer;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_IsCombatMode(bool IsCombatMode, bool UseOrientRotation, bool UseControllerDesiredRotation, float MaxWalkSpeed, FName SectionName = NAME_None);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Client_IsCombatMode(bool IsCombatMode, bool UseOrientRotation, bool UseControllerDesiredRotation, float MaxWalkSpeed, FName SectionName = NAME_None);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_PlayMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSocketName = NAME_None);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Client_PlayMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSocketName = NAME_None);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_FindClosestEnemy();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_ReticleVisibility();

public:
	bool IsCombatMode() const { return m_IsCombatMode; }

	UStaticMeshComponent* GetWeaponMeshComponent() const { return m_SMSword; }

public:
	void SetTargetRotateTimer();
	void SetDetectAttackDirectionTimer();
	
	void RotateToTarget() const;
	void DetectAttackDirection();
	

protected:
	void PressedLockOn();
	void ReleasedLockOn();

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};