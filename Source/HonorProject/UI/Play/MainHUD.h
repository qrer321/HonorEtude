// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "AttackReticleHUD.h"
#include "MainHUD.generated.h"

UCLASS()
class HONORPROJECT_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = UI)
	UAttackReticleHUD* m_AttackReticleHUD;

public:
	UAttackReticleHUD* GetAttackReticleHUD() const { return m_AttackReticleHUD; }
	
	void SetAttackReticleVisibility(ESlateVisibility HUDVisibility) const;
	void SetAttackReticleOpacity(EAttackDirection AttackDirection) const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
