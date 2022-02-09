// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_AttackReticleHUD = Cast<UAttackReticleHUD>(GetWidgetFromName(TEXT("WB_AttackReticleHUD")));
}

void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainHUD::SetAttackReticleVisibility(ESlateVisibility HUDVisibility) const
{
	if (IsValid(m_AttackReticleHUD))
		m_AttackReticleHUD->SetVisibility(HUDVisibility);
}

void UMainHUD::SetAttackReticleOpacity(EAttackDirection AttackDirection) const
{
	if (!IsValid(m_AttackReticleHUD))
		return;

	FLinearColor color = { 1.f, 1.f, 1.f, 0.3f };
	m_AttackReticleHUD->GetReticleUp()->SetColorAndOpacity(color);
	m_AttackReticleHUD->GetReticleRight()->SetColorAndOpacity(color);
	m_AttackReticleHUD->GetReticleLeft()->SetColorAndOpacity(color);

	color.A = 1.f;
	if (AttackDirection == EAttackDirection::Up)
	{
		m_AttackReticleHUD->GetReticleUp()->SetColorAndOpacity(color);
	}
	else if (AttackDirection == EAttackDirection::Right)
	{
		m_AttackReticleHUD->GetReticleRight()->SetColorAndOpacity(color);
	}
	else if (AttackDirection == EAttackDirection::Left)
	{
		m_AttackReticleHUD->GetReticleLeft()->SetColorAndOpacity(color);
	}
}
