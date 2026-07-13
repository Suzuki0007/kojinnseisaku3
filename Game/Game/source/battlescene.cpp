#include "pch.h"
#include "battlescene.h"
#include "playerbase.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "attackaction.h"
#include "modegame.h"

IBattleReceiver* BattleScene::GetPlayer() 
{
	if(!_battlePlayers.empty())
	{
		return _battlePlayers.front();
	}

	auto& player = PlayerManager::GetInstance()->GetPlayer();
	if(!player.empty())
	{
		return player.front().get();
	}
	return nullptr;
}

IBattleReceiver* BattleScene::GetEnemy() const
{
	if(!_battleEnemies.empty())
	{
		return _battleEnemies.front().get();
	}

	auto& enemy = EnemyManager::GetInstance()->GetEnemies();
	if(!enemy.empty())
	{
		return enemy.front().get();
	}
	return nullptr;
}

void BattleScene::Initialize()
{
	_battleCamera.Initialize();
	_battleCamera._v_pos = v::VGet(0.0f, 210.0f, 620.0f);
	_battleCamera._v_target = v::VGet(0.0f, 70.0f, 0.0f);
	_battleCamera._clip_near = 2.0f;
	_battleCamera._clip_far = 5000.0f;

	_backgroundHandle = LoadGraph("res/Texture/Background.png");

	_mapRender = std::make_unique<MapRenderComponent>();
	_mapRender->Initialize();

	_actions.clear();
	_actions.emplace_back(std::make_unique<AttackAction>());

	CreateBattleMembers();
}

void BattleScene::Update()
{
	InputDevice& input = InputLocator::Get();
	//input.Update();

	UpdateBattleMembers();
	if(_mapRender)
	{
		_mapRender->Process();
	}

	if(input.IsTrigger(InputButton::SceneDebug))
	{
		if(_observer)
		{
			_observer->OnChangeState(GameState::World, _targetEnemyId);
			auto& player = PlayerManager::GetInstance()->GetPlayer();
			for(auto& p : player)
			{
				p->SetCanControl(true);
			}
		}


		return;
	}


	auto* modegame = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"));
	if(modegame)
	{
		lua_State* L = modegame->GetLuaState();
		if(L)
		{
			lua_getglobal(L, "UpdateOrderAnimation");
			lua_pushnumber(L, 1.0f / 60.0f); // deltaTime (1フレーム分)

			if(lua_pcall(L, 1, 0, 0) != LUA_OK)
			{
				_LOG(lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		}
	}

	if(_isWorldScene)
	{
		if(input.IsTrigger(InputButton::Decide))
		{

			_observer->OnChangeState(GameState::World, _targetEnemyId);
			auto& player = PlayerManager::GetInstance()->GetPlayer();
			for(auto& p : player)
			{
				p->SetCanControl(true);
				p->SetPos(p->GetOldPos());
				p->SetDir(p->GetOldDir());
				p->SetStatus(CharaBase::STATUS::WAIT);
			}
			return;
		}
	}

	if(IsActionRunning())
	{
		//return;
	}

	// コマンドメニューのカーソル移動
	if(input.IsTrigger(InputButton::CommandUp))
	{
		_commandMenu.MoveUp();
	}
	if(input.IsTrigger(InputButton::CommandDown))
	{
		_commandMenu.MoveDown();
	}

	// 決定
	if(input.IsTrigger(InputButton::Decide))
	{
		OnCommandSelected(_commandMenu.GetSelectedCommand());
	}

}

void BattleScene::OnCommandSelected(BattleCommandMenu::BattleCommand command)
{
	switch(command)
	{
	case BattleCommandMenu::BattleCommand::Fight:
		// たたかうコマンドが選択されたら、既存の攻撃アクション経由で再生する
		if(!_actions.empty())
		{
			_actions.front()->Execute(GetPlayer(), GetEnemy());

			auto* modegame = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"));
			if(modegame)
			{
				lua_State* L = modegame->GetLuaState();
				if(L)
				{
					lua_getglobal(L, "CalculateActionOrder");
					lua_newtable(L);

					int listTableIdx = lua_gettop(L);

					int index = 1;

					for(auto& player : _battlePlayers)
					{
						if(!player) { continue; }

						lua_pushinteger(L, index); // 大元テーブル用のキー
						lua_newtable(L);           // 子テーブル
						int childTableIdx = lua_gettop(L); // 子テーブルの絶対位置を記憶

						// type の設定
						lua_pushstring(L, "type");
						lua_pushinteger(L, 0); // 0 = Player
						lua_settable(L, childTableIdx); // 子テーブルに type を設定

						// id の設定
						lua_pushstring(L, "id");
						lua_pushinteger(L, player->GetCharaId());
						lua_settable(L, childTableIdx);

						// 大元テーブル（listTableIdx）に、キー（index）と子テーブルを紐付け
						lua_settable(L, listTableIdx);
						index++;
					}

					for(auto& enemy : _battleEnemies)
					{
						if(!enemy) continue; // 安全対策

						lua_pushinteger(L, index); // 大元テーブル用のキー
						lua_newtable(L);           // 子テーブル
						int childTableIdx = lua_gettop(L);

						// type の設定
						lua_pushstring(L, "type");
						lua_pushinteger(L, 1); // 1 = Enemy
						lua_settable(L, childTableIdx);

						// id の設定
						lua_pushstring(L, "id");
						lua_pushinteger(L, enemy->GetCharaId());
						lua_settable(L, childTableIdx);

						// 大元テーブルに紐付け
						lua_settable(L, listTableIdx);
						index++;
					}

					if(lua_pcall(L, 1, 0, 0) != LUA_OK)
					{
						_LOG(lua_tostring(L, -1));
						lua_pop(L, 1);
					}
				}
			}

		}
		break;
	case BattleCommandMenu::BattleCommand::End:
		if(_observer)
		{
			_observer->OnChangeState(GameState::World, _targetEnemyId);
		}
		break;
	default:
		break;
	}
}

void BattleScene::Render()
{
	ClearDrawScreen();

	SetCameraPositionAndTarget_UpVecY(
		VC::VecToDxLib(_battleCamera._v_pos),
		VC::VecToDxLib(_battleCamera._v_target));
	SetCameraNearFar(_battleCamera._clip_near, _battleCamera._clip_far);

	RenderBattleMembers();

	if(_mapRender)
	{
		_mapRender->Render();
	}

	SetFontSize(32);
	DrawFormatString(80, 60, GetColor(255, 255, 255), "コマンドバトル");
	DrawFormatString(80, 105, GetColor(255, 255, 255), "敵（No.%d）に遭遇した！", _targetEnemyId);
	DrawFormatString(80, 150, GetColor(255, 255, 255), "Aで決定、十字キーでカーソル移動");
	// コマンド選択メニューを描画
	_commandMenu.Render(80, 540, 42);
	SetFontSize(16);

	auto* modegame = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"));
	if(modegame)
	{
		lua_State* L = modegame->GetLuaState();
		if(L)
		{
			lua_getglobal(L, "DrawActionOrderList");
			lua_pushinteger(L, 300); // 表示位置 X (他のUIと被らないように右にずらしました)
			lua_pushinteger(L, 150); // 表示位置 Y

			if(lua_pcall(L, 2, 0, 0) != LUA_OK)
			{
				_LOG(lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		}
	}

	if(modegame)
	{
		lua_State* L = modegame->GetLuaState();

		lua_getglobal(L, "IsActionOrderFinished");
		if(lua_pcall(L, 0, 1, 0) == LUA_OK)
		{
			bool finished = lua_toboolean(L, -1);
			lua_pop(L, 1);

			if(finished)
			{
				DrawFormatString(80, 300, GetColor(255, 255, 0), "アクションが終了しました。AボタンでWorldに戻る");
				_isWorldScene = true;
				return;
			}
		}
	}
}

void BattleScene::CreateBattleMembers()
{
	_battlePlayers.clear();
	_battleEnemies.clear();

	const Vec4 playerPositions[] =
	{
		v::VGet(-170.0f, 0.0f, 220.0f),
		v::VGet(0.0f, 0.0f, 260.0f),
		v::VGet(170.0f, 0.0f, 220.0f),
	};
	const Vec4 enemyPositions[] =
	{
		v::VGet(-170.0f, 0.0f, -160.0f),
		v::VGet(0.0f, 0.0f, -210.0f),
		v::VGet(170.0f, 0.0f, -160.0f),
	};

	auto& players = PlayerManager::GetInstance()->GetPlayer();

	for(int i = 0; i < 3; i++)
	{
		// プレイヤーの配置（万が一マネージャーに3人いなくてもエラーにならないようにif文をつける）
		if(i < players.size())
		{
			auto player = players[i].get();
			player->Initialize();
			player->SetCamera(&_battleCamera);
			player->SetPos(playerPositions[i]);
			player->SetDir(v::VGet(0.0f, 0.0f, -1.0f));
			player->SetCanControl(false);
			_battlePlayers.emplace_back(player);
		}

		// 敵の生成と配置
		auto enemy = std::make_unique<Enemy>();
		enemy->Initialize();
		enemy->SetPos(enemyPositions[i]);
		enemy->SetDir(v::VGet(0.0f, 0.0f, 1.0f));
		_battleEnemies.emplace_back(std::move(enemy));
	}


}

void BattleScene::UpdateBattleMembers()
{
	for(auto& player : _battlePlayers)
	{
		player->Process();
	}
	for(auto& enemy : _battleEnemies)
	{
		enemy->Process();
	}
}

void BattleScene::RenderBattleMembers()
{
	for(auto& enemy : _battleEnemies)
	{
		enemy->Render();
	}
	for(auto& player : _battlePlayers)
	{
		player->Render();
	}
}

bool BattleScene::IsActionRunning() const
{
	for(const auto& player : _battlePlayers)
	{
		if(player->IsExceutionAction())
		{
			return true;
		}
	}
	for(const auto& enemy : _battleEnemies)
	{
		if(enemy->IsExceutionAction())
		{
			return true;
		}
	}
	return false;
}
