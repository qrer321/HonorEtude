// Fill out your copyright notice in the Description page of Project Settings.


#include "HonorProjectGameInstance.h"

UHonorProjectGameInstance::UHonorProjectGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterTableAsset(TEXT("DataTable'/Game/HonorProejct/Character/Data/DTCharacterInfo.DTCharacterInfo'"));
	if (CharacterTableAsset.Succeeded())
		m_CharacterInfoTable = CharacterTableAsset.Object;
}

UHonorProjectGameInstance::~UHonorProjectGameInstance()
{
}

void UHonorProjectGameInstance::Init()
{
	Super::Init();
}

const FCharacterTableInfo* UHonorProjectGameInstance::FindCharacterInfo(const FString& Name) const
{
	return m_CharacterInfoTable->FindRow<FCharacterTableInfo>(*Name, "");
}
