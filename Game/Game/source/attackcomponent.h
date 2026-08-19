#pragma once
#include "charabase.h"

namespace attack
{
	constexpr float APPROACH_RANGE = 0.0f;      // 近づく距離の閾値
	constexpr float APPROACH_EPSILON = 30.0f;   // 近づく距離の許容誤差
	constexpr float GROUND_APPROACH_SPEED = 10.0f;
	constexpr float AIR_APPROACH_SPEED = 8.0f;
	constexpr float GROUND_FORWARD_SPEED = 5.0f;
	constexpr float AIR_FORWARD_SPEED = 8.0f;
}

class AttackComponent final : public Component<CharaBase>
{
public:
	using Component::Component;
	using STA = CharaBase::STATUS;
	AttackComponent() = delete;
	virtual ~AttackComponent() = default;
	
	// 実際の攻撃カプセル生成処理を登録する
	void SetAttacExecutor(std::move_only_function<void()> executor) { _attackExecutor = std::move(executor); } 

	void Update(float deltaTime) override;

	void RequestAttack(); // 攻撃要求を出す

	float GetApproachSpeed() const;
	float GetForwardSpeed() const;

	void ResetAirAttack() { _airAttackUsed = false; }// 攻撃状態をリセットする

	bool IsAttacking() const { return _owner->GetStatus() == STA::ATTACK; }

private:
	bool _pendingAttack{ false }; // 攻撃保留フラグ
	bool _airAttackUsed{ false }; // 空中攻撃使用済みフラグ
	std::move_only_function<void()> _attackExecutor { nullptr }; // 攻撃カプセル生成処理
};

