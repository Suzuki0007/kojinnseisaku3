#include "pch.h"
#include "alivestate.h"

bool AliveState::ApplyDamage(float damage)
{
	_hp -= damage; // ダメージをHPから減算
	if(_hp < 0.0f)
	{
		_hp = 0.0f;
		return true; // 死亡状態に遷移
	}
	return false; // 生存状態のまま
}