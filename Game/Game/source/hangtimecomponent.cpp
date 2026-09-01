#include "pch.h"
#include "hangtimecomponent.h"

void HangTimeComponent::Start()
{
	_timer = air::AIR_TIME;
}


void HangTimeComponent::Update(float deltaTime)
{
	if(_timer <= 0.0f)
	{
		return;
	}

	_timer -= deltaTime;

	// 滞空中は少しずつ下降する
	Vec4 pos = _owner->GetPos();
	pos.y -= air::DESCENT_SPEED * deltaTime;

	if(pos.y < 0.0f)
	{
		pos.y = 0.0f;
	}

	_owner->SetPos(pos);

	if(_timer <= 0.0f || _owner->IsGround())
	{
		Reset();
	}
}

void HangTimeComponent::Reset()
{
	_timer = 0.0f;
}