// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Global/HonorProjectGameInstance.h"
#include "MasterAICharacter.h"
#include "HonorProject/GameMode/PlayGameMode.h"

APlayerCharacter::APlayerCharacter()
{
	bReplicates = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraBoom->SetupAttachment(RootComponent);
	m_CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	m_CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	m_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_FollowCamera->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	m_FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	m_SMSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Sword"));
	m_SMSword->SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMSwordAsset(TEXT("StaticMesh'/Game/GKnight/Meshes/Weapon/SM_WP_GothicKnight_Sword.SM_WP_GothicKnight_Sword'"));
	if (SMSwordAsset.Succeeded())
		m_SMSword->SetStaticMesh(SMSwordAsset.Object);



	// For Server Code
	m_UDPReady = false;
	m_ServerPostEnable = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_CharacterController = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(m_CharacterController))
	{
		if (IsValid(m_CharacterController->GetPawn()))
		{
			m_CharacterController->GetPawn()->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		}
	}
	
	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	m_SMSword->AttachToComponent(GetMesh(), rules, TEXT("S_Unequip"));

	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (false == IsValid(PlayGameMode))
	{
		return;
	}
	
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (false == IsValid(GameInstance))
	{
		return;
	}
	
	const FCharacterTableInfo* CharacterInfo = GameInstance->FindCharacterInfo(TEXT("Player"));
	if (CharacterInfo)
	{
		m_CharacterInfo.Name = CharacterInfo->Name;
		m_CharacterInfo.Attack = CharacterInfo->Attack;
		m_CharacterInfo.Armor = CharacterInfo->Armor;
		m_CharacterInfo.HP = CharacterInfo->HP;
		m_CharacterInfo.HPMax = CharacterInfo->HPMax;
		m_CharacterInfo.SP = CharacterInfo->SP;
		m_CharacterInfo.SPMax = CharacterInfo->SPMax;
		m_CharacterInfo.SPRecoverMaxTime = CharacterInfo->SPRecoverMaxTime;
		m_CharacterInfo.AttackSpeed = CharacterInfo->AttackSpeed;
		m_CharacterInfo.MoveSpeed = CharacterInfo->MoveSpeed;
	}

	SetObjectType(EGameObjectType::Player);
	SetActorIndex(PlayGameMode->GetUniqueID());
	PlayGameMode->RegistObject(GameInstance->m_ActorIndex, EGameObjectType::Player, this);

	ClientToReadyMessage ReadyMessage;
	GameServerSerializer Serializer;
	ReadyMessage.m_ActorIndex = GameInstance->m_ActorIndex;
	ReadyMessage.m_ThreadIndex = GameInstance->m_ThreadIndex;
	ReadyMessage.m_SectionIndex = GameInstance->m_SectionIndex;

	ReadyMessage.Serialize(Serializer);
	if (true == GameInstance->Send(Serializer.GetData()))
	{
		GameInstance->m_LoginProcess = true;
	}
}

void APlayerCharacter::SendPlayerUpdatePacket()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (false == IsValid(GameInstance))
	{
		return;
	}

	PlayerUpdateMessage UpdateMessage;
	GameServerSerializer Serializer;
	
	UpdateMessage.m_Datum.m_ThreadIndex = GameInstance->m_ThreadIndex;
	UpdateMessage.m_Datum.m_SectionIndex = GameInstance->m_SectionIndex;
	UpdateMessage.m_Datum.m_ActorIndex = GameInstance->m_ActorIndex;
	
	UpdateMessage.m_Datum.m_Pos = GetActorLocation();
	UpdateMessage.m_Datum.m_Dir = GetActorForwardVector();
	const FQuat Quaternion = GetActorRotation().Quaternion();
	UpdateMessage.m_Datum.m_Rot = FVector4(Quaternion.X, Quaternion.Y, Quaternion.Z, Quaternion.W);

	UpdateMessage.Serialize(Serializer);
	GameInstance->SendTo(Serializer.GetData());
}

void APlayerCharacter::ServerTick(float DeltaTime)
{
	// For Server Message
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (false == IsValid(GameInstance))
	{
		return;
	}

	while (false == GetObjectMessage()->IsEmpty())
	{
		std::shared_ptr<GameServerMessage> Message = GetObjectMessage()->Dequeue();
		if(MessageType::MoveLevel == Message->GetType<MessageType>())
		{
			m_ServerPostEnable = false;
			
			MoveLevelResponseMessage ResponseMessage;
			GameServerSerializer Serializer;
			ResponseMessage.m_ActorIndex = GameInstance->m_ActorIndex;
			ResponseMessage.m_ThreadIndex = GameInstance->m_ThreadIndex;
			ResponseMessage.m_SectionIndex = GameInstance->m_SectionIndex;
			ResponseMessage.Serialize(Serializer);

			GameInstance->Send(Serializer.GetData());
		}
		else if (MessageType::UDPReadyOK == Message->GetType<MessageType>())
		{
			m_UDPReady = true;
		}
		else if (MessageType::ObjectDestroy == Message->GetType<MessageType>())
		{
			Destroy();
		}
		else if (MessageType::PlayerUpdate == Message->GetType<MessageType>())
		{
			m_ServerPostEnable = true;
		}
		else
		{
			
		}
	}

	if (false == m_ServerPostEnable)
	{
		return;
	}

	if (false == m_UDPReady)
	{
		SendPlayerUpdatePacket();
		return;
	}

	if (GetActorLocation() == m_TempVector)
	{
		SendPlayerUpdatePacket();
		return;
	}
	
	SendPlayerUpdatePacket();
	m_TempVector = GetActorLocation();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RotateToTarget();
	CombatCameraSwitch();

	ServerTick(DeltaSeconds);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &APlayerCharacter::PressedLockOn);
	PlayerInputComponent->BindAction("LockOn", IE_Released, this, &APlayerCharacter::ReleasedLockOn);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::PressedAttack);

	PlayerInputComponent->BindAxis("LockOnValue");
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket0", EKeys::NumPadZero));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket1", EKeys::NumPadOne));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("TestPacket2", EKeys::NumPadTwo));

	PlayerInputComponent->BindAction("TestPacket0", EInputEvent::IE_Released, this, &APlayerCharacter::TestPacketUpdate0);
	PlayerInputComponent->BindAction("TestPacket1", EInputEvent::IE_Released, this, &APlayerCharacter::TestPacketUpdate1);
	PlayerInputComponent->BindAction("TestPacket2", EInputEvent::IE_Released, this, &APlayerCharacter::TestPacketUpdate2);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

/*
 * 	?????? ????????? ?????? Enemy??? ????????????.
 * 	????????? ??? ????????? ????????? ???????????? ?????? MasterAI ???????????? ???????????? ?????? ?????????
 * 	?????? ?????? ????????? ????????? ?????? ??????.
 *
 * 	SweepMultiByChannel ???????????? ??????????????? ??????
 * 	?????? ????????? KatanaProject??? ???????????? ???????????? Detect ????????? ????????????.
 *
 * 	?????? ????????? ?????? ?????? ????????? ????????? ???????????????????????? ???????????? ??????
 * 	???, ?????? ????????? ????????? ???????????????????????? ???????????? ?????? ????????????
 * 	????????? Server??? NetMulticast??? ?????? ????????? ?????? Client?????? ???????????????
*/
void APlayerCharacter::Client_FindClosestEnemy_Implementation()
{
	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (false == PlayGameMode->IsValidLowLevel())
	{
		LOG(TEXT("PlayGameMode Is Not Valid"));
		m_IsEnemyFind = false;
		return;
	}
	
	// if (false == IsCombatMode())
	// {
	// 	if (IsValid(m_ClosestEnemy))
	// 	{
	// 		m_ClosestEnemy->GetTargetDecal()->SetVisibility(false);
	// 		m_ClosestEnemy = nullptr;
	// 	}
	//
	// 	m_IsEnemyFind = false;
	// 	return;
	// }

	m_ClosestEnemyDistance = TNumericLimits<float>::Max();
	TArray<AActor*> AllEnemyActor = PlayGameMode->GetObjectGroupRef(EGameObjectType::Enemy);
	for (const auto Actor : AllEnemyActor)
	{
		const float DistanceDiff = Actor->GetDistanceTo(this);
		if (DistanceDiff < m_ClosestEnemyDistance)
		{
			m_ClosestEnemyDistance = DistanceDiff;
			m_ClosestEnemy = Cast<AMasterAICharacter>(Actor);
			m_ClosestEnemy->GetTargetDecal()->SetVisibility(false);
		}
	}

	if (false == IsValid(m_ClosestEnemy))
	{
		m_IsEnemyFind = false;
		MultiCast_IsCombatMode(false, true, false, 600.f);
		Client_ReticleVisibility();
		Server_PlayMontage(m_EquipAnimMontage, 1.f, TEXT("Unequip"));
		return;
	}

	m_ClosestEnemy->GetTargetDecal()->SetVisibility(true);
	SetDetectAttackDirectionTimer();
}

void APlayerCharacter::Client_ReticleVisibility_Implementation()
{
	if (m_IsCombatMode)
		m_CharacterController->GetMainHUD()->SetAttackReticleVisibility(ESlateVisibility::HitTestInvisible);
	else
		m_CharacterController->GetMainHUD()->SetAttackReticleVisibility(ESlateVisibility::Collapsed);
}

/*
 *	SetActorRotation?????? ????????? ????????? ???????????? ???????????? ??????????????? Replicate ??? ????????? ??????.
 *	????????? SetControlRotation??? ???????????? Replicate ??? ????????? ??????.
 *	???????????? ??????????????? ????????? UCharacterMovementComponent??? ?????? ???????????? ???????????? ????????????.
 */
void APlayerCharacter::RotateToTarget()
{
	if (false == IsCombatMode())
	{
		return;
	}

	if (false == IsValid(m_ClosestEnemy))
	{
		return;
	}

	const FRotator CurrentRotation = GetActorRotation();
	const FVector CurrentLocation = GetActorLocation();
	const FVector TargetLocation = m_ClosestEnemy->GetActorLocation();

	// PlayerActor??? ???????????? TargetActor??? ????????? ???????????????
	// FindLookAtRotation ????????? ???????????? ???????????? ????????????.
	// Actor??? ??????????????? Yaw????????? ??????????????? Yaw(Z) ?????? ???????????? ????????????.
	const float ToTargetYaw = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation).Yaw;
	const FRotator ToTargetRotation = { CurrentRotation.Pitch, ToTargetYaw, CurrentRotation.Roll };

	// ?????? Actor??? ?????? ????????? Target??? ????????? ???????????? ?????? ????????????
	const FRotator DesiredRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, ToTargetRotation, GetWorld()->GetDeltaSeconds(), 5.f);

	m_CharacterController->SetControlRotation(DesiredRotation);
}

void APlayerCharacter::CombatCameraSwitch()
{
	FVector CameraBoomTargetOffset = m_CameraBoom->TargetOffset;
	FVector CameraBoomSocketOffset = m_CameraBoom->SocketOffset;
	float CameraBoomTargetArmLength = m_CameraBoom->TargetArmLength;
	FVector CameraBoomRelativeLocation = m_CameraBoom->GetRelativeLocation();
	FRotator FollowCameraRelativeRotation = m_FollowCamera->GetRelativeRotation();
	
	if (m_IsCombatMode)
	{
		CameraBoomTargetOffset = FMath::VInterpTo(CameraBoomTargetOffset, {0.f, 0.f, 30.f},
		                                          GetWorld()->GetDeltaSeconds(), 5.f);

		CameraBoomSocketOffset = FMath::VInterpTo(CameraBoomSocketOffset, {0.f, 100.f, 0.f},
		                                          GetWorld()->GetDeltaSeconds(), 5.f);

		CameraBoomTargetArmLength = FMath::FInterpTo(CameraBoomTargetArmLength, 200.f, GetWorld()->GetDeltaSeconds(),
													 5.f);
		
		CameraBoomRelativeLocation = FMath::VInterpTo(CameraBoomRelativeLocation, {0.f, 0.f, 30.f},
													  GetWorld()->GetDeltaSeconds(), 5.f);

		FollowCameraRelativeRotation = FMath::RInterpTo(FollowCameraRelativeRotation, {0.f, -10.f, 0.f},
		                                                GetWorld()->GetDeltaSeconds(), 5.f);
	}
	else
	{
		CameraBoomTargetOffset = FMath::VInterpTo(CameraBoomTargetOffset, {0.f, 0.f, 150.f},
		                                          GetWorld()->GetDeltaSeconds(), 5.f);

		CameraBoomSocketOffset = FMath::VInterpTo(CameraBoomSocketOffset, {0.f, 0.f, 0.f},
		                                          GetWorld()->GetDeltaSeconds(), 5.f);

		CameraBoomTargetArmLength = FMath::FInterpTo(CameraBoomTargetArmLength, 300.f, GetWorld()->GetDeltaSeconds(),
															 5.f);
		
		CameraBoomRelativeLocation = FMath::VInterpTo(CameraBoomRelativeLocation, {0.f, 0.f, 80.f},
		                                              GetWorld()->GetDeltaSeconds(), 5.f);

		FollowCameraRelativeRotation = FMath::RInterpTo(FollowCameraRelativeRotation, {-30.f, 0.f, 0.f},
														GetWorld()->GetDeltaSeconds(), 5.f);
	}

	m_CameraBoom->TargetOffset = CameraBoomTargetOffset;
	m_CameraBoom->SocketOffset = CameraBoomSocketOffset;
	m_CameraBoom->TargetArmLength = CameraBoomTargetArmLength;
	m_CameraBoom->SetRelativeLocation(CameraBoomRelativeLocation);
	m_FollowCamera->SetRelativeRotation(FollowCameraRelativeRotation);
}

void APlayerCharacter::SetDetectAttackDirectionTimer()
{
	if (false == IsValid(m_CharacterController))
	{
		return;
	}

	if (false == m_IsCombatMode)
	{
		// ????????? LockOn??? ?????? Reticle ????????? ?????????
		Server_SetAttackDirection(EAttackDirection::None);
		const UMainHUD* MainHUD = m_CharacterController->GetMainHUD();
		if (IsValid(MainHUD))
			MainHUD->SetAttackReticleOpacity(m_AttackDirection);
		
		GetWorldTimerManager().ClearTimer(m_DetectAttackDirectionTimer);
	}

	GetWorldTimerManager().SetTimer(m_DetectAttackDirectionTimer, this, &APlayerCharacter::DetectAttackDirection, 0.1f,
	                                true);
}

void APlayerCharacter::SetControllerYawTimer(float MontageLength)
{
	GetWorldTimerManager().SetTimer(m_ControllerYawTimer, this, &APlayerCharacter::ResetControllerYaw, MontageLength, false);
}

void APlayerCharacter::SetCombatOffDelayTimer(float MontageLength)
{
	 GetWorldTimerManager().SetTimer(m_CombatOffDelayTimer, this, &APlayerCharacter::CombatOffDelay, MontageLength, false);
}

void APlayerCharacter::SetAttackTraceTimer(bool SetTimer)
{
	if (false == SetTimer)
	{
		GetWorldTimerManager().ClearTimer(m_AttackTraceTimer);
		m_AlreadyDamagedEnemyArray.Empty();
		
		return;
	}

	if (!GetWorldTimerManager().TimerExists(m_AttackTraceTimer))
		GetWorldTimerManager().SetTimer(m_AttackTraceTimer, this, &APlayerCharacter::AttackTrace, 0.01f, true);
}

/*
 * PlayerController ???????????? GetInputMouseDelta( ) ????????? ????????????
 * ???????????? ???????????? ????????? ???????????? ??????????????? ?????? ?????? ??????????????? ?????? ????????? ????????????
 */
void APlayerCharacter::DetectAttackDirection()
{
	if (false == IsValid(m_CharacterController))
	{
		UE_LOG(LogTemp, Error, TEXT("Character Controller Is Not Valid"));
		return;
	}
	
	float TurnValue = 0.f, UpValue = 0.f;
	m_CharacterController->GetInputMouseDelta(TurnValue, UpValue);

	EAttackDirection AttackDirection = EAttackDirection::None;
	if (TurnValue > 0.5f)
		AttackDirection = EAttackDirection::Right;

	if (TurnValue < -0.5f)
		AttackDirection = EAttackDirection::Left;

	if (UpValue > 0.5f)
		AttackDirection = EAttackDirection::Up;

	// ?????? ????????? ????????? ???????????? ?????? ???????????? Opacity ??????
	if (AttackDirection != EAttackDirection::None && m_AttackDirection != AttackDirection)
	{
		Server_SetAttackDirection(AttackDirection);
	}
}

void APlayerCharacter::ResetControllerYaw()
{
	GetWorldTimerManager().ClearTimer(m_ControllerYawTimer);
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::CombatOffDelay()
{
	GetWorldTimerManager().ClearTimer(m_CombatOffDelayTimer);
	const float LockOnKeyValue = GetInputAxisValue(TEXT("LockOnValue"));
	if (0.f != LockOnKeyValue)
	{
		const float TimerRate = GetWorldTimerManager().GetTimerRate(m_CombatOffDelayTimer);
		GetWorldTimerManager().SetTimer(m_CombatOffDelayTimer, this, &APlayerCharacter::CombatOffDelay, TimerRate);
		return;
	}

	ReleasedLockOn();
}

void APlayerCharacter::SetWeaponSocketLocation_Implementation(FName SocketName)
{
	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	GetWeaponMeshComponent()->AttachToComponent(GetMesh(), rules, SocketName);
}

void APlayerCharacter::AttackTrace_Implementation()
{
	const FQuat SwordQuat = m_SMSword->GetComponentRotation().Quaternion();
	const FVector SwordBottomLocation = m_SMSword->GetSocketLocation(TEXT("S_Bottom"));
	const FVector SwordTopLocation = m_SMSword->GetSocketLocation(TEXT("S_Top"));
	
	const float SocketHalfDiff = UKismetMathLibrary::Vector_Distance(SwordTopLocation, SwordBottomLocation) / 2.f;	
	FVector ToTopVector = SwordTopLocation - SwordBottomLocation;
	ToTopVector.Normalize();

	const FVector SwordCenterLocation = SwordBottomLocation + (ToTopVector * SocketHalfDiff);

	TArray<FHitResult> ResultArray;
	const FCollisionQueryParams ColParam(NAME_None, false, this);
	bool Detect = GetWorld()->SweepMultiByChannel(ResultArray, SwordBottomLocation, SwordTopLocation, SwordQuat,
													ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(20.f), ColParam);
	

#ifdef ENABLE_DRAW_DEBUG
	const FColor DrawColor = Detect ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), SwordCenterLocation, SocketHalfDiff, 20.f, SwordQuat, DrawColor, false, 3.f);
#endif

	for (const auto& HitResult : ResultArray)
	{
		AMasterAICharacter* HitActor = Cast<AMasterAICharacter>(HitResult.Actor);
		if (false == IsValid(HitActor))
		{
			LOGSTRING("Hit Actor Is Not Valid");
			continue;
		}

		if (true == m_AlreadyDamagedEnemyArray.Contains(HitActor))
		{
			continue;
		}

		m_AlreadyDamagedEnemyArray.Add(HitActor);
		Server_ApplyDamage(HitActor, m_CharacterInfo.Attack, m_CharacterController, this);
	}
}

void APlayerCharacter::PressedLockOn()
{
	Server_IsCombatMode(true, false, true, 250.f, TEXT("Equip"));
}

void APlayerCharacter::ReleasedLockOn()
{
	Server_IsCombatMode(false, true, false, 600.f, TEXT("Unequip"));
	
	if (bUseControllerRotationYaw)
		ResetControllerYaw();
}

void APlayerCharacter::PressedAttack()
{
	if (IsCombatMode())
		Server_Attack();
}

void APlayerCharacter::AddControllerYawInput(float Val)
{
	if (!m_IsCombatMode)
		Super::AddControllerYawInput(Val);
}

void APlayerCharacter::AddControllerPitchInput(float Val)
{
	if (!m_IsCombatMode)
		Super::AddControllerPitchInput(Val);
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::TestPacketUpdate0()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (nullptr == GameInstance || false == GameInstance->IsValidLowLevel())
	{
		return;
	}

	if (false == GameInstance->GetClientMode())
	{
		return;
	}

#include "HonorProject/Message/ServerToClient.h"

	std::shared_ptr<EnemyUpdateMessage> Message = std::make_shared<EnemyUpdateMessage>();
	Message->m_ActorIndex = 100;
	Message->m_Pos = FVector(500.0f, 500.0f, 200.0f);
	Message->m_EnemyType = 0;
	GameInstance->PushClientMessage(Message);
}

void APlayerCharacter::TestPacketUpdate1()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (nullptr == GameInstance || false == GameInstance->IsValidLowLevel())
	{
		return;
	}

	if (false == GameInstance->GetClientMode())
	{
		return;
	}

#include "HonorProject/Message/ServerToClient.h"

	std::shared_ptr<EnemyUpdateMessage> Message = std::make_shared<EnemyUpdateMessage>();
	Message->m_ActorIndex = 100;
	Message->m_Pos = FVector(300.0f, 300.0f, 200.0f);
	Message->m_EnemyType = 0;
	//Message->m_UpdateType = EEnemyState::State_Idle;
	GameInstance->PushClientMessage(Message);
}

void APlayerCharacter::TestPacketUpdate2()
{
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (nullptr == GameInstance || false == GameInstance->IsValidLowLevel())
	{
		return;
	}

	if (false == GameInstance->GetClientMode())
	{
		return;
	}

#include "HonorProject/Message/ServerToClient.h"

	std::shared_ptr<ObjectDestroyMessage> Message = std::make_shared<ObjectDestroyMessage>();
	Message->m_ActorIndex = 100;
	GameInstance->PushClientMessage(Message);
}