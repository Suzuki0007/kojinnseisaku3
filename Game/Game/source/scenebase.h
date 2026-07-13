#pragma once
#include "gameobserver.h"

class SceneBase
{
public:

	virtual ~SceneBase() {};
	virtual void Initialize() {};
	void RegisterObserver(GameObserver* observer) { _observer = observer; }
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	GameObserver* _observer{ nullptr };
};

