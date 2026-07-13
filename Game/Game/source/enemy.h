#pragma once
#include "enemybase.h"
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

protected:

};

