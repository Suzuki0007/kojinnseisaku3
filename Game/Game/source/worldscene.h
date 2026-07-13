#pragma once
#include "scenebase.h"

class WorldScene : public SceneBase
{
public:
	WorldScene() = default;
	virtual ~WorldScene() = default;

	virtual void Update() override;
	virtual void Render() override;
};

