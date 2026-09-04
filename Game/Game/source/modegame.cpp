#include "pch.h"
#include "modegame.h"
#include "applicationmain.h"
#include "modeeffekseer.h"
#include "modetitle.h"
#include "scenefactory.h"
#include "shadowmapcomponent.h"
#include "modegameover.h"
#include "PlayerManager.h"
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

	_sceneScreenHandle = MakeScreen(
		ApplicationMain::GetInstance()->DispSizeW(),
		ApplicationMain::GetInstance()->DispSizeH(),
		TRUE
	);

	if(_sceneScreenHandle == -1)
	{
		return false;
	}

	_distortionPixelShaderHandle = LoadPixelShader("distortion.cso");

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

	_map->SetCamera(_camera);

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

	auto* player = GetPlayer();
	player->SetCamera(_camera);

	std::vector<Vec4> cube_positions =
	{
		v::VGet(-1204.0f, 0.0f, 1700.0f),
		v::VGet(-1500.0f, 250.0f, -1600.0f),
		v::VGet(-950.0f, 0.0f, -1600.0f),
		v::VGet(-1200.0f, 0.0f, 1700.0f),
		v::VGet(1070.0f, 0.0f, -35.0f),
		v::VGet(750.0f, 200.0f, -35.0f),
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
		v::VGet(650.0f,400.0f, 1500.0f),
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

	TimeManager::GetInstance()->Start();

	return true;
}

// 終了
bool ModeGame::Terminate()
{
	base::Terminate();

	if(_sceneScreenHandle != -1)
	{
		DeleteGraph(_sceneScreenHandle);
		_sceneScreenHandle = -1;
	}


	// キャラクターを終了
	for(auto& chara : _chara)
	{
		if(chara)
		{
			chara->Terminate();
		}
	}
	_chara.clear();

	// PlayerManagerのオブジェクトを削除
	auto* playerManager = PlayerManager::GetInstance();
	playerManager->ClearPlayer();

	// EnemyManagerのオブジェクトを削除
	auto* enemyManager = EnemyManager::GetInstance();
	enemyManager->GetEnemies().clear();

	// その他のオブジェクトを終了
	for(auto& object : _object)
	{
		if(object)
		{
			object->Terminate();
		}
	}
	_object.clear();

	_cube.clear();
	_map.reset();
	_skySphere.reset();
	_goal.reset();

	delete _camera;
	_camera = nullptr;

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

//void ModeGame::OnChangeState(GameState state, int enemyId)
//{
//	ChangeState(state, enemyId);
//}

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
	auto* player = GetPlayer();

	if(!player || !_camera)
	{
		return false;
	}

	bool isAttacking = player->GetStatus() == CharaBase::STATUS::ATTACK;

	float deltaTime = TimeManager::GetInstance()->GetDeltaTime();

	// 攻撃開始時のカメラ注視点を保存
	if(isAttacking && !_wasCameraAttack)
	{
		_cameraAttackTime = 0.0f;
		_cameraAttackStartTarget = _camera->_v_target;
	}

	Vec4 playerTarget = player->GetPos();
	playerTarget.y += camera::PLAYER_TARGET;

	if(isAttacking)
	{
		_cameraAttackTime += deltaTime;

		// 攻撃開始から1秒かけて戻す
		float progress = _cameraAttackTime / camera::CAMERA_ATTACK_TIME;

		if(progress > 1.0f)
		{
			progress = 1.0f;
		}

		// 早めに移動し、最後はゆっくり定位置へ近づく
		const float easedProgress =
			1.0f - (1.0f - progress) * (1.0f - progress) * (1.0f - progress);

		const Vec4 targetDelta =
			v::VSub(playerTarget, _cameraAttackStartTarget);

		const Vec4 smoothedTarget =
			v::VAdd(
				_cameraAttackStartTarget,
				v::VScale(targetDelta, easedProgress)
			);

		_camera->SetTargetPosition(smoothedTarget);
	}
	else
	{
		_cameraAttackTime = 0.0f;

		// 攻撃していないときは通常位置へ追従
		_camera->SetTargetPosition(playerTarget);
	}

	_wasCameraAttack = isAttacking;

	return true;
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
	float deltaTime = 1.0f; // 1フレームの時間
	AnimationManager::GetInstance()->Update(deltaTime);

	EffekseerManager::GetInstance()->Update();

	TimeManager::GetInstance()->Update();

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

	// ヒットストップ更新
	if(_hitStopManager)
	{
		_hitStopManager->Update();
		if(_hitStopManager->IsStopping())
		{
			return true; // ヒットストップ中はこれ以降の処理を一切やらない
		}
	}

	//if(_gameState == GameState::World)
	//{
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

		if(player)
		{
			const bool isAttacking =
				player->GetStatus() == CharaBase::STATUS::ATTACK;

			Vec4 playerMove =
				v::VSub(
					player->GetPos(),
					player->GetOldPos()
				);

			const bool isMoving =
				playerMove.x != 0.0f ||
				playerMove.y != 0.0f ||
				playerMove.z != 0.0f;

			const bool isAttackMoving =
				isAttacking && isMoving;

			if(isAttackMoving)
			{
				_screenMotionBlur.Start();
			}
			else
			{
				_screenMotionBlur.Stop();
			}
		}

		for(auto& object : _object)
		{
			object->Process();
		}
		for(auto& effect : EffectManager::GetInstance()->GetEffects())
		{
			if(effect)
			{
				effect->Process();
			}
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

		if(_goal && CharaToCharaCollision(player, _goal.get()))
		{
			if(ModeServer::GetInstance()->Get("gameover") == nullptr)
			{
				ModeServer::GetInstance()->Add(
					new ModeGameOver(),
					2,
					"gameover");
			}

			// 現在のModeGameを削除予約
			ModeServer::GetInstance()->Del(this);
		}

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
	//}
	if(_camera)
	{
		_camera->Process();

		if(_cameraShakeManager)
		{
			_cameraShakeManager->Update();
			_camera->ApplyShakeOffset(_cameraShakeManager->GetOffset());
		}
	}

	DebugProcess();

	return true;
}

void ModeGame::RenderShadowCaster()
{
	if(_map && _map->GetHandleMap() != -1)
	{
		MV1DrawModel(_map->GetHandleMap());
	}


	// プレイヤー・敵
	RenderChara();

	// キューブなどにも影を落とす場合
	for(auto& object : _object)
	{
		if(object)
		{
			object->Render();
		}
	}
}

void ModeGame::RenderChara()
{
	// キャラを描画（生存しているもののみ）
	auto& enemies = GetEnemies();
	for(auto& chara : _chara)
	{
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
			if(!_enemyAliveList[enemyIndex] || !chara->IsAlive())
			{
				continue;
			}
		}
		else
		{
			if(!chara->IsAlive())
			{
				continue;
			}
		}
		chara->Render();
	}
}

void ModeGame::RenderNormalScene()
{
	auto* shadowMap = _map->GetShadowMapComponent();

	if(shadowMap && _camera)
	{
		shadowMap->Begin(*_camera);

		RenderShadowCaster();

		shadowMap->End();
	}

	// シャドウマップ描画後に通常カメラを再設定する
	SetCameraPositionAndTarget_UpVecY(
		VC::VecToDxLib(_camera->_v_pos),
		VC::VecToDxLib(_camera->_v_target)
	);

	SetCameraNearFar(
		_camera->_clip_near,
		_camera->_clip_far
	);

	// 通常描画
	RenderChara();

	for(auto& object : _object)
	{
		if(object)
		{
			object->Render();
		}
	}

	for(auto& effect : EffectManager::GetInstance()->GetEffects())
	{
		if(effect)
		{
			effect->Render();
		}
	}

	SetUseShadowMap(0, -1);

	_skySphere->Render();
}

void DrawGraphUseMotionBlurShader(
	const int x,
	const int y,
	const int textureHandle,
	const int pixelShaderHandle
)
{
	std::array<VERTEX2DSHADER, 4> vertices{};

	for(auto& vertex : vertices)
	{
		vertex.rhw = 1.0f;
		vertex.dif = GetColorU8(255, 255, 255, 255);
		vertex.spc = GetColorU8(0, 0, 0, 0);
	}

	int graphWidth = 0;
	int graphHeight = 0;

	GetGraphSize(
		textureHandle,
		&graphWidth,
		&graphHeight
	);

	const float left =
		static_cast<float>(x);

	const float top =
		static_cast<float>(y);

	const float right =
		left + graphWidth;

	const float bottom =
		top + graphHeight;

	// 左上
	vertices[0].pos = { left, top, 0.0f };
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;
	vertices[0].su = 0.0f;
	vertices[0].sv = 0.0f;

	// 右上
	vertices[1].pos = { right, top, 0.0f };
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;
	vertices[1].su = 1.0f;
	vertices[1].sv = 0.0f;

	// 左下
	vertices[2].pos = { left, bottom, 0.0f };
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;
	vertices[2].su = 0.0f;
	vertices[2].sv = 1.0f;

	// 右下
	vertices[3].pos = { right, bottom, 0.0f };
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;
	vertices[3].su = 1.0f;
	vertices[3].sv = 1.0f;

	unsigned short indices[6] =
	{
		0, 1, 2,
		2, 1, 3
	};

	SetUseTextureToShader(0, textureHandle);
	SetUsePixelShader(pixelShaderHandle);

	DrawPolygonIndexed2DToShader(
		vertices.data(),
		static_cast<int>(vertices.size()),
		indices,
		2
	);
}

// 描画処理
bool ModeGame::Render()
{
	base::Render();


	// 画面テクスチャへ描画
	SetDrawScreen(_sceneScreenHandle);
	ClearDrawScreen();

	RenderNormalScene();

	DebugRender();// デバック描画処理

	// バックバッファへ戻す
	SetDrawScreen(DX_SCREEN_BACK);

	if(_screenMotionBlur.IsActive())
	{
		DrawGraphUseMotionBlurShader(
			0,
			0,
			_sceneScreenHandle,
			_distortionPixelShaderHandle
		);
	}
	else
	{
		DrawGraph(
			0,
			0,
			_sceneScreenHandle,
			TRUE
		);
	}

	SetUsePixelShader(-1);
	SetUseTextureToShader(0, -1);

	DrawFormatString(
		10,
		200,
		GetColor(0, 0, 0),
		"Shader Handle: %d",
		_distortionPixelShaderHandle
	);

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

	

	return true;
}