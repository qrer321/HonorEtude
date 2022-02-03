// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginUI.h"
#include "HonorProject/HonorProjectGameInstance.h"
#include "../Global/ClientBlueprintFunctionLibrary.h"
#include "../Global/GameServerPacket.h"

void ULoginUI::ResetInfo()
{
	m_IPString = TEXT("127.0.0.1");
	m_PortString = TEXT("30001");
}

void ULoginUI::ServerConnect()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	
	if (false == GameInstance->ServerConnect(m_IPString, m_PortString))
	{
		m_ConnectStatus = TEXT("접속 실패");
		return;
	}

	m_ConnectStatus = TEXT("접속 성공");
}

void ULoginUI::ServerLogin()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());

	std::string ID;
	std::string PW;
	UClientBlueprintFunctionLibrary::FStringToUTF8(m_IDString, ID);
	UClientBlueprintFunctionLibrary::FStringToUTF8(m_PWString, PW);

	LoginPacket NewPacket;
	NewPacket.m_ID = ID;
	NewPacket.m_PW = PW;

	GameServerSerializer Serializer;
	NewPacket.Serialize(Serializer);

	GameInstance->Send(Serializer.GetData());
}
