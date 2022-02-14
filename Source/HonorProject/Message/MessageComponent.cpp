// Fill out your copyright notice in the Description page of Project Settings.

#include "MessageComponent.h"
#include "Handler/HandlerHeader.h"
#include "../Global/HonorProjectGameInstance.h"

template <typename MessageHandler, typename MessageType>
void OnMessageProcess(std::shared_ptr<GameServerMessage> Message, UHonorProjectGameInstance* GameInstance, UWorld* World)
{
	std::shared_ptr<MessageType> ConvertMessage = std::static_pointer_cast<MessageType>(MoveTemp(Message));
	if (nullptr == ConvertMessage)
	{
		return;
	}

	MessageHandler Handler = MessageHandler(MoveTempIfPossible(ConvertMessage));
	Handler.Initialize(GameInstance, World);
	Handler.Start();
}

UMessageComponent::UMessageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMessageComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Dispatcher.AddHandler(MessageType::LoginResult,
	                        [this](std::shared_ptr<GameServerMessage> GameServerMessage)
	                        {
		                        UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
		                        OnMessageProcess<ThreadHandlerLoginResultMessage, LoginResultMessage>(MoveTemp(GameServerMessage), GameInstance, GetWorld());
	                        });

	m_Dispatcher.AddHandler(MessageType::Chat,
							[this](std::shared_ptr<GameServerMessage> GameServerMessage)
							{
								UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
								OnMessageProcess<ThreadHandlerChatMessage, ChatMessage>(MoveTemp(GameServerMessage), GameInstance, GetWorld());
							});
}

void UMessageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());

	while (false == GameInstance->GetMessageQueue().IsEmpty())
	{
		std::shared_ptr<GameServerMessage> ServerMessage;
		GameInstance->GetMessageQueue().Dequeue(ServerMessage);
		if (nullptr == ServerMessage)
		{
			continue;
		}

		MessageHandler Handler;
		m_Dispatcher.GetHandler(ServerMessage->GetType(), Handler);
		if (nullptr == Handler)
		{
			return;
		}

		Handler(MoveTemp(ServerMessage));
	}
}

