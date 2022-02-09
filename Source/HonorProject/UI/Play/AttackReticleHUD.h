// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "AttackReticleHUD.generated.h"

UCLASS()
class HONORPROJECT_API UAttackReticleHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Reticle)
	UImage* m_ReticleUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Reticle)
	UImage* m_ReticleLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Reticle)
	UImage* m_ReticleRight;

public:
	UImage* GetReticleUp() const { return m_ReticleUp; }
	UImage* GetReticleLeft() const { return m_ReticleLeft; }
	UImage* GetReticleRight() const { return m_ReticleRight; }
};
