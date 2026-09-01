#pragma once
#include "charabase.h"
class Goal : public CharaBase
{
	using base = CharaBase;

public:
	Goal() = default;
	virtual ~Goal() = default;
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;
	virtual const char* GetCharaClassName() const override { return "Goal"; }
	virtual float GetSpeed() const override { return 0.0f; }
	virtual void CommandAttack(IBattleReceiver* target) override {}
	virtual bool IsExceutionAction() const override { return false; }


};

