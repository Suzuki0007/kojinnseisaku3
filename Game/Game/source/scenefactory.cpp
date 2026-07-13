#include "pch.h"
#include "scenefactory.h"

#include "worldscene.h"
#include "battlescene.h"

std::unique_ptr<SceneBase> SceneFactory::CreateScene(GameState state, int enemyId)
{
	switch(state)
	{
		case GameState::World:
		{
			return std::make_unique<WorldScene>();
		}
		case GameState::Battle:
		{
			auto battleScene = std::make_unique<BattleScene>();
			battleScene->SetUp(enemyId);
			return battleScene;
		}
		default:
		{
			return nullptr;
		}
	}
}
