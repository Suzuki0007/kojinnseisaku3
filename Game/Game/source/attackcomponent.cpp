#include "pch.h"
#include "attackcomponent.h"
#include "targetcomponent.h"
#include "animationcomponent.h"
#include "effectmanager.h"

void AttackComponent::SetUpAttackCapusule(int handle, std::vector<mymath::ATTACKCOLLISION>& attackCollisionList, std::string attackChara)
{
	_handle = handle;
	_attackCollisionList = &attackCollisionList;
	_attackChara = attackChara;
}

void AttackComponent::ExecuteAttack()
{
	if(!_attackCollisionList)
	{
		return;
	}

	mymath::ATTACKCOLLISION attackCollision;

	attackCollision.capsule.underpos = attack::ATTACK_CAPSULE_UNDER_POS; // カプセルの下位置(剣の根元)
	attackCollision.capsule.overpos = attack::ATTACK_CAPSULE_OVER_POS;  // カプセルの上位置(剣の先端)
	attackCollision.capsule.r = attack::ATTACK_CAPSULE_RADIUS;
	attackCollision.capsule.modelhandle = _handle;
	attackCollision.capsule.framenum = MV1SearchFrame(_handle, "Character1_RightHandMiddle1"); // 右手中指先端に追従

	attackCollision.waittime = attack::ATTACK_CAPSULE_WAIT_TIME;
	attackCollision.activetime = attack::ATTACK_CAPSULE_ACTIVE_TIME;
	attackCollision.damage = attack::ATTACK_CAPSULE_DAMAGE;
	attackCollision.follow = true;
	attackCollision.attackChara = _attackChara;
	attackCollision.isHit = false;

	_attackCollisionList->push_back(attackCollision);
}

void AttackComponent::Update(float deltaTime)
{
	if(_owner->GetStatus() != STA::ATTACK)
	{
		return;
	}

	// 攻撃エフェクトの位置を更新
	if(_kickEffect)
	{
		_kickEffect->SetEffectPos(GetBackPos());
	}

	TargetComponent* target = _owner->GetTargetComponent();

	// 攻撃対象がいない場合は待機状態に戻す
	if(!(target && target->HasTarget()))
	{
		if(_kickEffect)
		{
			_kickEffect->EndAttack();
			_kickEffect = nullptr;
		}

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
				ExecuteAttack(); // 攻撃カプセル生成処理を実行
				_pendingAttack = false; // 攻撃保留を解除
			}

			if(_kickEffect)
			{
				_kickEffect->EndAttack();
				_kickEffect = nullptr;
			}

			_owner->SetStatus(STA::WAIT);
		}
	}
	else
	{
		AnimationComponent* anim = _owner->GetAnimComponent();

		// 攻撃対象がいない場合は攻撃アニメーションの再生中に前進する
		if(anim && anim->GetAnimPlayTime() < anim->GetAnimTotalTime() )
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

Vec4 AttackComponent::GetBackPos() const
{
	Vec4 dir = v::VNorm(_owner->GetDir());

	return  v::VAdd(_owner->GetPos(), v::VGet(-dir.x * attack::BACK_POS_OFFSET, attack::BACK_POS_HEIGHT, -dir.z * attack::BACK_POS_OFFSET));
}

void AttackComponent::RequestAttack()
{
	TargetComponent* target = _owner->GetTargetComponent();
	
	// 攻撃対象がいない場合は攻撃を保留
	if(!target || !target->HasTarget())
	{
		_pendingAttack = false;

		if(_owner->GetStatus() == STA::ATTACK)
		{
			if(_kickEffect)
			{
				_kickEffect->EndAttack();
				_kickEffect = nullptr;
			}

			_owner->SetStatus(STA::WAIT);
		}

		return;
	}

	if(_owner->GetStatus() != STA::ATTACK)
	{
		TargetComponent* target = _owner->GetTargetComponent();
		if(target && target->HasTarget())
		{
			_owner->SetDir(target->FaceTarget(_owner->GetDir()));// 攻撃対象の方向を向く
		}

		bool canAttack = false;

		// 地上にいる場合は攻撃状態に遷移する
		if(_owner->IsGround())
		{
			canAttack = true;
		}
		else if(!_airAttackUsed)
		{
			// 空中にいる場合は空中攻撃を使用済みにして攻撃状態に遷移する
			_airAttackUsed = true;
			canAttack = true;
		}

		_owner->SetStatus(STA::ATTACK);
		_pendingAttack = true; // 攻撃保留を解除

		_kickEffect = EffectManager::GetInstance()->CreateEffect(
			"KickEffect",
			GetBackPos(),
			_owner->GetDir(),
			attack::APPROACH_EPSILON);

	}
	else
	{
		_pendingAttack = true; // 攻撃を保留

		AnimationComponent* anim = _owner->GetAnimComponent();
		if(anim)
		{
			anim->ChangeAnimation(CharaBase::STATUS::ATTACK, true); // 攻撃アニメーションを強制的に再生
		}
	}
}