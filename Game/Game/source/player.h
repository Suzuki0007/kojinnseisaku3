#pragma once

#include "playerbase.h"
#include "camera.h"
#include "targetcomponent.h"

class Player : public PlayerBase
{
	using base = PlayerBase;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	virtual void CommandAttack(IBattleReceiver* target) override;

	virtual bool IsExceutionAction() const override;

	void SetCamera(Camera* cam)  override { _cam = cam; }

	// 攻撃用カプセル当たり判定
	bool AttackCapsule
	(
		Vec4 underpos,	// カプセルの下位置
		Vec4 overpos,		// カプセルの上下位置
		float r,			// 半径
		int waittime,		// 発生までの時間
		int activetime,		// 有効時間
		int timespeed,		// カプセルの伸縮速度
		bool follow,		// カプセルがキャラに追従するか		
		float damage,		// ダメージ量
		int framenum,		// ノックバックフレーム数
		Vec4 dir
	);

	bool PlayerMove(Vec4 v);

	void UpdateBattle();

	bool Attack();

	virtual const char* GetCharaClassName() const override { return "Player1"; }
	virtual float GetSpeed() const override { return _speed->GetSpeed(); }

	Vec4 MoveVector(int key);
	void CheckActionInput(int trg, const Vec4& v);
	void ExcecuteMovement(const Vec4& v, CharaBase::STATUS oldStatus);
	void ChangeAnim(CharaBase::STATUS next);

	void Targeting(InputDevice& input);

protected:
	Camera* _cam;


	bool _is_dashing;// ダッシュ中かどうか
	float _dash_speed;
	float _dash_time;
	float _dash_timer;
	Vec4 _dash_direction;

	bool _is_rolling; 
	float _roll_speed; 
	float _roll_time; 
	float _roll_timer;			// ドッジロール残り時間
	Vec4 _roll_direction;		// ドッジロール方向

	float _air_control;			// 空中制御係数

	bool _is_attack;			// 攻撃中かどうか
	bool _air_attack_used;		// 空中攻撃を使用したかどうか
	bool _pendingAttack;		// 攻撃を保留中かどうか
};

