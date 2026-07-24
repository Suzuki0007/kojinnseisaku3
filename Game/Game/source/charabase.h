#pragma once
#include "objectbase.h"
#include "camera.h"
#include "battlereceiver.h"
#include "speedcomponent.h"


#include "pch.h"

class AnimationComponent;
class JumpComponent;

class CharaBase : public ObjectBase, public IComponentBindable<CharaBase>, public IBattleReceiver
{
	typedef ObjectBase base;
public:
	enum class STATUS
	{
		NONE,
		WAIT,
		WALK,
		JUMP,
		FALL,
		ATTACK,
		LANDING,
		DASHING,
		ROLLING,
		COUNT
	};
	STATUS _status;

	struct AnimConfig
	{
		std::string_view idle;
		std::string_view walk;
		std::string_view attack;
	};

	CharaBase();
	virtual ~CharaBase();

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	const Vec4& GetPos() const { return _pos; }
	const Vec4& GetOldPos() const { return _oldPos; }
	const Vec4& SetPos(const Vec4& pos) { return _pos = pos; }
	const Vec4& SetDir(const Vec4& dir) { return _dir = dir; }
	const Vec4& GetDir() const { return _dir; }
	const Vec4& GetOldDir() const { return _oldDir; }
	auto GetColSubY() const { return _col_sub_y; }
	auto GetCollisionR() const { return _collision_r; }
	auto GetCollisionWeight() const { return _collision_weight; }
	auto GetStatus() const { return _status; }
	void SetStatus(STATUS status) { _status = status; }

	bool Damage(float damage);

	float GetHP() const { return _hp; }	// HP取得用ゲッター
	bool IsAlive() const { return _is_alive; } // 生存確認用ゲッター

	auto GetCanControl() const { return _canControl; }
	void SetCanControl(bool v) { _canControl = v; }

	void AnimationRender(int handle, const Vec4& pos, const Vec4& dir);

	void SetOnAnimEndCallback(std::move_only_function<void()>&& callback)
	{
		_onAnimEnd = std::move(callback);
	}
	void ExecuteOnAnimEnd()
	{
		if(_onAnimEnd)
		{
			_onAnimEnd();
			_onAnimEnd = nullptr;
		}
	}

	AnimConfig& GetAnimConfig() { return _animConfig; }

	virtual const char* GetCharaClassName() const = 0;

	int GetCharaId() const { return _charaId; }
	void SetCharaId(int id) { _charaId = id; }
	virtual float GetSpeed() const = 0;

	// ジャンプの処理関連
	void RequestJump();// ジャンプ要求を出す
	bool IsGround() const;// 着地しているかどうか
	void NotifyLand();// 着地したことを通知する
	bool GetLand() const;
	void SetLand(bool land);
	bool IsAscending() const;

protected:
	int _attach_index;
	float _total_time;
	float _play_time;
	Vec4 _pos; // キャラの位置
	Vec4 _oldPos; // 1フレーム前のキャラの位置
	Vec4 _dir; // キャラの向き
	Vec4 _oldDir; // 1フレーム前のキャラの向き
	float _col_sub_y; // コリジョン判定用のYオフセット
	float _collision_r; // 円の半径
	float _collision_weight; // キャラの重さ

	float _hp;// キャラの体力
	bool _is_alive;// 生存フラグ
	float _battleSpeed{ 0.0f };

	int _charaId{ 0 }; // キャラクターID

	bool _canControl{ true }; // キャラの操作が可能かどうか
	std::unique_ptr<SpeedComponent<CharaBase>> _speed { std::make_unique<SpeedComponent<CharaBase>>(*this) };

	// ジャンプコンポーネントの所有権を持つ
	std::unique_ptr<JumpComponent> _jump;

	AnimConfig _animConfig; // アニメーション名の設定
	AnimationComponent* _anim;
	std::move_only_function<void()> _onAnimEnd{ nullptr }; // アニメーション終了時のコールバック関数
};

