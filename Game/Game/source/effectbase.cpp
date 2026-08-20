#include "pch.h"
#include "effectbase.h"

bool EffectBase::Initialize()
{
	base::Initialize();

	EffekseerManager::GetInstance()->Initialize();

	return true;
}

bool EffectBase::Terminate()
{
	base::Terminate();
	return true;
}

bool EffectBase::Process()
{
	base::Process();
	return true;
}

bool EffectBase::Render()
{
	base::Render();

	EffekseerManager::GetInstance()->Render();

	return true;
}