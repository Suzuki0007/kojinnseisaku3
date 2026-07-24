#include "pch.h"
#include "charabase.h"
#include "animationcomponent.h"
#include "jumpcomponet.h"

CharaBase::CharaBase()
	: _jump(std::make_unique<JumpComponent>(*this))
{}

CharaBase::~CharaBase()
{
	ClearComponent();
}

// 初期化
bool CharaBase::Initialize()
{
	if(!base::Initialize()) { return false; }
	_handle = -1;
	_attach_index = -1;
	_total_time = 0.0f;
	_play_time = 0.0f;
	_pos = v::VGet(0.0f, 0.0f, 0.0f);
	_oldPos = v::VGet(0.0f, 0.0f, 0.0f);
	_dir = v::VGet(0.0f, 0.0f, 0.0f);
	_col_sub_y = 0.0f;
	_collision_r = 0.0f;
	_collision_weight = 0.0f;
	_status = STATUS::NONE;
	_is_alive = true; // 生存フラグを初期化
	return true;
}

// 終了
bool CharaBase::Terminate()
{
	base::Terminate();
	return true;
}

void CharaBase::RequestJump()
{
	if(_jump)
	{
		_jump->RequestJump();
	}
}

bool CharaBase::IsGround() const
{
	if(_jump)
	{
		return _jump->IsGround();
	}
	return false;
}

void CharaBase::NotifyLand()
{
	if(_jump)
	{
		_jump->SetGround(true);
	}
}

bool CharaBase::GetLand() const
{
	if(_jump)
	{
		return _jump->IsGround();
	}
	return false;
}

void CharaBase::SetLand(bool land)
{
	if(_jump)
	{
		_jump->SetGround(land);
	}
}

bool CharaBase::IsAscending() const
{
	if(_jump)
	{
		return !_jump->IsGround() && _jump->GetCurrentGravity() > 0.0f;
	}
	return false;
}

// 計算処理
bool CharaBase::Process()
{
	base::Process();
	return true;
}

bool CharaBase::Damage(float damage)
{
	// ダメージ量を減らす
	_hp -= damage;

	// 0以下になっていないか確認
	if(_hp <= 0.0f)
	{
		_hp = 0.0f;
		_is_alive = false; // HPが0になったら生存フラグをfalseにする
	}

	return true;
}

void CharaBase::AnimationRender(int handle, const Vec4& pos, const Vec4& dir)
{
	// 位置
	VC::MV1SetPosition(handle, pos);
	// 向きからY軸回転を算出
	Vec4 vrot = { 0.0f, 0.0f, 0.0f };
	vrot.y = atan2(dir.x * -1, dir.z * -1);// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
	VC::MV1SetRotationXYZ(handle, vrot);
	// 描画
	MV1DrawModel(handle);
}

// 描画処理
bool CharaBase::Render()
{
	base::Render();
	
	return true;
}


