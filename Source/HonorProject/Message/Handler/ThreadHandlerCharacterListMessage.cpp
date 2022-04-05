#include "ThreadHandlerCharacterListMessage.h"
#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"

void ThreadHandlerCharacterListMessage::Start()
{
	m_GameInstance->m_CharactersInfo = m_Message->m_CharactersInfo;
	for (const FCharacterInfo& CharacterInfo : m_GameInstance->m_CharactersInfo)
	{
		FString Nickname;
		UClientBlueprintFunctionLibrary::UTF8ToFString(CharacterInfo.m_Nickname, Nickname);
	}

	// 서버의 DB를 통해 유저의 캐릭터들 정보를 얻어오고
	// 그 이후 캐릭터 선택 레벨로 넘어간다.
	UGameplayStatics::OpenLevel(m_World, TEXT("CharacterSelectLevel"));
}