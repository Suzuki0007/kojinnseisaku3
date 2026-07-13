#pragma once
#include "scenebase.h"
#include "battleaction.h"
#include "battlereceiver.h"
#include "battlecommandmenu.h"
#include "maprendercomponent.h"
#include "camera.h"
#include "playerbase.h"
#include "enemy.h"
#include "lua.hpp"
#include "lua_cmd.h"

class ModeGame;

class BattleScene : public SceneBase
{
public:
	BattleScene() = default;
	virtual ~BattleScene() = default;
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	void SetUp(int enemyId) { _targetEnemyId = enemyId; }
	auto GetTargetEnemyId() const { return _targetEnemyId; }


private:
	std::vector<std::unique_ptr<BattleAction>> _actions;
	int _targetEnemyId { -1 };
	int _commandCursor{ 0 };
	bool _isWorldScene{ false };

	BattleCommandMenu _commandMenu;
	Camera _battleCamera;
	int _backgroundHandle{ -1 };
	std::vector<PlayerBase*>_battlePlayers;
	std::vector<std::unique_ptr<Enemy>> _battleEnemies;
	std::unique_ptr<MapRenderComponent> _mapRender;;

	void OnCommandSelected(BattleCommandMenu::BattleCommand command);
	void CreateBattleMembers();
	void UpdateBattleMembers();
	void RenderBattleMembers();
	bool IsActionRunning() const;

	IBattleReceiver* GetPlayer();
	IBattleReceiver* GetEnemy() const;

	lua_State* L{ nullptr }; // Luaの状態を保持するメンバ変数
};

