// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Global/GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "HonorProject/UI/Play/MainHUD.h"
#include "CharacterController.generated.h"

UCLASS()
class HONORPROJECT_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterController();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainHUD> m_MainHUDClass;

	UPROPERTY(Replicated)
	UMainHUD* m_MainHUD;

public:
	UMainHUD* GetMainHUD() const { return m_MainHUD; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_AddMainHUDToViewport();
	UFUNCTION(Client, Reliable)
	void Client_AddMainHUDToViewport();

public:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
};
