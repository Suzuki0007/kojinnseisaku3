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
#include "effectmanager.h"
#include "hitstopmanager.h"
#include "camerashakemanager.h"
#include "skysphere.h"
#include "goal.h"
#include "hitinfo.h"
#include "screenmotionblur.h"
//#include "scenebase.h"
//#include "gameobserver.h"

#define CUBE_COUNT 6
#define ENEMY_COUNT 6

namespace camera
{
	static constexpr float PLAYER_TARGET = 60.0f; // プレイヤーの注視点の高さ
	static constexpr float CAMERA_ATTACK_TIME = 0.8f; // 攻撃開始からカメラが元の位置に戻るまでの時間
	static constexpr float CAMERA_ATTACK_MAX = 0.5f; // 攻撃開始時のカメラの最大移動量

	static constexpr float CAMERA_ATTACK_FOLLOW = 0.5f;
	static constexpr float CAMERA_ATTACK_MAX_DISTANCE = 100.0f;
}

class ModeGame : public ModeBase
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

	// 描画関数
	void RenderShadowCaster();
	void RenderChara();
	void RenderNormalScene();

	//virtual void OnChangeState(GameState state, int enemyId) override;

	//void ChangeState(GameState nextState, int enemyId);

	std::vector<std::unique_ptr<EnemyBase>>& GetEnemies();

	bool IsEnemyAliveFromList(int index) const;
	lua_State* GetLuaState() const { return _L; }

private:
	// プレイヤーのヘルパー関数
	PlayerBase* GetPlayer() const;

	int _sceneScreenHandle{ -1 };
	int _distortionPixelShaderHandle{ -1 };

protected:
	Camera* _camera;
	// キャラクタ管理
	std::vector<CharaBase*> _chara;
	std::vector<std::shared_ptr<ObjectBase>> _object;
	// マップ
	std::shared_ptr<MapBase> _map;
	// スカイスフィア
	std::shared_ptr<SkySphere> _skySphere;
	// ゴール
	std::shared_ptr<Goal> _goal;
	// キューブ
	std::vector<std::shared_ptr<Cube>> _cube;
	
	// ヒット情報の通知用
	Subject<HitInfo> _hitSubject;
	// カメラシェイク管理
	std::unique_ptr<CameraShakeManager> _cameraShakeManager;
	// ヒットストップ管理
	std::unique_ptr<HitStopManager> _hitStopManager;
	// 画面の歪み
	ScreenMotionBlur _screenMotionBlur;
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

	//std::unique_ptr<SceneBase> _sceneBase;
	//GameState _gameState{ GameState::World };
	int _enemyIndexBattle{ -1 };// 現在戦っている敵の配列番号

	bool _wasAttacking{ false };// 前フレームで攻撃中だったかどうかのフラグ

	Vec4 _cameraAttackTarget;
	Vec4 _cameraAttackPreviousPlayerPos;
	Vec4 _cameraAttackOffset;
	Vec4 _cameraAttackStartTarget{};
	bool _wasCameraAttack{ false };// 前フレームでカメラ攻撃中だったかどうかのフラグ
	float _cameraAttackTime{ 0.0f };// カメラ攻撃中の経過時間



	int _cubeCoyoteFrame{ 0 };// キューブのクイックタイムのフレーム数
	static constexpr int CUBE_COYOTE_MAX{ 8 };// キューブのクイックタイムの最大フレーム数

	// lua用
	lua_State* _L;
	lua_State* _coL{ nullptr };
};

