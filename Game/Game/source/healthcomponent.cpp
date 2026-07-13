#include "pch.h"
#include "healthcomponent.h"

bool HealthComponent::IsAlive() const
{
	return _stateMachine.IsInState<AliveState>();
}

void HealthComponent::ApplyDamage(float damage)
{
	_stateMachine.Execute([this, damage](auto& state)
		{
			if(state.ApplyDamage(damage))
			{
				// ダメージを受けて死亡状態に遷移
				_stateMachine.ChangeState<DeadState>();
			}
		});
}

auto HealthComponent::GetHP() const
{
	return _stateMachine.Execute([](const auto& state)
		{
			return state.GetHP();
		});
}

auto HealthComponent::GetMaxHP() const
{
	return _stateMachine.Execute([](const auto& state)
		{
			return state.GetMaxHP();
		});
}

void HealthComponent::SetHP(float hp)
{
	_stateMachine.Execute([this, hp](auto& state)
		{
			state.SetHP(hp);
		});
}
