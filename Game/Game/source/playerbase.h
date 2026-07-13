#pragma once
#include "charabase.h"
#include "animationcomponent.h"

class PlayerBase : public CharaBase
{
	typedef CharaBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	const Vec4& GetInputVector() const { return _input_v; }
	float GetMoveSpeed() const { return _mv_speed; }
	std::vector<mymath::ATTACKCOLLISION>& GetAttackCollisionList() { return _attack_collision; }


protected:

	float _mv_speed;
	Vec4 _input_v;
	std::vector<mymath::ATTACKCOLLISION> _attack_collision; // 攻撃用カプセル当たり判定リスト
};

