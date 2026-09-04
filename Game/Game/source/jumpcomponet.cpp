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
	float height = 0.0f;
	if(_isGround)
	{
		height = _jumpHeight; // 地面にいる場合は通常のジャンプ高さ

	}
	else
	{
		height = _airJumpHeight; // 空中にいる場合は空中ジャンプ高さ
	}

	_currentJumpCount++; // ジャンプ回数を増やす
	_currentGravity = height; // ジャンプの高さを設定する
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
		_owner->SetStatus(STA::FALL);
	}
}

void JumpComponent::Land()
{
	_isGround = true; // 着地状態にする
	_currentJumpCount = 0; // ジャンプ回数をリセットする
	_currentGravity = 0.0f; // 重力をリセットする
	_owner->SetStatus(STA::LANDING);
}

void JumpComponent::SetGround(bool ground)
{
	if(ground && !_isGround)
	{
		Land(); // 着地処理

	}
	_isGround = ground;
}

void JumpComponent::UpdateWithoutFall()
{
	// 落下処理をスキップしてジャンプ処理のみを行う
	if(_jumpRequest && _isGround)
	{
		Jump();
		_jumpRequest = false;
	}
	else if(_jumpRequest && !_isGround && _currentJumpCount < _maxJumpCount)
	{
		Jump();
		_jumpRequest = false;
	}
}

void JumpComponent::ResetJumpState()
{
	_currentJumpCount = 0; // ジャンプ回数をリセットする
	_jumpRequest = false; // ジャンプ要求をリセットする
}

void JumpComponent::GrantExtraJump()
{
	if(_currentJumpCount > 0)
	{
		_currentJumpCount--; // ジャンプ回数を増やす
	}
}