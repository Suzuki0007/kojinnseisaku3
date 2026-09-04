#pragma once
#include "pch.h"
#include "camerashakestate.h"

class ICameraShakeStrategy
{
public:
	virtual ~ICameraShakeStrategy() = default;
	virtual Vec4 Calculate(CameraShakeState& state) const = 0;// カメラの揺れの計算のための純粋仮想関数
};

