// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../GameInfo.h"
#include "AllCharacter.h"
#include "GameFramework/Character.h"
#include "CharacterController.h"
#include "CharacterAnimInstance.h"
#include "HonorProjectCharacter.generated.h"

UCLASS(config=Game)
class AHonorProjectCharacter : public AAllCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_SMSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller, meta = (AllowPrivateAccess = "true"))
	ACharacterController* m_CharacterController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UCharacterAnimInstance* m_AnimInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_EquipAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_AttackUpMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_AttackLeftMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_AttackRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"), Replicated)
	class AMasterAICharacter* m_ClosestEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"), Replicated)
	TArray<AMasterAICharacter*> m_AlreadyDamagedEnemyArray;
	
public:
	AHonorProjectCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"), Replicated)
	bool m_IsCombatMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"), Replicated)
	float m_ClosestEnemyDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"), Replicated)
	EAttackDirection m_AttackDirection;

	FTimerHandle m_DetectAttackDirectionTimer;
	FTimerHandle m_ControllerYawTimer;
	FTimerHandle m_AttackTraceTimer;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_IsCombatMode(bool IsCombatMode, bool UseOrientRotation, bool UseControllerDesiredRotation, float MaxWalkSpeed, FName SectionName = NAME_None);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MultiCast_IsCombatMode(bool IsCombatMode, bool UseOrientRotation, bool UseControllerDesiredRotation, float MaxWalkSpeed, FName SectionName = NAME_None);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_PlayMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSocketName = NAME_None);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MultiCast_PlayMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSocketName = NAME_None);

	UFUNCTION(Server, Reliable)
	void Server_SetAttackDirection(EAttackDirection AttackDirection);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_SetAttackDirection(EAttackDirection AttackDirection);

	UFUNCTION(Server, Reliable)
	void Server_Attack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_Attack();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_FindClosestEnemy();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_ReticleVisibility();

public:
	UFUNCTION(BlueprintCallable)
	bool IsCombatMode() const { return m_IsCombatMode; }

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetWeaponMeshComponent() const { return m_SMSword; }

	UFUNCTION(BlueprintCallable)
	EAttackDirection GetAttackDirection() const { return m_AttackDirection; }

public:
	void RotateToTarget();
	void CombatCameraSwitch();
	
	void SetDetectAttackDirectionTimer();
	void DetectAttackDirection();

	void SetControllerYawTimer(float AnimMontageLength);
	void ResetControllerYaw();

	void SetAttackTraceTimer(bool SetTimer);
	void AttackTrace();

protected:
	void PressedLockOn();
	void ReleasedLockOn();

	void PressedAttack();

	void MoveForward(float Value);
	void MoveRight(float Value);

	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return m_FollowCamera; }
};