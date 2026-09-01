#include "pch.h"
#include "bombeffect.h"
#include "resourcepath.h"

namespace
{
	constexpr float BOMB_EFFECT_SCALE = 500.0f;
}

bool BombEffect::Initialize()
{
	base::Initialize();
	auto* effectManager = EffekseerManager::GetInstance();

	auto path = path::Effect("BombEffect");

	_handle = effectManager->LoadEffect(path.data(), BOMB_EFFECT_SCALE);
	return true;
}

bool BombEffect::Terminate()
{
	base::Terminate();
	return true;
}

bool BombEffect::Process()
{
	base::Process();
	return true;
}

const char* BombEffect::GetCharaClassName() const
{
	return "";
}

float BombEffect::GetSpeed() const
{
	return 0.0f;
}

void BombEffect::CommandAttack(IBattleReceiver* target)
{
	
}

bool BombEffect::IsExceutionAction() const
{
	return false;
}

void BombEffect::SetUp(const Vec4& startpos, const Vec4& dir, float distance)
{
	PlayEffect(startpos);
}

bool BombEffect::PlayEffect(const Vec4& pos)
{
	auto* effectManager = EffekseerManager::GetInstance();
	if(effectManager == nullptr)
	{
		return false;
	}

	effectManager->PlayEffect3DPos(_handle, pos);

	return true;
}

bool BombEffect::Render()
{
	base::Render();
	return true;
}