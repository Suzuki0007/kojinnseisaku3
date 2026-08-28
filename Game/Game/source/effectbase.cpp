#include "pch.h"
#include "effectbase.h"

bool EffectBase::Initialize()
{
	base::Initialize();

	EffekseerManager::GetInstance()->Initialize();

	return true;
}

void EffectBase::SetUp(const Vec4& startpos, const Vec4& dir, float distance)
{

}

void EffectBase::SetEffectPos(const Vec4& pos)
{

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