#pragma once
#include "battleaction.h"

class AttackAction : public BattleAction
{
public:
	std::string GetName() const override;

	void Execute(IBattleReceiver* user, IBattleReceiver* target) override;
};

