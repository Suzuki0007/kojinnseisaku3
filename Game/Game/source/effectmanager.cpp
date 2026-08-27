#include "pch.h"
#include "effectmanager.h"
#include "kickeffect.h"

void  EffectManager::RegisterType()
{
	OMB::RegisterType("KickEffect", []() { return std::make_unique<KickEffect>(); });
}

void EffectManager::CreateEffect(const std::string& type)
{
	Create(type);
}