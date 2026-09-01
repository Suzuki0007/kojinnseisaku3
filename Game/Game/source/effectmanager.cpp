#include "pch.h"
#include "effectmanager.h"
#include "kickeffect.h"
#include "bombeffect.h"

void  EffectManager::RegisterType()
{
	OMB::RegisterType("KickEffect", []() { return std::make_unique<KickEffect>(); });
	OMB::RegisterType("BombEffect", []() { return std::make_unique<BombEffect>(); });
}

EffectBase* EffectManager::CreateEffect(
	const std::string& type,
	const Vec4& pos,
	const Vec4& dir,
	float distance)
{
	auto& effects = GetAll();
	size_t oldSize = effects.size();

	Create(type);

	if(effects.size() == oldSize)
	{
		return nullptr;
	}

	EffectBase* effect = effects.back().get();

	effect->Initialize();
	effect->SetUp(pos, dir, distance);

	return effect;
}