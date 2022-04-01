// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "CharacterSelectUI.generated.h"

UCLASS()
class HONORPROJECT_API UCharacterSelectUI : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Info)
	FString m_Nickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = CharacterList)
	UListView* m_CharacterListView;

public:
	void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "Create")
	void CreateCharacter();

	UFUNCTION(BlueprintCallable, Category = "Create")
	void AddNewCharacter(UObject* Object, UUserWidget* Widget);
};
