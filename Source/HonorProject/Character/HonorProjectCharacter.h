// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "GameFramework/Character.h"
#include "CharacterController.h"
#include "CharacterAnimInstance.h"
#include "HonorProjectCharacter.generated.h"

UCLASS()
class HONORPROJECT_API AHonorProjectCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterInfo, meta = (AllowPrivateAccess = "true"), Replicated)
	FCharacterInfo m_CharacterInfo;

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
	bool m_IsCombatMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"), Replicated)
	EAttackDirection m_AttackDirection;

	
	FTimerHandle m_CombatOffDelayTimer;
	FTimerHandle m_WeaponCheckTimer;

public:
	FCharacterInfo GetCharacterInfo() const { return m_CharacterInfo; }

public:
	// Sets default values for this character's properties
	AHonorProjectCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	bool IsCombatMode() const { return m_IsCombatMode; }
	
	UFUNCTION(BlueprintCallable)
	EAttackDirection GetAttackDirection() const { return m_AttackDirection; }

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_IsCombatMode(bool IsCombatMode, bool UseOrientRotation, bool UseControllerDesiredRotation, float MaxWalkSpeed, FName StartSectionName = NAME_None);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_IsCombatMode(bool IsCombatMode, bool UseOrientRotation, bool UseControllerDesiredRotation, float MaxWalkSpeed);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_PlayMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_PlayMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetAttackDirection(EAttackDirection AttackDirection);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_SetAttackDirection(EAttackDirection AttackDirection);
	UFUNCTION(Client, Reliable)
	void Client_SetAttackReticle();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_Attack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_Attack();

public:
	virtual void Client_FindClosestEnemy() {}
	virtual void Client_ReticleVisibility() {}
	virtual void SetControllerYawTimer(float MontageLength) {}
	virtual void SetCombatOffDelayTimer(float MontageLength) {}

	void SetWeaponCheckTimer();
	void WeaponCheck();
	virtual void SetWeaponSocketLocation() {}

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


};
