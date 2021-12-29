// Copyright Epic Games, Inc. All Rights Reserved.

#include "HonorProjectCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MasterAICharacter.h"

//////////////////////////////////////////////////////////////////////////
// AHonorProjectCharacter

AHonorProjectCharacter::AHonorProjectCharacter()
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
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	m_SMSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Sword"));
	
	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	m_SMSword->AttachToComponent(GetMesh(), rules, TEXT("S_Unequip"));
	m_SMSword->SetIsReplicated(true);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SwordAsset(TEXT("StaticMesh'/Game/GKnight/Meshes/Weapon/SM_WP_GothicKnight_Sword.SM_WP_GothicKnight_Sword'"));
	if (SwordAsset.Succeeded())
		m_SMSword->SetStaticMesh(SwordAsset.Object);


	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAnimAsset(TEXT("AnimMontage'/Game/HonorProejct/Character/AnimMontage/GreatSword_Equip_Montage.GreatSword_Equip_Montage'"));
	if (EquipAnimAsset.Succeeded())
		m_EquipAnimMontage = EquipAnimAsset.Object;
}

void AHonorProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_CharacterController = Cast<ACharacterController>(GetController());
	m_AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	m_AttackDirection = EAttackDirection::None;
}

void AHonorProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AHonorProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &AHonorProjectCharacter::PressedLockOn);
	PlayerInputComponent->BindAction("LockOn", IE_Released, this, &AHonorProjectCharacter::ReleasedLockOn);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHonorProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHonorProjectCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHonorProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHonorProjectCharacter::LookUpAtRate);
}

void AHonorProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(AHonorProjectCharacter, m_IsCombatMode, COND_SkipOwner);
	DOREPLIFETIME(AHonorProjectCharacter, m_IsCombatMode);
	DOREPLIFETIME(AHonorProjectCharacter, m_ClosestEnemyDistance);
	DOREPLIFETIME(AHonorProjectCharacter, m_ClosestEnemy);
}

/*
 * RunonServer로 설정된 Server_IsCombatMode( ) 함수를 호출하면
 * NetMulticast로 설정된 Client_IsCombatMode( ) 함수를 호출하게 된다.
 *
 * 전체적인 흐름으로는 클라이언트 A가 Server 함수를 호출하면
 * 해당 Server 함수가 서버에서 Client 함수를 호출하게 되고
 * 서버와 클라이언트 B를 포함한 다른 모든 클라이언트에서 B 함수가 호출된다.
 */
void AHonorProjectCharacter::Server_IsCombatMode_Implementation(bool IsCombatMode, bool UseOrientRotation,
                                                                bool UseControllerDesiredRotation, float MaxWalkSpeed,
                                                                FName SectionName)
{
	Client_IsCombatMode(IsCombatMode, UseOrientRotation, UseControllerDesiredRotation, MaxWalkSpeed, SectionName);
	Client_FindClosestEnemy();
	Client_ReticleVisibility();
	Server_PlayMontage(m_EquipAnimMontage, 1.f, SectionName);
}

void AHonorProjectCharacter::Client_IsCombatMode_Implementation(bool IsCombatMode, bool UseOrientRotation,
	bool UseControllerDesiredRotation, float MaxWalkSpeed, FName SectionName)
{
	m_IsCombatMode = IsCombatMode;
	
	GetCharacterMovement()->bOrientRotationToMovement = UseOrientRotation;
	GetCharacterMovement()->bUseControllerDesiredRotation = UseControllerDesiredRotation;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void AHonorProjectCharacter::Server_PlayMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSocketName)
{
	Client_PlayMontage(AnimMontage, InPlayRate, StartSocketName);
}

void AHonorProjectCharacter::Client_PlayMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSocketName)
{
	PlayAnimMontage(AnimMontage, InPlayRate, StartSocketName);
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
void AHonorProjectCharacter::Client_FindClosestEnemy_Implementation()
{
	m_ClosestEnemyDistance = TNumericLimits<float>::Max();

	TArray<AActor*> AllEnemyActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterAICharacter::StaticClass(), AllEnemyActor);

	for (const auto& actor : AllEnemyActor)
	{
		const float DistanceDiff = actor->GetDistanceTo(this);
		if (DistanceDiff < m_ClosestEnemyDistance)
		{
			m_ClosestEnemyDistance = DistanceDiff;
			m_ClosestEnemy = Cast<AMasterAICharacter>(actor);
			m_ClosestEnemy->GetTargetDecal()->SetVisibility(false);
		}
	}

	m_ClosestEnemy->GetTargetDecal()->SetVisibility(true);

	SetTargetRotateTimer();
	SetDetectAttackDirectionTimer();
}

void AHonorProjectCharacter::Client_ReticleVisibility_Implementation()
{
	if (m_IsCombatMode)
		m_CharacterController->GetMainHUD()->SetAttackReticleVisibility(ESlateVisibility::HitTestInvisible);
	else
		m_CharacterController->GetMainHUD()->SetAttackReticleVisibility(ESlateVisibility::Hidden);
}

void AHonorProjectCharacter::SetTargetRotateTimer()
{
	if (m_IsCombatMode)
	{
		GetWorldTimerManager().SetTimer(m_ToTargetRotateTimer, this, &AHonorProjectCharacter::RotateToTarget, 0.01f, true);
	}
	else
	{
		if (IsValid(m_ClosestEnemy))
		{
			m_ClosestEnemy->GetTargetDecal()->SetVisibility(false);
			GetWorldTimerManager().ClearTimer(m_ToTargetRotateTimer);
			
			m_ClosestEnemy = nullptr;
		}
	}
}

void AHonorProjectCharacter::SetDetectAttackDirectionTimer()
{
	if (m_IsCombatMode)
	{
		GetWorldTimerManager().SetTimer(m_DetectAttackDirectionTimer, this, &AHonorProjectCharacter::DetectAttackDirection, 0.01f, true);
	}
	else
	{
		// 다음번 LockOn을 위해 Reticle 알파값 초기화
		m_AttackDirection = EAttackDirection::None;
		const UMainHUD* MainHUD = m_CharacterController->GetMainHUD();
		if (IsValid(MainHUD))
			MainHUD->SetAttackReticleOpacity(m_AttackDirection);
		
		GetWorldTimerManager().ClearTimer(m_DetectAttackDirectionTimer);
	}
}

/*
 *	SetActorRotation으로 액터의 회전을 결정하기 위해서는 기본적으로 Replicate 할 필요가 있다.
 *	하지만 SetControlRotation을 사용하면 Replicate 할 필요가 없다.
 *	왜냐하면 컨트롤러의 이동은 UCharacterMovementComponent를 통해 움직임이 복제되기 때문이다.
 */
void AHonorProjectCharacter::RotateToTarget() const
{
	if (!IsValid(m_ClosestEnemy))
		return;

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

	GetController()->SetControlRotation(DesiredRotation);
}

/*
 * PlayerController 클래스의 GetInputMouseDelta( ) 함수를 사용하여
 * 순간적인 마우스의 방향을 알아내어 플레이어가 공격 혹은 방어하고자 하는 방향을 알아낸다
 */
void AHonorProjectCharacter::DetectAttackDirection()
{
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
		m_AttackDirection = AttackDirection;

		const UMainHUD* MainHUD = m_CharacterController->GetMainHUD();
		if (IsValid(MainHUD))
			MainHUD->SetAttackReticleOpacity(m_AttackDirection);
	}
}

void AHonorProjectCharacter::PressedLockOn()
{
	Server_IsCombatMode(true, false, true, 250.f, TEXT("Equip"));
}

void AHonorProjectCharacter::ReleasedLockOn()
{
	Server_IsCombatMode(false, true, false, 600.f, TEXT("Unequip"));
}

void AHonorProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHonorProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHonorProjectCharacter::MoveForward(float Value)
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

void AHonorProjectCharacter::MoveRight(float Value)
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
