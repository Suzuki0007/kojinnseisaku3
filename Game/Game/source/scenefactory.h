#pragma once
#include "scenebase.h"

class SceneFactory
{
public:
	SceneFactory() = delete;
	virtual ~SceneFactory() = default;

	static std::unique_ptr<SceneBase> CreateScene(GameState state, int enemyId);
};

