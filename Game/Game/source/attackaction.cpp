#include "pch.h"
#include "attackaction.h"

std::string AttackAction::GetName() const
{
	return "たたかう";
}

void AttackAction::Execute(IBattleReceiver* user, IBattleReceiver* target)
{
	if(user)
	{
		user->CommandAttack(target);
	}
}
