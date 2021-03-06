#pragma once
#include "GameServerSerializer.h"

struct FCharacterInfo 
{
public:
	int			m_Index{};
	std::string m_Nickname;
	int			m_UserIndex{};
	float		m_HP{};
	float		m_Att{};
	int			m_LastSectionID{};
	float		m_LastSectionPosX{};
	float		m_LastSectionPosY{};
	float		m_LastSectionPosZ{};

public:
	[[nodiscard]] int GetDataSize() const { return (4 * 8) + 4 + static_cast<int>(m_Nickname.size()); }

	void Serialize(GameServerSerializer& serializer)
	{
		serializer << m_Index;
		serializer << m_Nickname;
		serializer << m_UserIndex;
		serializer << m_HP;
		serializer << m_Att;
		serializer << m_LastSectionID;
		serializer << m_LastSectionPosX;
		serializer << m_LastSectionPosY;
		serializer << m_LastSectionPosZ;
	}

	void DeSerialize(GameServerSerializer& serializer)
	{
		serializer >> m_Index;
		serializer >> m_Nickname;
		serializer >> m_UserIndex;
		serializer >> m_HP;
		serializer >> m_Att;
		serializer >> m_LastSectionID;
		serializer >> m_LastSectionPosX;
		serializer >> m_LastSectionPosY;
		serializer >> m_LastSectionPosZ;
	}

public:
	FCharacterInfo() = default;
	~FCharacterInfo() = default;
};

struct FPlayerUpdateData
{
	uint64_t	m_ActorIndex;
	uint64_t	m_ThreadIndex;
	uint64_t	m_SectionIndex;
	FVector4	m_Pos;
	FVector4	m_Dir;
	FVector4	m_Rot;
	int			m_State;

	int GetDataSize() const { return sizeof(FPlayerUpdateData); }

	template <typename StateType>
	StateType GetState() const
	{
		return static_cast<StateType>(m_State);
	}

	template <typename StateType>
	void SetState(StateType type)
	{
		m_State = static_cast<int>(type);
	}

	void Serialize(GameServerSerializer& serializer)
	{
		serializer << m_ActorIndex;
		serializer << m_ThreadIndex;
		serializer << m_SectionIndex;
		serializer << m_Pos;
		serializer << m_Dir;
		serializer << m_Rot;
		serializer << m_State;
	}

	void DeSerialize(GameServerSerializer& serializer)
	{
		serializer >> m_ActorIndex;
		serializer >> m_ThreadIndex;
		serializer >> m_SectionIndex;
		serializer >> m_Pos;
		serializer >> m_Dir;
		serializer >> m_Rot;
		serializer >> m_State;
	}

public:
	FPlayerUpdateData() = default;
	~FPlayerUpdateData() = default;
};