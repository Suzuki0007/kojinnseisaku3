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
	_mv_speed = 12.0f;

	_air_control = 1.0f;
	_battleSpeed = 5.0f;
	_speed->SetSpeed(_battleSpeed);
	_charaId = 1;

	_attack = AddComponent<AttackComponent>();
	_hangtime = AddComponent<HangTimeComponent>();
	_attack->SetUpAttackCapusule(_handle, _attack_collision, "Player");

	_anim = AddComponent<AnimationComponent>();
	_anim->SetAnimation(
		{
			AnimationClip(""),                                          // NONE
			//AnimtionClip(アニメーション名, ループするか, 再生速度, 再生速度のばらつき, 開始オフセットの最大値)
			AnimationClip("mot_attack_charge_loop", true, 1.0f, 0.1f, 30), // WAIT
			AnimationClip("mot_move_run"),                              // WALK
			AnimationClip("mot_move_jump_f_start", true, 0.5f),              // JUMP
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
	if(_status != STATUS::ATTACK && (trg & PAD_INPUT_1))
	{
		if(_jump->IsGround())
		{
			// 地上ジャンプ
			_jump->RequestJump();
		}
		else if(_hangtime->IsActive())
		{
			// 攻撃終了後の滞空中はジャンプ可能
			_jump->RequestJump();
			_hangtime->Reset();
		}
		else if(!_dash->IsDashing())
		{
			// 通常の空中状態ではダッシュ
			_dash->RequestDash(v, _dir);
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
			_dash->CancelDash();
			_jump->ResetJumpState();
			_hangtime->Reset();

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
	else
	{
		UpdateJump();

		if(_jump->IsJumping())
		{
			if(_pos.y <= 0.0f)
			{
				_pos.y = 0.0f;
				_jump->SetGround(true);
				_dash->CancelDash();
				_attack->ResetAirAttack();
			}

			if(_dash->IsDashing())
			{
				_dash->Update(1.0f);
			}
			else
			{
				if(v::VSize(v) > 0.0f)
				{
					_pos.x += v.x * _air_control;
					_pos.z += v.z * _air_control;
					_dir = v;
				}
			}
		}
		else
		{
			// 既存の地上処理
			if(_roll->IsRolling())
			{
				_roll->Update(1.0f);
			}
			else
			{
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
					else if(oldStatus == STATUS::LANDING &&
						!_anim->IsAnimationEnd())
					{
						_status = STATUS::LANDING;
					}
				}
			}
		}
	}

	// 攻撃状態から抜けた直後に、空中なら滞空開始
	if(oldStatus == STATUS::ATTACK &&
		_status != STATUS::ATTACK &&
		_jump->IsJumping())
	{
		_hangtime->Start();
	}
}

void Player::UpdateJump()
{
	if(_status == STATUS::ATTACK)
	{
		return;
	}

	if(_hangtime->IsActive() && _jump->IsJumping())
	{
		_jump->UpdateWithoutFall();
	}
	else
	{
		_jump->Update(1.0f);
	}
}

void Player::Targeting(InputDevice& input)
{
	if(!_targetComponent)
	{
		return;
	}

	_targetComponent->SetCamera(_cam);

	// 攻撃中は候補更新を行わない。
	// カメラを動かしても、現在のターゲットを維持する。
	if(_status == STATUS::ATTACK)
	{
		if(!_targetComponent->HasTarget())
		{
			_targetComponent->AcquireTarget();
		}

		return;
	}

	// 攻撃していないときだけターゲット候補を更新する
	_targetComponent->RefreshCandidate();

	if(!_targetComponent->HasTarget())
	{
		_targetComponent->AcquireTarget();
	}

	if(input.IsTrigger(InputButton::LeftTarget))
	{
		_targetComponent->CycleTarget
		(
			TargetComponent::CycleDirection::Left
		);
	}

	if(input.IsTrigger(InputButton::RightTarget))
	{
		_targetComponent->CycleTarget
		(
			TargetComponent::CycleDirection::Right
		);
	}
}

void Player::CancelAttackCube()
{
	if(_status != STATUS::ATTACK)
	{
		return;
	}

	_status = STATUS::FALL;

	_attack_collision.clear();

	if(_jump && _jump->IsJumping() && _hangtime)
	{
		_hangtime->Start();
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

	Vec4 moveVector = v::VGet(0.0f, 0.0f, 0.0f);

	// 移動処理
	moveVector = MoveVector(key);
	CheckActionInput(trg, moveVector);
	ExcecuteMovement(moveVector, old_status);
	Targeting(input);

	//ChangeAnim(old_status);
	_anim->Update(1.0f);
	_hangtime->Update(1.0f);
	
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

