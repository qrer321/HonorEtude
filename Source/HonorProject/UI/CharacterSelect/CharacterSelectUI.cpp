// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectUI.h"
#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "HonorProject/Message/ClientToServer.h"
#include "CharacterListObject.h"
#include "CharacterListWidget.h"

void UCharacterSelectUI::NativeConstruct()
{
	Super::NativeConstruct();

	const UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	
	m_CharacterListView = Cast<UListView>(GetWidgetFromName(TEXT("CharacterList")));
	if (false == m_CharacterListView->IsValidLowLevel())
	{
		return;
	}
	
	for (size_t i = 0; i < GameInstance->m_Characters.size(); ++i)
	{
		UCharacterListObject* NewCharacter = NewObject<UCharacterListObject>();
		NewCharacter->SetCharacterInfo(GameInstance->m_Characters[i]);

		FString Nickname;
		UClientBlueprintFunctionLibrary::UTF8ToFString(GameInstance->m_Characters[i].m_Nickname, Nickname);
		NewCharacter->SetNickname(Nickname);

		m_CharacterListView->AddItem(NewCharacter);
		m_CharacterListView->SetScrollOffset(m_CharacterListView->GetNumItems() * 50.f);
	}
}

void UCharacterSelectUI::AddNewCharacter(UObject* Object, UUserWidget* Widget)
{
	UCharacterListObject* CharacterObject = Cast<UCharacterListObject>(Object);
	UCharacterListWidget* CharacterListWidget = Cast<UCharacterListWidget>(Widget);

	if (nullptr == CharacterObject || nullptr == CharacterListWidget)
	{
		return;
	}

	CharacterListWidget->m_Nickname = CharacterObject->GetNickname();
}

void UCharacterSelectUI::CreateCharacter()
{
	if (m_Nickname.IsEmpty())
	{
		return;
	}
	
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (true == GameInstance->GetClientMode())
	{
		return;
	}

	std::string Nickname;
	UClientBlueprintFunctionLibrary::FStringToUTF8(m_Nickname, Nickname);

	CreateCharacterMessage NewMessage;
	NewMessage.m_Nickname = Nickname;

	GameServerSerializer Serializer;
	NewMessage.Serialize(Serializer);

	GameInstance->m_Nickname = m_Nickname;
	GameInstance->Send(Serializer.GetData());
}
