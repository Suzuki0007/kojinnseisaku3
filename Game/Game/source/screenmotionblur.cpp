#include "pch.h"
#include "screenmotionblur.h"

namespace
{
	constexpr float MAX_ENVELOPE = 1.0f;
}

void ScreenMotionBlur::Start()
{
	_data.isActive = true;
}

void ScreenMotionBlur::Stop()
{
	_data.isActive = false;
}

bool ScreenMotionBlur::IsActive() const
{
	return _data.isActive;
}