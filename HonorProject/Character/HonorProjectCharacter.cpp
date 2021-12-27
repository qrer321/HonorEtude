// Copyright Epic Games, Inc. All Rights Reserved.

#include "HonorProjectCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

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

	m_AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
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
}

void AHonorProjectCharacter::Server_IsCombatMode_Implementation(bool IsCombatMode, bool UseOrientRotation,
                                                                bool UseControllerDesiredRotation, float MaxWalkSpeed,
                                                                FName SectionName)
{
	// RunonServer로 설정된 Server_IsCombatMode( ) 함수를 호출하면
	// NetMulticast로 설정된 Client_IsCombatMode( ) 함수를 호출하게 된다.
	
	// 전체적인 흐름으로는 클라이언트 A가 Server 함수를 호출하면
	// 해당 Server 함수가 서버에서 Client 함수를 호출하게 되고
	// 서버와 클라이언트 B를 포함한 다른 모든 클라이언트에서 B 함수가 호출된다.
	Client_IsCombatMode(IsCombatMode, UseOrientRotation, UseControllerDesiredRotation, MaxWalkSpeed, SectionName);
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
