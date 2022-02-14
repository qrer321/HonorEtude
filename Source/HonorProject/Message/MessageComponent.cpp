// Fill out your copyright notice in the Description page of Project Settings.

#include "MessageComponent.h"
#include "Handler/HandlerHeader.h"
#include "../Global/HonorProjectGameInstance.h"

UMessageComponent::UMessageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMessageComponent::BeginPlay()
{
	Super::BeginPlay();

	AddGlobalHandler(m_Dispatcher, GetWorld());
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

