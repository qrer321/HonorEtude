// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "LoginUI.generated.h"

UCLASS()
class HONORPROJECT_API ULoginUI : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LoginUI)
	FString m_IPString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LoginUI)
	FString m_PortString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LoginUI)
    FString m_IDString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LoginUI)
	FString m_PWString;

public:
	UFUNCTION(BlueprintCallable, Category="Login")
	void ResetInfo();

	UFUNCTION(BlueprintCallable, Category="Login")
	void ServerConnect();
	
	UFUNCTION(BlueprintCallable, Category="Login")
	void ServerLogin();
};
