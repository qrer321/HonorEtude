
#pragma once

#include "Engine.h"
#include "Engine\AssetManager.h"
#include "EngineGlobals.h"
#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"

DECLARE_LOG_CATEGORY_EXTERN(HonorProject, Log, All);
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))
#define LOG(Format, ...) UE_LOG(HonorProject, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOGSTRING(Str)	 UE_LOG(HonorProject, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)

void PrintViewport(float _fTime, const FColor& _Color, const FString& _Text);