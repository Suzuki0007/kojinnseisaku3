#include "pch.h"
#include "dashcomponent.h"

void DashComponent::Update(float deltaTime)
{
	if(!_isDashing)
	{
		return;
	}

	// ダッシュ中の処理
	if(_dashTimer > 0.0f)
	{
		Vec4 pos = _owner->GetPos();
		pos.x += _dashDirection.x * _dashSpeed;
		pos.z += _dashDirection.z * _dashSpeed;
		_owner->SetPos(pos);
		_dashTimer -= deltaTime;
	}
	else
	{
		_isDashing = false;
		_owner->SetStatus(STA::FALL);
	}

}

Vec4 DashComponent::ResolveDashDirection(const Vec4& inputV, const Vec4& fallbackDir)
{
	Vec4 dir;

	if(v::VSize(inputV) > 0.0f)
	{
		dir = inputV;// 入力ベクトルをそのまま使用
	}
	else
	{
		dir = fallbackDir;// 入力ベクトルがゼロの場合はfallbackDirを使用
	}
	
	dir.y = 0.0f; // Y軸方向は無視する

	// 正規化して返す
	if(v::VSize(dir) > 0.0f)
	{
		return v::VNorm(dir);// 正規化した方向を返す
	}
	else
	{
		return v::VGet(0.0f, 0.0f, -1.0f); // デフォルトの前方方向
	}

	return dir;
}

void DashComponent::RequestDash(const Vec4& inputV, const Vec4& fallbackDir)
{
	if(_isDashing)
	{
		return; // すでにダッシュ中なら何もしない
	}

	_isDashing = true;
	_dashTimer = _dashTime;
	_dashDirection = ResolveDashDirection(inputV, fallbackDir);// ダッシュ方向を決定する
	_owner->SetStatus(STA::DASHING);
}

void DashComponent::CancelDash()
{
	_isDashing = false;
	_dashTimer = 0.0f;
}