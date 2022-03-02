// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "HonorProjectCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS(config=Game)
class APlayerCharacter : public AHonorProjectCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_SMSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class AMasterAICharacter* m_ClosestEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float m_ClosestEnemyDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TArray<AMasterAICharacter*> m_AlreadyDamagedEnemyArray;

	

	FTimerHandle m_DetectAttackDirectionTimer;
	FTimerHandle m_AttackTraceTimer;
	FTimerHandle m_ControllerYawTimer;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UFUNCTION(BlueprintCallable, Client, Reliable)
	virtual void Client_FindClosestEnemy() override;

	UFUNCTION(BlueprintCallable, Client, Reliable)
	virtual void Client_ReticleVisibility() override;

public:
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetWeaponMeshComponent() const { return m_SMSword; }

public:
	void RotateToTarget();
	void CombatCameraSwitch();
	
	void SetDetectAttackDirectionTimer();
	void SetAttackTraceTimer(bool SetTimer);
	virtual void SetControllerYawTimer(float MontageLength) override;
	virtual void SetCombatOffDelayTimer(float MontageLength) override;
	
	void DetectAttackDirection();

	UFUNCTION(Client, Reliable)
	void AttackTrace();
	void ResetControllerYaw();
	void CombatOffDelay();

	void SetWeaponSocketLocation_Implementation(FName SocketName) override;

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

	// 서버 테스트용 함수
public:
	void TestPacketUpdate0();
	void TestPacketUpdate1();
	void TestPacketUpdate2();

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return m_FollowCamera; }
};