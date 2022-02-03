// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include <vector>
#include <string>

class HONORPROJECT_API GameServerSerializer
{
private:
	unsigned int m_Offset;
	TArray<uint8> m_Data;
	
public:
	GameServerSerializer();

public:
	void operator<<(const std::string& Value);
	void operator<<(const int Value);
	void operator<<(const unsigned int Value);

	void operator>>(std::string& Value);
	void operator>>(int& Value);
	void operator>>(unsigned int& Value);

public:
	const TArray<uint8>& GetData() { return m_Data; }

	void Read(void* Data, unsigned int Size);
	void Write(const void* Data, unsigned int Size);
};
