// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherPlayerCharacter.h"

AOtherPlayerCharacter::AOtherPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOtherPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AOtherPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	while (false == GetObjectMessage()->IsEmpty())
	{
		std::shared_ptr<GameServerMessage> ServerMessage = GetObjectMessage()->Dequeue();
		if (MessageType::PlayerUpdate == ServerMessage->GetType<MessageType>())
		{
			std::shared_ptr<PlayerUpdateMessage> UpdateMessage = std::static_pointer_cast<PlayerUpdateMessage>(ServerMessage);
			if (nullptr == UpdateMessage)
			{
				continue;
			}

			SetActorLocation(UpdateMessage->m_Datum.m_Pos);
		}
		else if (MessageType::ObjectDestroy == ServerMessage->GetType<MessageType>())
		{
			Destroy();
		}
	}
}