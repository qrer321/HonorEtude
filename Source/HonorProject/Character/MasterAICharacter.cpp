// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterAICharacter.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "HonorProject/UI/Play/HealthHUD.h"

// Sets default values
AMasterAICharacter::AMasterAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_TargetDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Target Decal"));
	m_TargetDecal->SetupAttachment(GetMesh());
	m_TargetDecal->SetVisibility(false);

	m_HealthHUD = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP"));
	m_HealthHUD->SetupAttachment(GetCapsuleComponent());
	m_HealthHUD->SetWidgetSpace(EWidgetSpace::Screen);
	m_HealthHUD->SetBlendMode(EWidgetBlendMode::Transparent);
	m_HealthHUD->SetDrawSize(FVector2D(200, 15));
	m_HealthHUD->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarAsset(TEXT("WidgetBlueprint'/Game/HonorProejct/PlayRelevant/UI/WB_HPBar.WB_HPBar_C'"));
	if (HPBarAsset.Succeeded())
		m_HealthHUD->SetWidgetClass(HPBarAsset.Class);
}

// Called when the game starts or when spawned
void AMasterAICharacter::BeginPlay()
{
	Super::BeginPlay();

	const UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetWorld()->GetGameInstance());
	if (false == IsValid(GameInstance))
	{
		LOGSTRING(TEXT("GameInstance Is Not Valid"));
		return;
	}
	
	if (false == IsValid(m_HealthHUD))
	{
		LOGSTRING(TEXT("Helath HUD Is Not Valid"));
		return;
	}

	m_HealthWidget = Cast<UHealthHUD>(m_HealthHUD->GetWidget());
	if (false == IsValid(m_HealthWidget))
	{
		LOGSTRING(TEXT("Helath Widget Is Not Valid"));
		return;
	}
	
	m_HealthWidget->SetWidgetOwner(this);
	//m_HealthWidget->SetHealthVisibility(false);

	const FCharacterTableInfo* CharacterInfo = GameInstance->FindCharacterInfo(TEXT("Player"));
	if (nullptr != CharacterInfo)
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
}

// Called every frame
void AMasterAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMasterAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMasterAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	MultiCast_CalcDamage(Damage);

	return Damage;
}
void AMasterAICharacter::MultiCast_CalcDamage_Implementation(float DamageAmount)
{
	if (IsValid(m_HealthWidget))
	{
		m_HealthWidget->SetHPPercent(m_CharacterInfo.HP / static_cast<float>(m_CharacterInfo.HPMax));
	}
}