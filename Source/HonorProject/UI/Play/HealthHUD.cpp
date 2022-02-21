// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthHUD.h"
#include "HonorProject/Character/HonorProjectCharacter.h"

void UHealthHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_HealthFront = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthFront")));
	m_HealthBack = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBack")));
}

void UHealthHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (false == IsValid(m_WidgetOwner))
	{
		return;
	}
	
	const float Percent = UKismetMathLibrary::FInterpTo(m_HealthBack->Percent, m_HealthFront->Percent, InDeltaTime, 1.f);
	m_HealthBack->SetPercent(Percent);
}

void UHealthHUD::SetHealthVisibility(bool Visible)
{
	if (true == Visible)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (false == Visible)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}