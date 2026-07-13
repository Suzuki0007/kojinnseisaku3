#pragma once
#include "healthcomponent.h"

class DeadComponent : public HealthComponent
{
public:
	explicit DeadComponent(CharaBase& owner) : HealthComponent(owner) {}

	virtual bool IsAlive() const override
	{
		return false; // 常に死亡状態
	}

	virtual void ApplyDamage(float damage) override
	{
		// ダメージを受けても何もしない
	}
};

