#include "pch.h"
#include "modegame.h"
#include "applicationmain.h"
#include "modeeffekseer.h"
#include "modetitle.h"
#include "scenefactory.h"

#include "lua.hpp"

#include "tolua.h"
#include "tolua_cmd/luaglue_cmd.h"
#include "lua_cmd.h"

// luaの標準ライブラリテーブル
static const luaL_Reg loadedlibs[] =
{
	{ "_G", luaopen_base },
	{ LUA_LOADLIBNAME, luaopen_package },
	{ LUA_COLIBNAME, luaopen_coroutine },
	{ LUA_TABLIBNAME, luaopen_table },
	{ LUA_IOLIBNAME, luaopen_io },
	//  {LUA_OSLIBNAME, luaopen_os},
	{ LUA_STRLIBNAME, luaopen_string },
	{ LUA_MATHLIBNAME, luaopen_math },
	{ LUA_UTF8LIBNAME, luaopen_utf8 },
	{ LUA_DBLIBNAME, luaopen_debug },
#if defined(LUA_COMPAT_BITLIB)
	{ LUA_BITLIBNAME, luaopen_bit32 },
#endif
	{ NULL, NULL }
};

static void _luaL_openlibs(lua_State* L)
{
	const luaL_Reg* lib;
	/* "require" functions from 'loadedlibs' and set results to global table */
	for(lib = loadedlibs; lib->func; lib++)
	{
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1);  /* remove lib */
	}
}

static lua_State* _luaL_RegistCoroutine(lua_State* L, const char* funcName)
{
	// コルーチンの生成とfunctionの登録
	lua_State* co = lua_newthread(L);
	lua_getglobal(co, funcName);
	return co;
}

static void _luaL_ErrMsg(lua_State* L, int errcode)
{
	std::string luaErrMsg = lua_tostring(L, lua_gettop(L));
	MessageBox(NULL, luaErrMsg.c_str(), "lua error", MB_OK);
}

// 初期化
bool ModeGame::Initialize()
{
	if(!base::Initialize())
	{
		return false;
	}

	// luaの初期化
	_L = luaL_newstate();
	_luaL_openlibs(_L);

	// toluaでの関数の登録
	luaopen_cmd(_L);

	// luaスクリプトの登録
	{
		CFile f("res/game.lua");
		if (f.Data())
		{
			luaL_loadbuffer(_L, (const char*)f.Data(), f.Size(), "script");
			int errcode = lua_pcall(_L, 0, 0, 0);
			if (errcode != 0)
			{
				// スクリプトにエラーがある
				_luaL_ErrMsg(_L, errcode);
			}
			else
			{
				_coL = _luaL_RegistCoroutine(_L, "DrawEnemy");
			}
		}
	}

	// カメラ初期化
	_camera = new Camera();
	_camera->Initialize();

	ObjectInitialize();	// オブジェクト初期化

	// キャラ
	for(auto& chara : _chara)
	{
		chara->Initialize();
	}

	// オブジェクトの初期化
	for(auto& object : _object)
	{
		object->Initialize();
	}

	_map->SetCamera(_camera);
	auto* player = GetPlayer();
	player->SetCamera(_camera);

	std::vector<Vec4> cube_positions =
	{
		v::VGet(-1204.0f, 0.0f, 1700.0f),
		v::VGet(-1500.0f, 250.0f, -1600.0f),
		v::VGet(-950.0f, 0.0f, -1600.0f),
		v::VGet(-1200.0f, 0.0f, 1700.0f),
		v::VGet(1070.0f, 0.0f, -35.0f),
		v::VGet(6000.0f, 0.0f, 1500.0f)
	};

	for(size_t i = 0; i < _cube.size(); i++)
	{
		_cube[i]->SetPosition(cube_positions[i]);
		_cube[i]->UpdateAABB();
	}

	std::vector<Vec4> enemy_positions =
	{
		v::VGet(-1350.0f, 400.0f, -1600.0f),
		v::VGet(650.0f,0.0f, 1500.0f),
		v::VGet(1100.0f, 0.0f, 150.0f),
		v::VGet(-1200.0f,100.0f, 1700.0f),
		v::VGet(150.0f, 0.0f, -3700.0f),
		v::VGet(6000.0f, 100.0f, 1500.0f)
	};

	auto& enemy = GetEnemies();
	for(size_t i = 0; i < enemy_positions.size(); ++i)
	{
		enemy[i]->SetPos(enemy_positions[i]);
	}

	DebugInitialize();// デバック初期化

	_resolve_on_y = false;
	_landed_on_up = false;

	_time_limit = 60; // 制限時間1分
	_is_gameover = false;
	// 終了時の残り時間を未設定にする
	_final_remaining_time = -1;
	// _enemy_count を初期の総数として保持（倒した数は動的に計算する）
	auto& enemies = EnemyManager::GetInstance()->GetEnemies();
	_enemy_count = static_cast<int>(enemies.size());

	_enemyAliveList.clear();
	_enemyAliveList.resize(_enemy_count, false);
	for(size_t i = 0; i < enemies.size(); ++i)
	{
		_enemyAliveList[i] = enemies[i]->IsAlive();
	}

	InputDevice& input = InputLocator::Get();
	input.Update();

	return true;
}

// 終了
bool ModeGame::Terminate()
{
	base::Terminate();
	// キャラ
	for(auto& chara : _chara)
	{
		chara->Terminate();
	}
	_chara.clear();
	for(auto& object : _object)
	{
		object->Terminate();
	}
	_object.clear();
	delete _camera;
	return true;
}

PlayerBase* ModeGame::GetPlayer() const
{
	return PlayerManager::GetInstance()->GetPlayer().front().get();
}

std::vector<std::unique_ptr<EnemyBase>>& ModeGame::GetEnemies()
{
	return EnemyManager::GetInstance()->GetEnemies();
}

void ModeGame::OnChangeState(GameState state, int enemyId)
{
	ChangeState(state, enemyId);
}

// 円同士の当たり判定
bool ModeGame::IsHitCircle(CharaBase* c1, CharaBase* c2)
{
	// 「2つの円の中心点」の距離を求める
	// ピタゴラスの定理（三平方の定理）で求めることができる
	// x,zで処理。yは判定せず
	float w, h, length;
	w = c1->GetPos().x - c2->GetPos().x;
	h = c1->GetPos().z - c2->GetPos().z;
	length = static_cast<float>(sqrtf(w * w + h * h));

	// 中心点間の距離が、2つの円の半径の合計よりも小さい場合、当たり
	if(length < c1->GetCollisionR() + c2->GetCollisionR())
	{
		return true;
	}

	return false;
}

// プレイヤーのカメラ情報表示
bool ModeGame::PlayerCameraInfo()
{
	// カメラの位置/視点の移動を、プレイヤーの移動量に追従する
	auto* player = GetPlayer();
	Vec4 playermove = v::VSub(player->GetPos(), player->GetOldPos());
	// 水平移動は従来通り位置を移す
	_camera->_v_pos = v::VAdd(_camera->_v_pos, v::VGet(playermove.x, 0.0f, playermove.z));
	// 注視点はプレイヤーの現在位置を基準に高さも含めて追従させる
	Vec4 player_target = player->GetPos();
	player_target.y += 60.0f; // プレイヤーの目線高さなどのオフセット
	_camera->SetTargetPosition(player_target);
	return true;
}

void ModeGame::ChangeState(GameState nextState, int enemyId)
{
	if(_gameState == GameState::Battle && nextState == GameState::World)
	{
		// バトル終了時の処理
		if(_enemyIndexBattle >= 0 && _enemyIndexBattle < _enemyAliveList.size())
		{
			_enemyAliveList[_enemyIndexBattle] = false;
			_enemy_count--;
		}
		_enemyIndexBattle = -1;
	}
	
	if(nextState == GameState::World)
	{
		auto* player = GetPlayer();
		if(player)
		{
			player->SetCamera(_camera);
		}
	}

	_sceneBase = SceneFactory::CreateScene(nextState, enemyId);

	if(_sceneBase)
	{
		_sceneBase->RegisterObserver(this);
		_sceneBase->Initialize();
	}

	_gameState = nextState;
}

bool ModeGame::IsEnemyAliveFromList(int index) const
{
	if(index >= 0 && index < static_cast<int>(_enemyAliveList.size()))
	{
		return _enemyAliveList[index];
	}
	return false;
}

// 計算処理
bool ModeGame::Process()
{
	float deltaTime = 1.0f; // 1フレームの時間を秒単位で計算
	AnimationManager::GetInstance()->Update(deltaTime);

	int trg = ApplicationMain::GetInstance()->GetTrg();
	if(_is_gameover)
	{
		ModeServer::GetInstance()->SkipRenderUnderLayer();

		if(trg & PAD_INPUT_1)
		{
			ModeServer::GetInstance()->Add(new ModeTitle(), 0, "title");
			ModeServer::GetInstance()->Del(this);
		}
		return true; // ゲームオーバー中はこれ以降の処理を一切やらない
	}

	if(_gameState == GameState::World)
	{
		// 1. 敵の行動更新（ModeGame側の生存リストを正義にする！）
		auto& enemies = GetEnemies();
		for(size_t i = 0; i < enemies.size(); i++)
		{
			if(_enemyAliveList[i]) // 生きている敵だけ
			{
				enemies[i]->Process();
			}
		}

		// 2. プレイヤーとその他オブジェクトの行動更新
		auto* player = GetPlayer();
		if(player && player->IsAlive())
		{
			player->Process();
		}
		for(auto& object : _object)
		{
			object->Process();
		}

		CheckEncount();

		// プレイヤーとステージの判定
		_resolve_on_y = false;
		_landed_on_up = false;
		for(auto& cube : _cube)
		{
			CharaToCubeCollision(player, cube.get());
		}
		LandCheck();

		// 移動中専用のアクション・カメラ情報更新
		UpdateCheckAttackCollision();
		PlayerCameraInfo();

		// フィールド上の敵が全滅したかのチェック
		int alive_count = 0;
		for(auto& enemy : enemies)
		{
			if(enemy->IsAlive()) ++alive_count;//  生きている敵の数をカウント
		}
		if(alive_count == 0)
		{
			_is_gameover = true;
			ModeServer::GetInstance()->SkipProcessUnderLayer();

			int elapsed_sec = static_cast<int>(GetModeTm() / 1000);
			int remaining = _time_limit - elapsed_sec;
			if(remaining < 0) remaining = 0;
			if(_final_remaining_time < 0) _final_remaining_time = remaining;
		}
	}

	// ---------------------------------------------------------
	// C. 【共通】現在のサブシーン（WorldScene / BattleScene）の更新
	// ---------------------------------------------------------
	//if(_sceneBase)
	//{
	//	_sceneBase->Update();
	//}

	// ---------------------------------------------------------
	// D. 【共通】カメラや制限時間などの環境更新
	// ---------------------------------------------------------
	if(_camera)
	{
		_camera->Process();
	}



	return true;
}

// 描画処理
bool ModeGame::Render()
{
	base::Render();


	// カメラ設定更新
	SetCameraPositionAndTarget_UpVecY(VC::VecToDxLib(_camera->_v_pos), VC::VecToDxLib(_camera->_v_target));
	SetCameraNearFar(_camera->_clip_near, _camera->_clip_far);

	// キャラを描画（生存しているもののみ）
	auto& enemies = GetEnemies();
	for(auto& chara : _chara)
	{
		// 1. まず、このキャラが「敵（Enemy）」のリストに含まれているか探す
		size_t enemyIndex = -1;
		for(size_t i = 0; i < enemies.size(); i++)
		{
			if(chara == enemies[i].get()) // ポインタの住所が一致するかチェック
			{
				enemyIndex = i; // 一致したら、その敵のインデックス（番号）を保存
				break;
			}
		}
		if(enemyIndex != -1)
		{
			// このキャラは「敵」だった！
			// ModeGameの生存メモを見て、すでに倒されているなら描画をスキップする
			if(!_enemyAliveList[enemyIndex])
			{
				continue; //  ここで弾くことで、画面に映らなくなります！
			}
		}
		else
		{
			// このキャラは「プレイヤー」や「NPC」だった！
			// 敵ではないので、個別の死亡チェック（chara->IsAlive()等）だけでOK
			if(!chara->IsAlive())
			{
				continue;
			}
		}

		// 3. 生き残ったキャラだけがここでめでたく描画される
		chara->Render(); // (または Draw() )
	}

	// オブジェクトを描画
	for(auto& object : _object)
	{
		object->Render();
	}

	//_sceneBase->Render(); // ワールド中ならWorldScene、バトル中ならBattleSceneが描画される

	DebugRender();// デバック描画処理

    // 敵のHP情報を画面に表示（生存している敵のみ）と生存カウント取得
    // フォントサイズを小さくして表示する
    SetFontSize(16);
	if(_coL)
	{
		int narg = 0;
		int nresult = 0;
		int ret = lua_resume(_coL, NULL, narg, &nresult);

		if(ret != LUA_YIELD)
		{
			// コルーチンが終了している（エラーも含む）
			if(ret != 0)
			{
				_luaL_ErrMsg(_coL, ret);
			}
			_coL = NULL; // コルーチンはもう使えないので無効化
		}
	}

	 SetFontSize(64);
   
	int alive_count = 0;
	for (size_t i = 0; i < _enemyAliveList.size(); ++i)
	{
		if (_enemyAliveList[i])
		{
			alive_count++;
		}
	}

	// ゲームオーバー時の表示
	if(_is_gameover)
	{
		// 中央付近に Game Over と倒した数と残り時間を表示
		int cx = 300;
		int cy = 200;
		SetFontSize(64);
		DrawFormatString(cx + 200, cy, GetColor(0, 0, 0), "GAME OVER");
		SetFontSize(32);
		// 倒した敵数は初期総数 - 現在生存数
		int defeated = _enemy_count - alive_count;
		if(defeated < 0)
		{
			defeated = 0;
		}
		DrawFormatString(cx + 200, cy + 100, GetColor(255, 0, 0), "敵を倒した数: %d", defeated);
		// 操作説明
		DrawFormatString(cx + 50, cy + 300, GetColor(255, 0, 0), "キーボード:Zまだはパッド:Aでタイトルへ");
		// フォントサイズを戻す（他描画に影響しないように）
		SetFontSize(16);
	}

	return true;
}