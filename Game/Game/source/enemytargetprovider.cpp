#include "pch.h"
#include "enemytargetprovider.h"
#include "enemymanager.h"
#include "cube.h"

EnemyTargetProvider::EnemyTargetProvider(const std::vector<std::shared_ptr<Cube>>& cubes)
	: _cubes(cubes)
{}

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

bool EnemyTargetProvider::IsBlocked(const Vec4& from, const Vec4& to) const
{
	for(auto& cube : _cubes)
	{
		if(!cube || cube->GetVertexCount() == 0)
		{
			continue;
		}

		if(mymath::LineIntersectAABB(from, to, cube->GetAABB()))
		{
			return true;
		}
	}
	return false;
}