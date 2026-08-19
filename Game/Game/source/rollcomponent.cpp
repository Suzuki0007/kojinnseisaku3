#include "pch.h"
#include "rollcomponent.h"

void RollComponent::Update(float deltaTime)
{
	if(!_isRolling)
	{
		return;
	}

	if(_rollTimer > 0.0f)
	{
		float ratio = _rollTimer / _rollTime;// ドッジロール残り時間の割合
		float speed = _rollSpeed * (ratio * ratio);// ドッジロール速度を減速させる

		Vec4 pos = _owner->GetPos();

		pos.x += _rollDirection.x * speed;
		pos.z += _rollDirection.z * speed;
		_owner->SetPos(pos);

		_owner->SetDir(_rollDirection);// ドッジロール中は向きをドッジロール方向に固定する

		_rollTimer -= deltaTime;

		_owner->SetStatus(STA::ROLLING);// ドッジロール中のアニメーション
	}
	else
	{
		_isRolling = false;
		_owner->SetStatus(STA::WAIT);// ドッジロール終了後は待機状態に戻す
	}
}

Vec4 RollComponent::ResolveDirecton(const Vec4& inputV, const Vec4& fallbackDir)
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

void RollComponent::RequestRoll(const Vec4& inputV, const Vec4& fallbackDir)
{
	if(_isRolling)
	{
		return; // すでにドッジロール中なら何もしない
	}
	_isRolling = true;
	_rollTimer = _rollTime;
	_rollDirection = ResolveDirecton(inputV, fallbackDir);// ドッジロール方向を決定する
	_owner->SetStatus(STA::ROLLING); // ドッジロール中のアニメーション
}