#pragma once
#include "charabase.h"
#include "effectbase.h"

namespace attack
{
	// 攻撃対象に近づく距離の閾値と許容誤差
	constexpr float APPROACH_RANGE = 0.0f;      // 近づく距離の閾値
	constexpr float APPROACH_EPSILON = 30.0f;   // 近づく距離の許容誤差
	constexpr float GROUND_APPROACH_SPEED = 30.0f;// 攻撃対象に近づく速度
	constexpr float AIR_APPROACH_SPEED = 30.0f;// 攻撃対象に近づく速度
	constexpr float GROUND_FORWARD_SPEED = 5.0f;// 攻撃アニメーション中に前進する速度
	constexpr float AIR_FORWARD_SPEED = 8.0f;// 攻撃アニメーション中に前進する速度
	constexpr float FORWARD_MOVE_ANIM_RATIO = 0.5f; // 攻撃アニメーションの再生時間に対する前進する時間の割合

	// 攻撃カプセルのパラメータ
	constexpr Vec4 ATTACK_CAPSULE_UNDER_POS = v::VGet(0.0f, 0.0f, 0.0f); // 攻撃カプセルの下方向
	constexpr Vec4 ATTACK_CAPSULE_OVER_POS = v::VGet(0.0f, 0.0f, 0.0f); // 攻撃カプセルの上方向
	constexpr float ATTACK_CAPSULE_RADIUS = 80.0f; // 攻撃カプセルの半径
	constexpr int ATTACK_CAPSULE_WAIT_TIME = 0; // 攻撃カプセルの待機時間
	constexpr int ATTACK_CAPSULE_ACTIVE_TIME = 100; // 攻撃カプセルの有効時間
	constexpr float ATTACK_CAPSULE_DAMAGE = 10.0f; // 攻撃カプセルのダメージ量

	// 攻撃の位置
	constexpr float BACK_POS_OFFSET = 50.0f; // 攻撃エフェクトの位置の後方
	constexpr float BACK_POS_HEIGHT = 50.0f; // 攻撃エフェクトの位置の高さ
}

class AttackComponent final : public Component<CharaBase>
{
public:
	using Component::Component;
	using STA = CharaBase::STATUS;
	AttackComponent() = delete;
	virtual ~AttackComponent() = default;

	void SetUpAttackCapusule(int handle, std::vector<mymath::ATTACKCOLLISION>& attackCollisionList, std::string attackChara);

	void Update(float deltaTime) override;

	void RequestAttack(); // 攻撃要求を出す

	float GetApproachSpeed() const;// 攻撃対象に近づく速度を取得する
	float GetForwardSpeed() const;// 攻撃アニメーション中に前進する速度を取得する

	void ResetAirAttack() { _airAttackUsed = false; }// 攻撃状態をリセットする

	bool IsAttacking() const { return _owner->GetStatus() == STA::ATTACK; }

	void SetAttackCapusuleParams(int handle, std::vector<mymath::ATTACKCOLLISION>& attackCollisionList, std::string charaName);



private:
	void ExecuteAttack(); // 攻撃カプセル生成処理を実行する

	Vec4 GetBackPos() const;

	EffectBase* _kickEffect{ nullptr }; // 攻撃エフェクト

	bool _pendingAttack{ false }; // 攻撃保留フラグ
	bool _airAttackUsed{ false }; // 空中攻撃使用済みフラグ

	int _handle{ -1 };
	std::vector<mymath::ATTACKCOLLISION>* _attackCollisionList{ nullptr };
	std::string _attackChara;
};


