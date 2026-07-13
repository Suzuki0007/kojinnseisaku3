#pragma once

enum class GameState
{
	None,
	World,
	Battle,
};

class GameObserver
{
public:

	GameObserver() = default;
	virtual ~GameObserver() = default;
	
	virtual void OnChangeState(GameState state, int enemyId) = 0;
};

