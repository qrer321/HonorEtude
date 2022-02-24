// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectMessageComponent.h"

// Sets default values for this component's properties
UObjectMessageComponent::UObjectMessageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UObjectMessageComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UObjectMessageComponent::EnQueue(std::shared_ptr<GameServerMessage> Message)
{
	m_MessageQueue.Enqueue(Message);
}

std::shared_ptr<GameServerMessage> UObjectMessageComponent::Dequeue()
{
	std::shared_ptr<GameServerMessage> Message;
	m_MessageQueue.Dequeue(Message);

	return Message;
}
