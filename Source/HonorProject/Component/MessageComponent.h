// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HonorProject//Global/GameInfo.h"
#include "Components/ActorComponent.h"
#include "HonorProject/Message/Dispatcher.h"
#include "MessageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HONORPROJECT_API UMessageComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	Dispatcher m_Dispatcher;

public:	
	// Sets default values for this component's properties
	UMessageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
