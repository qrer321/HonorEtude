#pragma once

enum class EGameObjectType
{
	Player,
	Monster,
	Item,
	MAX
};

enum class EEnemyState
{
	NONE,
	State_Idle,
	State_Trace,
	State_Attack,
	MAX
};