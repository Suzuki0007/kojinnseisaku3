#include "pch.h"
#include "resourcepath.h"
#include "skysphere.h"

bool SkySphere::Initialize()
{
	_mHandle = Load::LoadModel(path::Map("SkySphere"));
	return true;
}

bool SkySphere::Terminate()
{
	if(_mHandle != -1)
	{
		MV1DeleteModel(_mHandle);
		_mHandle = -1;
	}
	return true;
}

bool SkySphere::Process()
{
	return true;
}

bool SkySphere::Render()
{
	if(_mHandle != -1)
	{
		MV1DrawModel(_mHandle);
	}
	return true;
}