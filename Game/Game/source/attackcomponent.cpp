#include "pch.h"
#include "attackcomponent.h"
#include "targetcomponent.h"
#include "animationcomponent.h"

void AttackComponent::Update(float deltaTime)
{
	if(_owner->GetStatus() != STA::ATTACK)
	{
		return;
	}

	TargetComponent* target = _owner->GetTargetComponent();

	// 攻撃対象がいない場合は待機状態に戻す
	if(!(target && target->HasTarget()))
	{
		_owner->SetStatus(STA::WAIT);
		return;
	}

	CharaBase* targetChara = target->GetTarget();

	// 攻撃対象が生存している場合は近づく処理を行う
	if(targetChara && targetChara->IsAlive())
	{
		// 攻撃対象の方向を向く
		Vec4 toTarget = v::VSub(targetChara->GetPos(), _owner->GetPos());
		float dist = v::VSize(toTarget);

		// 攻撃対象に近づく処理
		if(dist > attack::APPROACH_RANGE + attack::APPROACH_EPSILON)
		{
			Vec4 moveDir = v::VNorm(toTarget);

			float moveAmount = min::MyMin(dist - attack::APPROACH_RANGE, GetApproachSpeed());// 近づく距離の閾値を超えないようにする

			Vec4 pos = _owner->GetPos();
			pos.x += moveDir.x * moveAmount;
			pos.y += moveDir.y * moveAmount;
			pos.z += moveDir.z * moveAmount;
			_owner->SetPos(pos);
		}
		else
		{
			//	攻撃対象がいない場合は待機状態に戻す
			if(_pendingAttack)
			{
				if(_attackExecutor)
				{
					_attackExecutor();// 攻撃カプセル生成処理を実行
				}
				_pendingAttack = false; // 攻撃保留を解除
			}
			_owner->SetStatus(STA::WAIT);
		}
	}
	else
	{
		AnimationComponent* anim = _owner->GetAnim();

		// 攻撃対象がいない場合は攻撃アニメーションの再生中に前進する
		if(anim && anim->GetAnimPlayTime() < anim->GetAnimTotalTime())
		{
			Vec4 forward = v::VNorm(_owner->GetDir());

			Vec4 pos = _owner->GetPos();
			pos.x += forward.x * GetForwardSpeed();
			pos.z += forward.z * GetForwardSpeed();
			_owner->SetPos(pos);
		}
	}
}

float AttackComponent::GetApproachSpeed() const
{
	if(_owner->IsGround())
	{
		return attack::GROUND_APPROACH_SPEED;
	}
	else
	{
		return attack::AIR_APPROACH_SPEED;
	}
}

float AttackComponent::GetForwardSpeed() const
{
	if(_owner->IsGround())
	{
		return attack::GROUND_FORWARD_SPEED;
	}
	else
	{
		return attack::AIR_FORWARD_SPEED;
	}
}

void AttackComponent::RequestAttack()
{
	if(_owner->GetStatus() != STA::ATTACK)
	{
		TargetComponent* target = _owner->GetTargetComponent();
		if(target && target->HasTarget())
		{
			_owner->SetDir(target->FaceTarget(_owner->GetDir()));// 攻撃対象の方向を向く
		}
	}
}