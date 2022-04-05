// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterListWidget.h"

#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "HonorProject/Message/ClientToServer.h"
#include "HonorProject/Message/ServerToClient.h"

void UCharacterListWidget::CharacterSelect()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (true == GameInstance->GetClientMode())
	{
		std::shared_ptr<SelectCharacterResultMessage> ResultMessage = std::make_shared<SelectCharacterResultMessage>();
		ResultMessage->m_Code = EGameServerCode::OK;
		GameInstance->PushClientMessage(ResultMessage);
		return;
	}

	std::string ConvertNickname;
	UClientBlueprintFunctionLibrary::FStringToUTF8(m_Nickname, ConvertNickname);

	SelectCharacterMessage SelectPacket;
	SelectPacket.m_Nickname = ConvertNickname;

	GameServerSerializer Serializer;
	SelectPacket.Serialize(Serializer);

	GameInstance->Send(Serializer.GetData());
}
