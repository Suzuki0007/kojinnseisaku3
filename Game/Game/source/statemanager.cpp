#include "pch.h"
#include "statemanager.h"

StateManager::StateManager(CharaBase* owner) 
	: base(*owner)
	, _stateMachine(WaitState{})
{
}

void StateManager::ChangeBattleMode(bool isBattle)
{
	if(isBattle)
	{
		_stateMachine.ChangeState<BattleState>();
	}
	else
	{
		_stateMachine.ChangeState<WaitState>();
	}
}

bool StateManager::Process()
{
	if(!base::Process()) { return false; }

	_stateMachine.Execute([this](auto& state)
		{
			state.Update(_owner);
		});
	return true;
}
