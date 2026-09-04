#pragma once
#include "enemybase.h"

namespace enemy
{
	static constexpr float RESPAWN_TIME = 10.0f; // 敵のリスポーン時間
}

class Enemy : public EnemyBase
{
	typedef EnemyBase base;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	virtual const char* GetCharaClassName() const override { return "Enemy"; }
	virtual float GetSpeed() const override { return _speed->GetSpeed(); }

	void CommandAttack(IBattleReceiver* target) override;
	bool IsExceutionAction() const override;

	bool RespawnTime();
	float GetRespawnRemainingTime() const;

protected:
	float _deadElapsedTime{ 0.0f }; // 死亡してからの経過時間
};

