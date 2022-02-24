// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

ACharacterController::ACharacterController()
{
	static ConstructorHelpers::FClassFinder<UMainHUD> MainHUDClass(TEXT("WidgetBlueprint'/Game/HonorProejct/PlayRelevant/UI/WB_MainHUD.WB_MainHUD_C'"));
	if (MainHUDClass.Succeeded())
		m_MainHUDClass = MainHUDClass.Class;
}

void ACharacterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterController, m_MainHUD);
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();

	Server_AddMainHUDToViewport();

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(true);
	
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}

void ACharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ACharacterController::Server_AddMainHUDToViewport_Implementation()
{
	Client_AddMainHUDToViewport();
}

void ACharacterController::Client_AddMainHUDToViewport_Implementation()
{
	if (IsValid(m_MainHUDClass))
	{
		m_MainHUD = Cast<UMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
		if (IsValid(m_MainHUD))
			m_MainHUD->AddToViewport();
	}
}