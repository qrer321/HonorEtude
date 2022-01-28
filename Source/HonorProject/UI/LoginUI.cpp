// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginUI.h"
#include <string>
#include "HonorProject/HonorProjectGameInstance.h"

void ULoginUI::ResetInfo()
{
	m_IPString = TEXT("127.0.0.1");
	m_PortString = TEXT("30001");
}

void ULoginUI::ServerConnect()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	
	if (false == GameInstance->ServerConnect(m_IPString, m_PortString))
		return;
}

void ULoginUI::ServerLogin()
{
	FString StringTest = TEXT("한글 데이터 테스트");
	ANSICHAR* AnsiPtr = TCHAR_TO_UTF8(*StringTest);
	std::string SendString = std::string(AnsiPtr); 
	
	TArray<uint8> Data;
	Data.SetNum(SendString.size() + 1);
	Data[SendString.size()] = 0;
	std::copy(SendString.begin(), SendString.end(), Data.GetData());

	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance->Send(Data))
		return;
}
