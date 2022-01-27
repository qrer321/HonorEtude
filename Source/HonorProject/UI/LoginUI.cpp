// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginUI.h"

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
}
