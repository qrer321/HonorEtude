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
	if (IsValid(GameInstance))
	{
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

		if (true == GameInstance->GetClientMode())
		{
			SetObjectType(EGameObjectType::Player);
			SetObjectID(PlayGameMode->GetUniqueID());
			PlayGameMode->RegistObject(0, EGameObjectType::Player, this);
		}
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RotateToTarget();
	CombatCameraSwitch();
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
 * 	가장 가까이 있는 Enemy를 찾아낸다.
 * 	하지만 이 방법은 월드에 존재하는 모든 MasterAI 액터들을 찾아내고 있기 때문에
 * 	성능 상에 문제가 발생할 수도 있다.
 *
 * 	SweepMultiByChannel 방식으로 변경하여야 한다
 * 	해당 방법은 KatanaProject를 진행하며 만들었던 Detect 함수를 사용한다.
 *
 * 	가장 가까운 적을 찾아 쫓는건 하나의 클라이언트에서만 동작해야 한다
 * 	즉, 타겟 액터를 소유한 클라이언트에서만 실행되야 하는 함수이다
 * 	따라서 Server도 NetMulticast도 일반 함수도 아닌 Client로만 실행시킨다
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
 *	SetActorRotation으로 액터의 회전을 결정하기 위해서는 기본적으로 Replicate 할 필요가 있다.
 *	하지만 SetControlRotation을 사용하면 Replicate 할 필요가 없다.
 *	왜냐하면 컨트롤러의 이동은 UCharacterMovementComponent를 통해 움직임이 복제되기 때문이다.
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

	// PlayerActor의 위치에서 TargetActor의 위치를 바라보도록
	// FindLookAtRotation 함수를 사용하여 회전값을 알아낸다.
	// Actor의 회전에서는 Yaw값만이 필요함으로 Yaw(Z) 값만 추출해서 사용한다.
	const float ToTargetYaw = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation).Yaw;
	const FRotator ToTargetRotation = { CurrentRotation.Pitch, ToTargetYaw, CurrentRotation.Roll };

	// 현재 Actor의 회전 값에서 Target을 바라볼 회전값의 차를 보간한다
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
		// 다음번 LockOn을 위해 Reticle 알파값 초기화
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
 * PlayerController 클래스의 GetInputMouseDelta( ) 함수를 사용하여
 * 순간적인 마우스의 방향을 알아내어 플레이어가 공격 혹은 방어하고자 하는 방향을 알아낸다
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

	// 공격 방향이 이전과 동일하지 않은 경우에만 Opacity 설정
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
	Message->m_ObjectID = 100;
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
	Message->m_ObjectID = 100;
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
	Message->m_ObjectID = 100;
	GameInstance->PushClientMessage(Message);
}