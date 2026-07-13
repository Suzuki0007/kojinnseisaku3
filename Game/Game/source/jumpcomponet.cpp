#include "pch.h"
#include "jumpcomponet.h"


void JumpComponent::Update(float deltaTime)
{
	// ジャンプ1回目
	if(_jumpRequest && _isGround)
	{
		Jump();
		_jumpRequest = false; // ジャンプ要求をしない
	}
	// ジャンプ2回目
	else if(_jumpRequest && !_isGround && _currentJumpCount < _maxJumpCount)
	{
		Jump();
		_jumpRequest = false; // ジャンプ要求をしない

	}
	// 落下処理
	else if(!_isGround)
	{
		Fall();
	}
}

void JumpComponent::RequestJump()
{
	if(_currentJumpCount < _maxJumpCount)
	{
		_jumpRequest = true; // ジャンプ要求を出す
	}
}

void JumpComponent::Jump()
{
	_currentJumpCount++; // ジャンプ回数を増やす
	_currentGravity = _jumpHeight; // ジャンプの高さを設定する
	_isGround = false; // 着地していない状態にする

	Vec4 pos = _owner->GetPos();
	pos.y += _currentGravity; // ジャンプの高さ分だけ位置を上げる
	_owner->SetPos(pos);

	_owner->SetStatus(STA::JUMP); // ジャンプのアニメーション
}

void JumpComponent::Fall()
{
	_currentGravity -= _gravity; // 重力分だけ落下する

	Vec4 pos = _owner->GetPos();
	pos.y += _currentGravity; // 落下分だけ位置を下げる
	_owner->SetPos(pos);

	if(_currentGravity < 0.0f)
	{
		_owner->SetStatus(STA::FALL); // 落下のアニメーション
	}
}

void JumpComponent::Land()
{
	_isGround = true; // 着地状態にする
	_currentJumpCount = 0; // ジャンプ回数をリセットする
	_currentGravity = 0.0f; // 重力をリセットする
	_owner->SetStatus(STA::LANDING); // 着地のアニメーション
}

void JumpComponent::SetGround(bool ground)
{
	if(ground && !_isGround)
	{
		Land(); // 着地処理

	}
	_isGround = ground;
}
