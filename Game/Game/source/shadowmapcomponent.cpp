#include "pch.h"
#include "shadowmapcomponent.h"

bool ShadowMapComponent::Initialize()
{
	_shadowMapHandle = MakeShadowMap(1024, 1024);
	return true;
}

bool ShadowMapComponent::Process()
{
	if(_shadowMapHandle != -1)
	{
		return true;
	}

	return true;
}

bool ShadowMapComponent::Render()
{
	return true;
}