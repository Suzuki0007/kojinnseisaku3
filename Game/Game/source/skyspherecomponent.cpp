#include "pch.h"
#include "resourcepath.h"
#include "skyspherecomponent.h"

bool SkySphereComponent::Initialize()
{
	_mHandle = Load::LoadModel(path::Map("SkySphere"));
	return true;
}

bool SkySphereComponent::Process()
{
	return true;
}

bool SkySphereComponent::Render()
{
	DrawModel();
	return true;
}