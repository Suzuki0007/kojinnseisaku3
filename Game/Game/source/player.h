#pragma once

#include "playerbase.h"
#include "camera.h"
#include "targetcomponent.h"
#include "attackcomponent.h"
#include "hangtimecomponent.h"

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
	void UpdateJump();
	void CancelAttackCube() override;
	void RotateToward(const Vec4& targetDir, float maxAngle);

	void Targeting(InputDevice& input);

protected:
	Camera* _cam;

	AttackComponent* _attack;
	HangTimeComponent* _hangtime;

	float _air_control;			// 空中制御係数
};

