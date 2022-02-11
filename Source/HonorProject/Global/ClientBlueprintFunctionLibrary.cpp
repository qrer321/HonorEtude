// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientBlueprintFunctionLibrary.h"

bool UClientBlueprintFunctionLibrary::FStringToUTF8(const FString& String, std::string& UTF8)
{
	ANSICHAR* Ansi = TCHAR_TO_UTF8(*String);
	UTF8 = std::string(Ansi);

	return true;
}

bool UClientBlueprintFunctionLibrary::StringToData(const std::string& UTF8, std::vector<uint8> Data)
{
	Data.clear();
	Data.resize(UTF8.size() + 1);
	Data[UTF8.size()] = 0;
	std::copy(UTF8.begin(), UTF8.end(), Data.begin());

	return true;
}

bool UClientBlueprintFunctionLibrary::FStringToUTF8Data(const FString& String, std::vector<uint8> Data)
{
	std::string Text;
	FStringToUTF8(String, Text);
	StringToData(Text, Data);

	return true;
}

bool UClientBlueprintFunctionLibrary::UTF8ToFString(const std::string& UTF8, FString& String)
{
	String = UTF8ToFString(UTF8);
	return true;
}

FString UClientBlueprintFunctionLibrary::UTF8ToFString(const std::string& UTF8)
{
	return FString(UTF8_TO_TCHAR(&UTF8[0]));
}
