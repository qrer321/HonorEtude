// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject/Global/GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "HonorProject/Message/ContentsStructure.h"
#include "CharacterListObject.generated.h"

UCLASS()
class HONORPROJECT_API UCharacterListObject : public UObject
{
	GENERATED_BODY()

private:
	FCharacterInfo	m_CharacterInfo;
	FString			m_ConvertNickname;

public:
	const FCharacterInfo& GetCharacterInfo() const { return m_CharacterInfo; }
	const FString& GetNickname() const { return m_ConvertNickname; }

	void SetCharacterInfo(const FCharacterInfo& Info) { m_CharacterInfo = Info; }
	void SetNickname(const FString& Nickname) { m_ConvertNickname = Nickname; } 
};
