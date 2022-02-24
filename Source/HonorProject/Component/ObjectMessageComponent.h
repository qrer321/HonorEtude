// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include "HonorProject/Global/GameInfo.h"
#include "Components/ActorComponent.h"
#include "HonorProject/Message/Messages.h"
#include "ObjectMessageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HONORPROJECT_API UObjectMessageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectMessageComponent();

private:
	TQueue<std::shared_ptr<GameServerMessage>> m_MessageQueue;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	bool IsEmpty() { return m_MessageQueue.IsEmpty(); }

	void EnQueue(std::shared_ptr<GameServerMessage> Message);
	std::shared_ptr<GameServerMessage> Dequeue();
};
