// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Global/GameInfo.h"
#include "GameFramework/Character.h"
#include "HonorProjectCharacter.generated.h"

UCLASS()
class HONORPROJECT_API AHonorProjectCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterInfo, meta = (AllowPrivateAccess = "true"), Replicated)
	FCharacterInfo m_CharacterInfo;

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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


};
