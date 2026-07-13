#pragma once
#include "playerbase.h"

class Player3 : public PlayerBase
{
	typedef PlayerBase base;
public:
	Player3() = default;
	virtual ~Player3() = default;

	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;

	void ChangeAnim(CharaBase::STATUS next);

	virtual const char* GetCharaClassName() const override { return "Player3"; }
	virtual float GetSpeed() const override { return _speed->GetSpeed(); }

	void CommandAttack(IBattleReceiver* target) override;
	bool IsExceutionAction() const override;
};

