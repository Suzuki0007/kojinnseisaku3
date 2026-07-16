#include "pch.h"
#include "enemytargetprovider.h"
#include "enemymanager.h"

std::vector<CharaBase*> EnemyTargetProvider::GetTargets() const
{
	std::vector<CharaBase*> result;
	auto& enemies = EnemyManager::GetInstance()->GetEnemies();

	result.reserve(enemies.size());

	for(auto& enemy : enemies)
	{
		if(enemy && enemy->IsAlive())
		{
			result.emplace_back(enemy.get());
		}
	}

	return result;
}