#include "pch.h"
#include "modegame.h"
#include "player.h"
#include "cube.h"
#include "map.h"
#include "enemytargetprovider.h"
#include "neartargetstrategy.h"

// オブジェクトの初期化
// オブジェクトの初期化
bool ModeGame::ObjectInitialize()
{
	// キューブ初期化
	for(int i = 0; i < CUBE_COUNT; i++)
	{
		auto cube = std::make_shared<Cube>();
		_object.emplace_back(cube);
		_cube.emplace_back(cube);
	}

	// マップ初期化
	_map = std::make_shared<Map>();
	_object.emplace_back(_map);

	// プレイヤー初期化
	auto* pm = PlayerManager::GetInstance();
	pm->RegisterType();
	pm->CreatePlayer("Player");
	pm->CreatePlayer("Player2");
	pm->CreatePlayer("Player3");

	for(auto& player : pm->GetPlayer())
	{
		player->Initialize();
		_chara.emplace_back(player.get());

		auto* targetComponent = player->AddComponent<TargetComponent>
			(
				std::make_unique<NearTargetScreenStrategy>(),
				std::make_unique<EnemyTargetProvider>()
			);

		player->SetTargetComponent(targetComponent);
	}

	// 敵初期化
	auto* em = EnemyManager::GetInstance();
	em->RegisterType();

	for(int i = 0; i < ENEMY_COUNT; ++i)
	{
		em->CreateEnemy("Enemy");
	}

	for(auto& enemy : em->GetEnemies())
	{
		_chara.emplace_back(enemy.get());
	}

	ChangeState(GameState::World, -1);

	return true;
}
