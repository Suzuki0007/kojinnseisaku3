#include "pch.h"
#include "player.h"
#include "resourcepath.h"
#include "statemanager.h"
#include "jumpcomponet.h"
#include "rollcomponent.h"
#include "dashcomponent.h"

// プレイヤーの移動
bool Player::PlayerMove(Vec4 v)
{
	_pos.x += v.x;
	_pos.z += v.z;
	return true;
}

// 初期化
bool Player::Initialize()
{
	if(!base::Initialize()) { return false; }
	_handle = Load::LoadModel(path::Player("Player"));
	// ステータスを「無し」に設定
	_status = STATUS::NONE;
	// 位置、向きの初期化
	_pos = v::VGet(0.0f, 0.0f, 0.0f); // 初期位置が同じだが、押し出され処理のおかげで位置がずれる
	_dir = v::VGet(0.0f, 0.0f, -1.0f);// キャラモデルはデフォルトで-Z方向を向いている
	// 腰位置の設定
	_col_sub_y = 40.0f;
	// コリジョン半径の設定
	_collision_r = 30.0f;
	_collision_weight = 20.0f;
	_cam = nullptr;
	_mv_speed = 6.0f;

	_air_control = 1.0f;
	_battleSpeed = 5.0f;
	_speed->SetSpeed(_battleSpeed);
	_charaId = 1;

	_attack = AddComponent<AttackComponent>();
	_attack->SetUpAttackCapusule(_handle, _attack_collision, "Player");

	_anim = AddComponent<AnimationComponent>();
	_anim->SetAnimation(
		{
			AnimationClip(""),                                          // NONE
			//AnimtionClip(アニメーション名, ループするか, 再生速度, 再生速度のばらつき, 開始オフセットの最大値)
			AnimationClip("mot_attack_charge_loop", true, 1.0f, 0.1f, 30), // WAIT
			AnimationClip("mot_move_run"),                              // WALK
			AnimationClip("mot_move_jump_f_start", false, 0.4f),              // JUMP
			AnimationClip("mot_move_jump_f_downloop"),                  // FALL
			AnimationClip("mot_attack_nomal", false, 2.0f),             // ATTACK
			AnimationClip("mot_move_land", false),                      // LANDING
			AnimationClip("mot_attack_charge_step", false),             // DASHING
			AnimationClip("mot_move_jump_f_uploop", false),             // ROLLING
		});

	_anim->ChangeAnimation(AnimationComponent::Anim::WAIT);

	return true;
}

// 終了
bool Player::Terminate()
{
	base::Terminate();
	return true;
}

void Player::CommandAttack(IBattleReceiver* target)
{
	if(_status != STATUS::ATTACK)
	{
		_status = STATUS::ATTACK;
		//Attack();
	}
}

bool Player::IsExceutionAction() const
{
	return _status == STATUS::WAIT;
}

Vec4 Player::MoveVector(int key)
{
	Vec4 v = v::VGet(0.0f, 0.0f, 0.0f);
	float length = 0.0f;

	float sx = _cam->_v_pos.x - _cam->_v_target.x;
	float sz = _cam->_v_pos.z - _cam->_v_target.z;
	float camrad = atan2(sz, sx);
	float rad = 0.0f;

	InputDevice& input = InputLocator::Get();

	if(input.IsPress(InputButton::StickDown))
	{
		v.x = 1;
	}
	if(input.IsPress(InputButton::StickUp))
	{
		v.x = -1;
	}
	if(input.IsPress(InputButton::StickLeft))
	{
		v.z = -1;
	}
	if(input.IsPress(InputButton::StickRight))
	{
		v.z = 1;
	}

	_input_v = v;

	if(v::VSize(v) > 0.0f)
	{
		length = _mv_speed;
	}
	rad = atan2(v.z, v.x);
	v.x = cos(rad + camrad) * length;
	v.z = sin(rad + camrad) * length;

	return v;
}

void Player::CheckActionInput(int trg, const Vec4& v)
{
	// ジャンプ入力
	if(trg & PAD_INPUT_1)
	{
		if(_jump->IsGround())
		{
			// ジャンプ
			_jump->RequestJump();
		}
		else
		{
			// 空中ダッシュ
			if(!_dash->IsDashing())
			{
				_dash->RequestDash(v, _dir);
			}
		}
	}

	// ドッジロール
	if(_jump->IsGround() && (trg & PAD_INPUT_3) && !_roll->IsRolling() && !_dash->IsDashing())
	{
		_roll->RequestRoll(v, _dir);
	}

	// 攻撃入力
	if(trg & PAD_INPUT_2)
	{
		if(_status != STATUS::ATTACK)
		{
			_attack->RequestAttack();
		}
	}
}

void Player::ExcecuteMovement(const Vec4& v, CharaBase::STATUS oldStatus)
{
	if(_status == STATUS::ATTACK)
	{
		_attack->Update(1.0f);
	}
	else if(_jump->IsJumping())
	{
		if(_pos.y <= 0.0f)
		{
			_pos.y = 0.0f;
			_jump->SetGround(true);
			_dash->CancelDash();
			_attack->ResetAirAttack();
		}

		// ダッシュ処理
		if(_dash->IsDashing())
		{
			_dash->Update(1.0f);
		}
		else
		{
			// 空中通常移動（v はワールド移動量）
			if(v::VSize(v) > 0.0f)
			{
				_pos.x += v.x * _air_control;
				_pos.z += v.z * _air_control;
				// 軸ロック中は向きを固定
				_dir = v;
			}
		}
	}
	else
	{
		if(_roll->IsRolling())
		{
			_roll->Update(1.0f);
		}
		else
		{
			// 地上移動（v はワールド移動量）
			if(v::VSize(v) > 0.0f && _status != STATUS::ATTACK)
			{
				_dir = v;

				_status = STATUS::WALK;
				PlayerMove(v);
			}
			else
			{
				_status = STATUS::WAIT;
				if(oldStatus == STATUS::FALL)
				{
					_status = STATUS::LANDING;
				}
				else if(oldStatus == STATUS::LANDING && !_anim->IsAnimationEnd())
				{
					_status = STATUS::LANDING; // まだ再生中なら継続
				}
				else
				{
					_status = STATUS::WAIT; // 再生終了後は待機状態に戻す
				}
			}
		}
	}
}

void Player::Targeting(InputDevice& input)
{
	if(_targetComponent)
	{
		_targetComponent->SetCamera(_cam);
		_targetComponent->RefreshCandidate();

		if(!_targetComponent->HasTarget())
		{
			_targetComponent->AcquireTarget();

		}

		if(input.IsTrigger(InputButton::LeftTarget))
		{
			_targetComponent->CycleTarget(TargetComponent::CycleDirection::Left);
		}

		if(input.IsTrigger(InputButton::RightTarget))
		{
			_targetComponent->CycleTarget(TargetComponent::CycleDirection::Right);
		}
	}
}

void Player::UpdateBattle()
{
	if(_status == STATUS::NONE)
	{
		_status = STATUS::WAIT;
	}

	if(_status == STATUS::ATTACK)
	{
		if(_anim->IsAnimationEnd())
		{
			_status = STATUS::WAIT;
		}
	}
}

// 計算処理
bool Player::Process()
{
	base::Process();
	int key = ApplicationBase::GetInstance()->GetKey();
	int trg = ApplicationBase::GetInstance()->GetTrg();

	InputDevice& input = InputLocator::Get();
	input.Update();

	// 処理前の位置を保存
	_oldPos = _pos;
	_oldDir = _dir;

	// 処理前のステータスを保存しておく
	CharaBase::STATUS old_status = _status;

	if(_status != STATUS::ATTACK)
	{
		_jump->Update(1.0f);
	}

	Vec4 moveVector = v::VGet(0.0f, 0.0f, 0.0f);

	// 移動処理
	if(_canControl)
	{
		moveVector = MoveVector(key);
		CheckActionInput(trg, moveVector);
		ExcecuteMovement(moveVector, old_status);
	}
	else
	{
		UpdateBattle();
	}

	Targeting(input);

	//ChangeAnim(old_status);
	_anim->Update(1.0f);
	
	return true;
}

// 描画処理
bool Player::Render()
{
    base::Render();

	AnimationRender(_handle, _pos, _dir);

	if(_targetComponent)
	{
		_targetComponent->Render();
	}
	if(_cam)
	{
		_cam->DrawDebugFov();
	}

	//DrawFormatString(10, 10, GetColor(255, 255, 255), "Player Pos: (%.2f, %.2f, %.2f)", _pos.x, _pos.y, _pos.z);

    return true;

}

