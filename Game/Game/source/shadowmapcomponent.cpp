#include "pch.h"
#include "shadowmapcomponent.h"

namespace
{
	constexpr int SHADOW_MAP_SIZE_X = 1024;
	constexpr int SHADOW_MAP_SIZE_Y = 1024;
	constexpr float LENGTH = 800.0f;

	constexpr Vec4 LIGHT_DIRECTION = v::VGet(-1.0f, -1.0f, 0.5f);


	constexpr Vec4 LIGHT_POS_MIN = v::VGet(-LENGTH, -1.0f, -LENGTH);
	constexpr Vec4 LIGHT_POS_MAX = v::VGet(LENGTH, LENGTH, LENGTH);
}

bool ShadowMapComponent::Initialize()
{
	if(!base::Initialize())
	{
		return false;
	}

	_shadowMapHandle = MakeShadowMap(SHADOW_MAP_SIZE_X, SHADOW_MAP_SIZE_Y);
	return true;
}

bool ShadowMapComponent::Terminate()
{
	if(_shadowMapHandle != -1)
	{
		DeleteShadowMap(_shadowMapHandle);
		_shadowMapHandle = -1;
	}
	return true;
}

bool ShadowMapComponent::Process()
{
	return true;
}

void ShadowMapComponent::Begin(const Camera& camera)
{
	if(_shadowMapHandle == -1)
	{
		return;
	}

	VC::SetShadowMapLightDirection(
		_shadowMapHandle,
		LIGHT_DIRECTION
	);

	const auto minPos =
		v::VAdd(
			camera._v_target,
			LIGHT_POS_MIN
		);

	const auto maxPos =
		v::VAdd(
			camera._v_target,
			LIGHT_POS_MAX
		);

	VC::SetShadowMapDrawArea(
		_shadowMapHandle,
		minPos,
		maxPos
	);

	ShadowMap_DrawSetup(_shadowMapHandle);
}

void ShadowMapComponent::End()
{
	if(_shadowMapHandle == -1)
	{
		return;
	}
	ShadowMap_DrawEnd();
	SetUseShadowMap(0, _shadowMapHandle);
}

bool ShadowMapComponent::Render()
{
	return true;
}