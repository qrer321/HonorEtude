#include "ThreadHandlerLoginResultMessage.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ThreadHandlerLoginResultMessage::ThreadHandlerLoginResultMessage(TSharedPtr<LoginResultMessage> ResultMessage)
	: m_World(nullptr)
	, m_GameInstance(nullptr)
	, m_LoginResultMessage(MoveTemp(ResultMessage))
{
}

ThreadHandlerLoginResultMessage::ThreadHandlerLoginResultMessage(ThreadHandlerLoginResultMessage&& Other) noexcept
	: m_World(Other.m_World)
	, m_GameInstance(Other.m_GameInstance)
	, m_LoginResultMessage(MoveTemp(Other.m_LoginResultMessage))
{
	Other.m_World = nullptr;
	Other.m_GameInstance = nullptr;
}

void ThreadHandlerLoginResultMessage::Initialize(UHonorProjectGameInstance* GameInstance, UWorld* World)
{
	m_GameInstance = GameInstance;
	m_World = World;
}

void ThreadHandlerLoginResultMessage::Start()
{
	if (nullptr == m_World)
	{
		return;
	}

	if (nullptr == m_GameInstance)
	{
		return;
	}
	
	if (EGameServerCode::OK == m_LoginResultMessage->m_Code)
	{
		UGameplayStatics::OpenLevel(m_World, TEXT("PlayTestLevel"));

		FInputModeGameOnly InputMode;
		m_World->GetFirstPlayerController()->SetInputMode(InputMode);
		
		return;
	}
}
