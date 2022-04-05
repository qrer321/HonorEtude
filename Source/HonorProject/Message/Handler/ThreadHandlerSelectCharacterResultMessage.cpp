#include "ThreadHandlerSelectCharacterResultMessage.h"
#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"

void ThreadHandlerSelectCharacterResultMessage::Start()
{
	FString Nickname;
	UClientBlueprintFunctionLibrary::UTF8ToFString(m_Message->m_Nickname, Nickname);

	if (EGameServerCode::OK == m_Message->m_Code)
	{
		for (const FCharacterInfo& CharacterInfo : m_GameInstance->m_CharactersInfo)
		{
			if (CharacterInfo.m_Nickname == m_Message->m_Nickname)
			{
				m_GameInstance->m_SelectCharacterInfo = CharacterInfo;
				return;
			}
		}

		LOG(TEXT("Select Character Data Is Not Valid"));
		m_GameInstance->m_LoginProcess = false;
		UGameplayStatics::OpenLevel(m_World, TEXT("LoginLevel"));
	}
}