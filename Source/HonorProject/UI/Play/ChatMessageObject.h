// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "ChatMessageObject.generated.h"

UCLASS()
class HONORPROJECT_API UChatMessageObject : public UObject
{
	GENERATED_BODY()

private:
	FString m_ID;
	FString m_Message;

public:
	const FString& GetID() const { return m_ID; }
	const FString& GetMessage() const { return m_Message; }

public:
	void SetID(const FString& ID) { m_ID = ID; }
	void SetMessage(const FString& Message) { m_Message = Message; } 
};
