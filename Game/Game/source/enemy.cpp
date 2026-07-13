#include "pch.h"
#include "enemy.h"

// 初期化
bool Enemy::Initialize()
{
    if(!base::Initialize()) { return false; }

    _handle = MV1LoadModel("res/PoorEnemyMelee/PoorEnemy.mv1");
    _attach_index = -1;
    // ステータスを「無し」に設定
    _status = STATUS::NONE;
    // 再生時間の初期化
    _total_time = 0.0f;
    _play_time = 0.0f;
	// 位置、向きの初期化
	_pos = v::VGet(100.0f, 0.0f, 0.0f);
	_dir = v::VGet(0.0f, 0.0f, -1.0f);// キャラモデルはデフォルトで-Z方向を向いている
	// 腰位置の設定
	_col_sub_y = 40.0f;
	// コリジョン半径の設定
	_collision_r = 30.0f;
	_collision_weight = 10.0f;

	_hp = 20;

	_battleSpeed = 12.0f;
	_speed->SetSpeed(_battleSpeed);

    // 初期アタッチは行わない。Process()でステータスに応じてアタッチする

	return true;
}

// 終了
bool Enemy::Terminate()
{
	base::Terminate();
	return true;
}

void Enemy::CommandAttack(IBattleReceiver* target)
{

}

bool Enemy::IsExceutionAction() const
{
	return false;
}

// 計算処理
bool Enemy::Process()
{
	base::Process();

    CharaBase::STATUS old_status = _status;
   
    if(_status == STATUS::NONE)
    {
        _status = STATUS::WAIT;
    }

	// ステータスが変わっていないか？
	if(old_status == _status)
	{
		//再生時間を進める
		_play_time += 0.5f;
		// 再生時間をランダムに揺らがせる
		switch(_status)
		{
			case STATUS::WAIT:
			{
				_play_time += (float)(rand() % 10) / 100.0f;// 0.00 ～ 0.09 の揺らぎ。積算するとずれが起きる
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
			{
				_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "idle"), -1, FALSE);
				break;
			}
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

	// 再生時間がアニメーションの総再生時間に達したら再生時間を0に戻す
	if(_play_time >= _total_time)
	{
		_play_time = 0.0f;
	}

	return true;
}

// 描画処理
bool Enemy::Render()
{
	base::Render();
    // 再生時間をセット
    if(_attach_index != -1)
    {
        MV1SetAttachAnimTime(_handle, _attach_index, _play_time);
    }

	// 位置
	VC::MV1SetPosition(_handle, _pos);
	// 向きからY軸回転を算出
	Vec4 vrot = { 0.0f, 0.0f, 0.0f, 0.0f };
	vrot.y = atan2f(-_dir.x, -_dir.z);
	VC::MV1SetRotationXYZ(_handle, vrot);

	// 描画
	MV1DrawModel(_handle);
	return true;
}