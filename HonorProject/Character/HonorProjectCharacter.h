// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "HonorProjectCharacter.generated.h"

UCLASS(config=Game)
class AHonorProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
public:
	AHonorProjectCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool m_IsCombatMode;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_IsCombatMode(bool IsCombatMode, bool UseOrientRotation, bool UseControllerDesiredRotation, float MaxWalkSpeed);
	virtual void Server_IsCombatMode_Implementation(bool IsCombatMode, bool UseOrientRotation, bool UseControllerDesiredRotation, float MaxWalkSpeed);

public:
	bool IsCombatMode() const { return m_IsCombatMode; }

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