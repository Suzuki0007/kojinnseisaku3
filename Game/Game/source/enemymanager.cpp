#include "pch.h"
#include "enemymanager.h"
#include "enemy.h"

void EnemyManager::RegisterType()
{
	OMB::RegisterType("Enemy", []() { return std::make_unique<Enemy>(); });
}

void EnemyManager::CreateEnemy(const std::string& type)
{
	Create(type);
}