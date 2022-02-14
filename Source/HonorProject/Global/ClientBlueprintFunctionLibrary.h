// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Global/GameInfo.h"
#include <string>
#include <vector>
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ClientBlueprintFunctionLibrary.generated.h"

UCLASS()
class HONORPROJECT_API UClientBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool FStringToUTF8(const FString& String, std::string& UTF8);
	static bool StringToData(const std::string& UTF8, std::vector<uint8> Data);
	static bool FStringToUTF8Data(const FString& String, std::vector<uint8> Data);

	static bool UTF8ToFString(const std::string& UTF8, FString& String);
	static FString UTF8ToFString(const std::string& UTF8);
};
