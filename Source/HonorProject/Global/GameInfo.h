
#pragma once

#include "Engine.h"
#include "Engine\AssetManager.h"
#include "EngineGlobals.h"
#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

DECLARE_LOG_CATEGORY_EXTERN(HonorProject, Log, All);	// 로그 카테고리 지정
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))
#define LOG(Format, ...) UE_LOG(HonorProject, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOGSTRING(Str)	 UE_LOG(HonorProject, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)

void PrintViewport(float _fTime, const FColor& _Color, const FString& _Text);

#include "GameInfo.generated.h"

UENUM(Blueprintable)
enum class EAttackDirection : uint8
{
	Up,
	Left,
	Right,
	None
};

USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SPRecoverMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;
};