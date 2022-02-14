// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "ChatWindow.generated.h"

UCLASS()
class HONORPROJECT_API UChatWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Chat)
	FString m_ChatString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Chat)
	UListView* m_MessageListView;

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = Chat)
	void SendChat(FString Text, ETextCommit::Type CommitType);

	UFUNCTION(BlueprintCallable, Category = Chat)
	void AddNewMessage(UObject* Object, UUserWidget* UserWidget);

	void AddMessage(class UChatMessageObject* ChatMessageObject);
	void AddMessage(const FString& ID, const FString& Message);
};
