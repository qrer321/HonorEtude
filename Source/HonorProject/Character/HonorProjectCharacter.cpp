// Fill out your copyright notice in the Description page of Project Settings.


#include "HonorProjectCharacter.h"
#include "HonorProject/GameMode/PlayGameMode.h"

// Sets default values
AHonorProjectCharacter::AHonorProjectCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAsset(TEXT("AnimMontage'/Game/HonorProejct/PlayRelevant/Character/AnimMontage/GreatSword_Equip_Montage.GreatSword_Equip_Montage'"));
	if (EquipAsset.Succeeded())
		m_EquipAnimMontage = EquipAsset.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackUpAsset(TEXT("AnimMontage'/Game/HonorProejct/PlayRelevant/Character/AnimMontage/GreatSword_Attack_Up_Montage.GreatSword_Attack_Up_Montage'"));
	if (AttackUpAsset.Succeeded())
		m_AttackUpMontage = AttackUpAsset.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackLeftAsset(TEXT("AnimMontage'/Game/HonorProejct/PlayRelevant/Character/AnimMontage/GreatSword_Attack_Left_Montage.GreatSword_Attack_Left_Montage'"));
	if (AttackLeftAsset.Succeeded())
		m_AttackLeftMontage = AttackLeftAsset.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackRightAsset(TEXT("AnimMontage'/Game/HonorProejct/PlayRelevant/Character/AnimMontage/GreatSword_Attack_Right_Montage.GreatSword_Attack_Right_Montage'"));
	if (AttackRightAsset.Succeeded())
		m_AttackRightMontage = AttackRightAsset.Object;

	m_MessageComponent = CreateDefaultSubobject<UObjectMessageComponent>(TEXT("Message Component"));
}

// Called when the game starts or when spawned
void AHonorProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	m_AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	m_AttackDirection = EAttackDirection::None;
}

// Called every frame
void AHonorProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHonorProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHonorProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHonorProjectCharacter, m_CharacterInfo);
	DOREPLIFETIME(AHonorProjectCharacter, m_IsCombatMode);
	DOREPLIFETIME(AHonorProjectCharacter, m_AttackDirection);
}

/*
 * RunonServer로 설정된 Server_IsCombatMode( ) 함수를 호출하면
 * NetMulticast로 설정된 MultiCast_IsCombatMode( ) 함수를 호출하게 된다.
 *
 * 전체적인 흐름으로는 클라이언트 A가 Server 함수를 호출하면
 * 해당 Server 함수가 서버에서 MultiCast 함수를 호출하게 되고
 * 서버와 클라이언트 B를 포함한 다른 모든 클라이언트에서 B 함수가 호출된다.
 */
void AHonorProjectCharacter::Server_IsCombatMode_Implementation(bool IsCombatMode, bool UseOrientRotation,
                                                                bool UseControllerDesiredRotation, float MaxWalkSpeed,
                                                                FName StartSectionName)
{
	// Delay Timer가 설정되어 있다면 return
	if (true == GetWorldTimerManager().IsTimerActive(m_CombatOffDelayTimer))
	{
		return;
	}

	// 먼저 가장 가까이 있는 Enemy Actor를 찾는다.
	// 없으면 그냥 return
	m_IsEnemyFind = true;
	Client_FindClosestEnemy();
	if (false == m_IsEnemyFind)
	{
		// Replicated Function의 경우 return value를 반환할 수 없고,
		// non-const reference 또한 사용할 수 없기에
		// bool 멤버변수로 확인한다.
		return;
	}
	
	MultiCast_IsCombatMode(IsCombatMode, UseOrientRotation, UseControllerDesiredRotation, MaxWalkSpeed);
	Client_ReticleVisibility();
	Server_PlayMontage(m_EquipAnimMontage, 1.f, StartSectionName);
}

void AHonorProjectCharacter::MultiCast_IsCombatMode_Implementation(bool IsCombatMode, bool UseOrientRotation,
	bool UseControllerDesiredRotation, float MaxWalkSpeed)
{
	m_IsCombatMode = IsCombatMode;
	
	GetCharacterMovement()->bOrientRotationToMovement = UseOrientRotation;
	GetCharacterMovement()->bUseControllerDesiredRotation = UseControllerDesiredRotation;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	SetWeaponCheckTimer();
}

void AHonorProjectCharacter::Server_PlayMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	MultiCast_PlayMontage(AnimMontage, InPlayRate, StartSectionName);
}

void AHonorProjectCharacter::MultiCast_PlayMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	if (false == IsValid(m_AnimInstance))
	{
		LOG(TEXT("AnimInstance Is Not Valid"));
		return;
	}

	if (false == IsValid(AnimMontage))
	{
		LOG(TEXT("AnimMontage Is Not Valid"));
		return;
	}

	// StartSectionName이 Name_None인 경우...AnimMontage에 하나의 AnimSequence만 있는 경우
	// StartSectionName이 Name_None이 아니면서 같은 Section을 반복재생하는 AnimMontage가 없다고 가정하고 함수 구성
	if (true == StartSectionName.IsNone())
	{
		PlayAnimMontage(AnimMontage, InPlayRate);
		return;
	}
	
	const UAnimMontage* CurrentAnimMontage = m_AnimInstance->GetCurrentActiveMontage();
	if (false == IsValid(CurrentAnimMontage))
	{
		m_AnimInstance->Montage_Play(AnimMontage, InPlayRate);
		m_AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
		return;
	}

	if (CurrentAnimMontage != AnimMontage)
	{
		m_AnimInstance->Montage_Play(AnimMontage, InPlayRate);
		m_AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
		return;
	}

	/*
	 * Equip / UnEquip과 같이 연관되어 있는 AnimMontage의 경우
	 * 동작을 반복 실행했을 때 처음부터 애니메이션이 실행되는 것이 아닌
	 * 이전 Section의 진행된 길이만큼 현재 재생될 Animation Section
	 * 길이에서 뺀 위치에서 실행되도록 설정
	 */
	const float CurrentMontagePosition = m_AnimInstance->Montage_GetPosition(CurrentAnimMontage);
	const int32 CurrentSectionIndex = CurrentAnimMontage->GetSectionIndexFromPosition(CurrentMontagePosition);
	float CurrentSectionStartTime = 0.f, CurrentSectionEndTime = 0.f;
	CurrentAnimMontage->GetSectionStartAndEndTime(CurrentSectionIndex, CurrentSectionStartTime, CurrentSectionEndTime);
	
	const int32 NextSectionIndex = AnimMontage->GetSectionIndex(StartSectionName);
	float NextSectionStartTime = 0.f, NextSectionEndTime = 0.f;
	AnimMontage->GetSectionStartAndEndTime(NextSectionIndex, NextSectionStartTime, NextSectionEndTime);

	// 다른 Section의 진행 시간만큼을 빼고 재생되어야 할 현재 Montage Section의 시작 위치
	const float ModifiedSectionTime = CurrentMontagePosition - CurrentSectionStartTime;
	const float JumpSectionTime = NextSectionEndTime - ModifiedSectionTime;


	m_AnimInstance->Montage_Play(AnimMontage, InPlayRate, EMontagePlayReturnType::MontageLength, JumpSectionTime);
}

/*
 * 각각의 클라이언트가 가지고 있는 AttackDirection을 서버에 전송하여
 * 모든 클라이언트가 다른 클라이언트의 AttackDirection을 알도록 만든다
 */
void AHonorProjectCharacter::Server_SetAttackDirection_Implementation(EAttackDirection AttackDirection)
{
	MultiCast_SetAttackDirection(AttackDirection);

	Client_SetAttackReticle();
}

void AHonorProjectCharacter::MultiCast_SetAttackDirection_Implementation(EAttackDirection AttackDirection)
{
	m_AttackDirection = AttackDirection;
}

void AHonorProjectCharacter::Client_SetAttackReticle_Implementation()
{
	if (IsValid(m_CharacterController))
	{
		const UMainHUD* MainHUD = m_CharacterController->GetMainHUD();
		if (IsValid(MainHUD))
			MainHUD->SetAttackReticleOpacity(m_AttackDirection);		
	}
}

/*
 * 서버에 저장되고 있는 AttackDirection에 따라서
 * 각각 다른 애니메이션 몽타주를 재생한다
 * 한 방향에 따른 콤보 애니메이션이 추가된다면
 * StartSocketName 인자를 활용하게 만든다
 */
void AHonorProjectCharacter::Server_Attack_Implementation()
{
	MultiCast_Attack();
}

void AHonorProjectCharacter::MultiCast_Attack_Implementation()
{
	if (m_EquipAnimMontage == GetCurrentMontage())
	{
		return;
	}

	if (true == GetWorldTimerManager().IsTimerActive(m_CombatOffDelayTimer))
	{
		const float RemainTime = GetWorldTimerManager().GetTimerRemaining(m_CombatOffDelayTimer);
		if (1.f < RemainTime)
		{
			// 버그성 연속 공격을 막도록
			// CombatOffDelayTimer가 설정되어 있다면 강제 return
			return;
		}		
	}
	
	
	UAnimMontage* AnimMontage = nullptr;
	
	EAttackDirection AttackDirection = GetAttackDirection();
	switch (static_cast<uint8>(AttackDirection))
	{
	case EAttackDirection::Up:
		AnimMontage = m_AttackUpMontage;
		break;
	case EAttackDirection::Left:
		AnimMontage = m_AttackLeftMontage;
		break;
	case EAttackDirection::Right:
		AnimMontage = m_AttackRightMontage;
		break;
	default:
		break;
	}

	if (false == IsValid(AnimMontage))
	{
		return;
	}

	bUseControllerRotationYaw = true;
	Server_PlayMontage(AnimMontage);
	
	const float MontageLength = AnimMontage->GetPlayLength() - AnimMontage->BlendOut.GetBlendTime();
	SetControllerYawTimer(MontageLength);
	SetCombatOffDelayTimer(MontageLength);
}

void AHonorProjectCharacter::Server_ApplyDamage_Implementation(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser)
{
	MultiCast_ApplyDamage(DamagedActor, BaseDamage, EventInstigator, DamageCauser);
}

void AHonorProjectCharacter::MultiCast_ApplyDamage_Implementation(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser)
{
	UGameplayStatics::ApplyDamage(DamagedActor, BaseDamage, EventInstigator, DamageCauser, nullptr);
}

void AHonorProjectCharacter::SetWeaponCheckTimer()
{
	GetWorldTimerManager().SetTimer(m_WeaponCheckTimer, this, &AHonorProjectCharacter::WeaponCheck, 0.1f, true);
}

/*
 * Equip 혹은 UnEquip Section이 실행될 때
 * 처음부터 Section이 실행되는 것이 아닌
 * 이전에 실행한 Section의 최종 실행 프레임을 기반으로
 * 다음번에 실행할 Section의 시작 프레임 위치를 조정해주었다.
 *
 * 이와 같이 설정했을 때 AnimNotify Function이 실행되지 못하고 넘어가게 되어
 * 무기의 소켓위치가 이상할 때가 많아 현재 Section 위치를 확인하고
 * Socket 위치가 올바르지 않다면 수정해주기 위한 함수이다.
 */
void AHonorProjectCharacter::WeaponCheck()
{
	if (false == IsValid(m_AnimInstance))
	{
		LOGSTRING(TEXT("AnimInstance Is Not Valid"));
		return;
	}
	
	UAnimMontage* CurrentAnimMontage = GetCurrentMontage();
	if (false == IsValid(CurrentAnimMontage) || m_EquipAnimMontage != CurrentAnimMontage)
	{
		return;
	}

	const float CurrentSectionFriction = 1.f - m_AnimInstance->GetCurrentMontageRemainingFriction();
	if (0.3f < CurrentSectionFriction)
	{
		// 남은 길이가 일정 위치를 넘어설 경우
		FName SocketName;
		if (true == IsCombatMode())
		{
			SocketName = TEXT("S_Equip");
		}
		else if (false == IsCombatMode())
		{
			SocketName = TEXT("S_Unequip");
		}

		SetWeaponSocketLocation(SocketName);
		GetWorldTimerManager().ClearTimer(m_WeaponCheckTimer);
	}
}

void AHonorProjectCharacter::Destroyed()
{
	Super::Destroyed();

	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (nullptr == PlayGameMode || false == PlayGameMode->IsValidLowLevel())
	{
		LOG(TEXT("PlayGameMode Is Not Valid"));
		return;
	}

	PlayGameMode->UnregistObject(m_ObjectID, GetObjectType(), this);
}

float AHonorProjectCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                         AActor* DamageCauser)
{
	if (false == IsLocallyControlled())
	{
		return 0.f;
	}
	
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (1.f > Damage)
	{
		return Damage;
	}
	
	Damage = Damage - m_CharacterInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;

	MultiCast_TakeDamage(Damage);
	
	return Damage;
}

void AHonorProjectCharacter::MultiCast_TakeDamage_Implementation(float DamageAmount)
{
	// m_CharacterInfo는 Replicated 했지만
	// struct 내부 원소들까지 replicate 되는지는 확인할 수 없었다.
	// 만약 서버에서 클라로 복사가 됐다고 하면 클라가 들고있는 CharacterInfo HP가 수정되어 있어야 하지만
	// 그렇지 않았고, 함수 호출이 제대로 이루어지지 않아 당장 체력을 줄이는것은 MultiCast로 설정했다...
	m_CharacterInfo.HP -= DamageAmount;
}