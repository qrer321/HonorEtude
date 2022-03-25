// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginUI.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"
#include "HonorProject/Message/Messages.h"

void ULoginUI::NativeConstruct()
{
	Super::NativeConstruct();

	m_IPString = TEXT("127.0.0.1");
	m_PortString = TEXT("30001");
	m_IDString = TEXT("LoginTest");
	m_PWString = TEXT("LoginTest");

	m_ConnectStatus = TEXT("Connect Required");
	m_LoginStatus = TEXT("Login Required");
}

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
		m_ConnectStatus = TEXT("Connect Failed");
		return;
	}

	m_ConnectStatus = TEXT("Connect Succeed");
}

void ULoginUI::ServerJoin()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (true == GameInstance->GetClientMode())
	{
		return;
	}

	std::string ID;
	std::string PW;
	UClientBlueprintFunctionLibrary::FStringToUTF8(m_IDString, ID);
	UClientBlueprintFunctionLibrary::FStringToUTF8(m_PWString, PW);

	JoinMessage NewMessage;
	NewMessage.m_ID = ID;
	NewMessage.m_PW = PW;

	GameServerSerializer Serializer;
	NewMessage.Serialize(Serializer);

	GameInstance->Send(Serializer.GetData());
}

void ULoginUI::ServerLogin()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (true == GameInstance->GetClientMode())
	{
		std::shared_ptr<LoginResultMessage> Message = std::make_shared<LoginResultMessage>();
		Message->m_Code = EGameServerCode::OK;

		GameInstance->PushClientMessage(Message);
		return;
	}
	
	std::string ID;
	std::string PW;
	UClientBlueprintFunctionLibrary::FStringToUTF8(m_IDString, ID);
	UClientBlueprintFunctionLibrary::FStringToUTF8(m_PWString, PW);

	LoginMessage NewMessage;
	NewMessage.m_ID = ID;
	NewMessage.m_PW = PW;

	GameServerSerializer Serializer;
	NewMessage.Serialize(Serializer);

	GameInstance->m_UserID = m_IDString;
	GameInstance->Send(Serializer.GetData());
}

void ULoginUI::SetClientMode(bool Mode)
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (false == IsValid(GameInstance))
	{
		return;
	}

	GameInstance->SetClientMode(Mode);
}
