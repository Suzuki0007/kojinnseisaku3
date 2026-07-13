#pragma once
#include "charabase.h"

class BattleState
{
public:
	BattleState() = default;
	virtual ~BattleState() = default;

	void Enter(CharaBase* owner) {};
	void Update(CharaBase* owner) {};
	void Exit(CharaBase* owner) {};
};

