#include "ThreadHandlerCreateCharacterResultMessage.h"
#include "HonorProject/Message/ContentsStructure.h"
#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"
#include "HonorProject/UI/CharacterSelect/CharacterListWidget.h"
#include "HonorProject/UI/CharacterSelect/CharacterListObject.h"

void ThreadHandlerCreateCharacterResultMessage::Start()
{
	if (EGameServerCode::OK == m_Message->m_Code)
	{
		m_GameInstance->m_CharactersInfo.push_back(m_Message->m_CharacterInfo);

		UCharacterListObject* NewCharacterObject = NewObject<UCharacterListObject>();
		NewCharacterObject->SetCharacterInfo(m_Message->m_CharacterInfo);

		FString Nickname;
		UClientBlueprintFunctionLibrary::UTF8ToFString(NewCharacterObject->GetCharacterInfo().m_Nickname, Nickname);
		NewCharacterObject->SetNickname(Nickname);

		m_GameInstance->m_CharacterListView->AddItem(NewCharacterObject);
		m_GameInstance->m_CharacterListView->SetScrollOffset(m_GameInstance->m_CharacterListView->GetNumItems() * 50.f);
	}
}