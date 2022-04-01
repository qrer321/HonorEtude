// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "CharacterListWidget.generated.h"

UCLASS()
class HONORPROJECT_API UCharacterListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharcterInfo")
	FString m_Nickname;

	UFUNCTION(BlueprintCallable, Category = "Login|Connect")
	void CharacterSelect();
};
