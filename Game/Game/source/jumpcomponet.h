#pragma once
#include "charabase.h"

namespace jump
{
	static constexpr float GRAVITY = 0.98f; // 重力
	static constexpr float JUMP_HEIGHT = 15.0f; // ジャンプの高さ
	static constexpr int MAX_JUMP_COUNT = 2; // ジャンプの最大回数
}


class JumpComponent final : public Component<CharaBase>
{
public:
	using Component::Component;
	using STA = CharaBase::STATUS;

	JumpComponent() = delete;
	virtual ~JumpComponent() = default;


	void RequestJump();// ジャンプ要求を出す
	void Jump();// ジャンプ処理
	void Fall();// 落下処理
	void Land();// 着地処理

	void Update(float deltaTime) override;

	// ゲッター
	bool IsGround() const { return _isGround; }// 着地しているかどうか
	bool IsJumping() const { return !_isGround; }// ジャンプ中かどうか
	int GetCurrentJumpCount() const { return _currentJumpCount; }// 現在のジャンプ回数を返す
	float GetCurrentGravity() const { return _currentGravity; }// 現在の重力を返す
	
	// セッター
	void SetGravity(float g) { _gravity = g; }// 重力を設定する
	void SetJumpHeight(float h) { _jumpHeight = h; }// ジャンプの高さを設定する
	void SetMaxJumpCount(int count) { _maxJumpCount = count; }// ジャンプの最大回数を設定する
	void SetGround(bool ground);

private:
	int _maxJumpCount{ jump::MAX_JUMP_COUNT };
	float _gravity{ jump::GRAVITY };
	float _jumpHeight{ jump::JUMP_HEIGHT };

	int _currentJumpCount{ 0 };
	float _currentGravity{ 0.0f };
	bool _isGround{ true };
	bool _jumpRequest{ false };

};

