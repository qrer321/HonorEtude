// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "HealthHUD.generated.h"

UCLASS()
class HONORPROJECT_API UHealthHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar* m_HealthFront;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar* m_HealthBack;

	UPROPERTY()
	class AHonorProjectCharacter* m_WidgetOwner;

public:
	void SetHPPercent(const float Percent) const { m_HealthFront->SetPercent(Percent); }
	void SetWidgetOwner(AHonorProjectCharacter* WidgetOwner) { m_WidgetOwner = WidgetOwner; }
	void SetHealthVisibility(bool Visible);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};