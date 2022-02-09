// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Global/GameInfo.h"
#include "HonorProjectCharacter.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "MasterAICharacter.generated.h"

UCLASS()
class HONORPROJECT_API AMasterAICharacter : public AHonorProjectCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMasterAICharacter();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* m_TargetDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_HealthHUD;

	UPROPERTY()
	class UHealthHUD* m_HealthWidget;

public:
	UDecalComponent* GetTargetDecal() const { return m_TargetDecal; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_CalcDamage(float DamageAmount);
};
