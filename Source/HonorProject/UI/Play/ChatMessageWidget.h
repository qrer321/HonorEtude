// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageWidget.generated.h"

UCLASS()
class HONORPROJECT_API UChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Chat)
	FString m_ChatString;

public:
	void SetChatString(const FString& Chat) { m_ChatString = Chat; }
};
