// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "LoginUI.generated.h"

UCLASS()
class HONORPROJECT_API ULoginUI : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = ConnectInfo)
	FString m_IPString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = ConnectInfo)
	FString m_PortString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = ConnectInfo)
	FString m_ConnectStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LoginInfo)
    FString m_IDString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LoginInfo)
	FString m_PWString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LoginInfo)
	FString m_LoginStatus;

public:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION(BlueprintCallable, Category="Login")
	void ResetInfo();

	UFUNCTION(BlueprintCallable, Category="Login")
	void ServerConnect();

	UFUNCTION(BlueprintCallable, Category = "Login")
	void ServerJoin();
	
	UFUNCTION(BlueprintCallable, Category="Login")
	void ServerLogin();

	UFUNCTION(BlueprintCallable, Category="GameMode")
	void SetClientMode(bool Mode);
};
