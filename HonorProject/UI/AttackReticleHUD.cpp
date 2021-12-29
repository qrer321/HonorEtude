// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackReticleHUD.h"

void UAttackReticleHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_ReticleUp = Cast<UImage>(GetWidgetFromName(TEXT("Reticle_Up")));
	m_ReticleLeft = Cast<UImage>(GetWidgetFromName(TEXT("Reticle_Left")));
	m_ReticleRight = Cast<UImage>(GetWidgetFromName(TEXT("Reticle_Right")));
}

void UAttackReticleHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
