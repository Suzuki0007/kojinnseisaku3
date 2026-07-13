#pragma once
#include "lua.hpp"
#include "modemenu.h"
#include "charabase.h"
#include "objectbase.h"
#include "camera.h"
#include "playerbase.h"
#include "enemy.h"
#include "map.h"
#include "cube.h"
#include "PlayerManager.h"
#include "enemymanager.h"
#include "scenebase.h"
#include "gameobserver.h"

#define CUBE_COUNT 6
#define ENEMY_COUNT 6

class ModeGame : public ModeBase, public GameObserver
{
	typedef ModeBase base;
public:
	enum class GameResult
	{
		Continue, // 今のシーンをそのまま続ける
		BattleEnd, // バトル終了
	};

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool GetDebugViewCollision() const { return _d_view_collision; }
	bool GetDebugUseCollision() const { return _d_use_collision; }
	bool GetDebugViewCameraInfo() const { return _d_view_camera_info; }
	bool GetDebugViewShadowMap() const { return _d_view_shadow_map; }
	void SetDebugViewCollsion(bool d) { this->_d_view_collision = d; }
	void SetDebugUseCollision(bool d) { this->_d_use_collision = d; }
	void SetDebugViewCameraInfo(bool d) { this->_d_view_camera_info = d; }
	void SetDebugViewShadowMap(bool d) { this->_d_view_shadow_map = d; }

	bool PushChara(CharaBase* move, CharaBase* stop);
	
	bool IsHitCircle(CharaBase* c1, CharaBase* c2);
	bool IsHitCircle(CharaBase* target)
	{
		auto* player = PlayerManager::GetInstance()->GetPlayer().front().get();
		return IsHitCircle(player, target);
	}

	// 当たり判定処理
	bool EscapeCollision();// キャラの回避処理
	bool CharaToCharaCollision(CharaBase* c1, CharaBase* c2);// キャラ同士の当たり判定処理
	bool CharaToCubeCollision(CharaBase* chara, Cube* cube);// キャラとキューブの当たり判定処理
	bool LandCheck();// 着地判定処理
	bool UpdateCheckAttackCollision();// 攻撃用当たり判定の更新処理
	bool CheckEncount();// エンカウントの判定処理

	// デバック関数
	bool DebugInitialize();
	bool DebugProcess();
	bool DebugRender();

	// カメラ情報
	bool PlayerCameraInfo();// プレイヤーのカメラ情報表示

	// オブジェクト関数
	bool ObjectInitialize();

	virtual void OnChangeState(GameState state, int enemyId) override;

	void ChangeState(GameState nextState, int enemyId);

	std::vector<std::unique_ptr<EnemyBase>>& GetEnemies();

	bool IsEnemyAliveFromList(int index) const;
	lua_State* GetLuaState() const { return _L; }

private:
	// プレイヤーのヘルパー関数
	PlayerBase* GetPlayer() const;

protected:
	Camera* _camera;

	// キャラクタ管理
		// キャラクタ管理
	std::vector<CharaBase*> _chara;
	std::vector<std::shared_ptr<ObjectBase>> _object;
	// マップ
	std::shared_ptr<Map> _map;
	// キューブ
	std::vector<std::shared_ptr<Cube>> _cube;
	// デバッグ用
	bool _d_view_collision;
	bool _d_use_collision;
	bool _d_view_camera_info;
	bool _d_view_shadow_map;

	bool _resolve_on_y;// Y方向のコリジョン解決を行うかどうか
	bool _landed_on_up;// 上方向に着地したかどうか

	int _time_limit;// 制限時間
	bool _is_gameover;// ゲームオーバーフラグ
	int _enemy_count;// 敵の総数
	std::vector<bool> _enemyAliveList;// 敵の生存フラグリスト
	std::vector<int> _enemyOrderList;// 敵の死亡順リスト
	// 全滅または時間切れになったときの残り時間を保持。未設定は -1
	int _final_remaining_time;

	std::unique_ptr<SceneBase> _sceneBase;
	GameState _gameState{ GameState::World };
	int _enemyIndexBattle{ -1 };// 現在戦っている敵の配列番号

	// lua用
	lua_State* _L;
	lua_State* _coL{ nullptr };
};

