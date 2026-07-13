#include "pch.h"
#include "Player3.h"
#include "resourcepath.h"

bool Player3::Initialize()
{
	if(!base::Initialize()) { return false; }
	_handle = Load::LoadModel(path::Player("Player"));
	_attach_index = -1;
	// ステータスを「無し」に設定
	_status = STATUS::NONE;
	// 再生時間の初期化
	_total_time = 0.0f;
	_play_time = 0.0f;
	// 位置、向きの初期化
	_pos = v::VGet(0.0f, 0.0f, 0.0f);
	_dir = v::VGet(0.0f, 0.0f, -1.0f);// キャラモデルはデフォルトで-Z方向を向いている
	// 腰位置の設定
	_col_sub_y = 40.0f;
	// コリジョン半径の設定
	_collision_r = 30.0f;
	_collision_weight = 10.0f;
	_hp = 100;
	_battleSpeed = 20.0f;
	_speed->SetSpeed(_battleSpeed);
	_charaId = 3;
	return true;
}

bool Player3::Terminate()
{
	base::Terminate();
	return true;
}

void Player3::CommandAttack(IBattleReceiver* target)
{
	// 攻撃コマンドの実行
}

bool Player3::IsExceutionAction() const
{
	// 攻撃コマンドの実行中かどうかを返す
	return false;
}

void Player3::ChangeAnim(CharaBase::STATUS next)
{
	if(next == _status)
	{
		float anim_speed = 1.0f;
		if(_status == STATUS::ATTACK)
		{
			anim_speed = 2.0f;
		}
		_play_time += anim_speed;
		switch(_status)
		{
		case STATUS::WAIT:
		{
			_play_time += (float) (rand() % 10) / 100.0f;
			break;
		}
		}
	}
	else
	{
		// アニメーションがアタッチされていたら、デタッチする
		if(_attach_index != -1)
		{
			MV1DetachAnim(_handle, _attach_index);
			_attach_index = -1;
		}
		// ステータスに応じたアニメーションをアタッチする
		switch(_status)
		{
		case STATUS::WAIT:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_attack_charge_loop"), -1, FALSE);
			break;

		case STATUS::WALK:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_run"), -1, FALSE);
			break;

		case STATUS::JUMP:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_jump_f_start"), -1, FALSE);
			break;

		case STATUS::FALL:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_jump_f_downloop"), -1, FALSE);
			break;

		case STATUS::ATTACK:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_attack_nomal"), -1, FALSE);
			break;

		case STATUS::LANDING:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_land"), -1, FALSE);
			break;

		case STATUS::DASHING:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_attack_charge_step"), -1, FALSE);
			break;

		case STATUS::ROLLING:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_jump_f_uploop"), -1, FALSE);
			break;
		}
		// アタッチしたアニメーションの総再生時間を取得する
		_total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
		// 再生時間を初期化
		_play_time = 0.0f;
		// 再生時間をランダムにずらす
		switch(_status)
		{
		case STATUS::WAIT:
		{
			_play_time += rand() % 30; // 0 ～ 29 の揺らぎ
			break;
		}
		}
	}

	if(_play_time >= _total_time)
	{
		_play_time = 0.0f;
	}
}

bool Player3::Process()
{
	base::Process();

	CharaBase::STATUS oldStatus = _status;

	if(_status == STATUS::NONE)
	{
		_status = STATUS::WAIT;
	}

	ChangeAnim(oldStatus);

	return true;
}

bool Player3::Render()
{
	base::Render();

	if(!_canControl)
	{

		if(_attach_index != -1)
		{
			MV1SetAttachAnimTime(_handle, _attach_index, _play_time);
		}

		AnimationRender(_handle, _pos, _dir);
	}


	return true;
}