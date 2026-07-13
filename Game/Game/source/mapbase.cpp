#include "pch.h"
#include "mapbase.h"
#include "shadowmapcomponent.h"

bool MapBase::Initialize()
{
	if(!base::Initialize()) 
	{
		return false;
	}
	return true;
}

bool MapBase::Terminate()
{
	base::Terminate();
	ClearComponent();
	return true;
}

bool MapBase::Process()
{
	base::Process();
	return true;
}

bool MapBase::Render()
{
	base::Render();

	//auto* shadowCom = GetComponent<ShadowMapComponent>();

	return true;
}