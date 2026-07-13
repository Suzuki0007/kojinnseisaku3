#pragma once
#include "charabase.h"
#include "alivestate.h"
#include "deadstate.h"

class HealthComponent : public Component<CharaBase>
{
public:
	explicit HealthComponent(CharaBase& owner) : Component(owner), _stateMachine(AliveState()) {}
	virtual ~HealthComponent() {}
	virtual bool IsAlive() const;
	virtual void ApplyDamage(float damage);

	auto GetHP() const;	// HP取得用ゲッター
	auto GetMaxHP() const; // 最大HP取得用ゲッター

	void SetHP(float hp);

private:
	StateMachine<AliveState, DeadState> _stateMachine{ AliveState() };
};

