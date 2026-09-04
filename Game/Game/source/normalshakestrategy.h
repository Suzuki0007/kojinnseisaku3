#pragma once
#include "camerashakestrategy.h"

namespace shake
{
	constexpr float SHAKE_X = 1.0f; // X軸の揺れの強さ
	constexpr float SHAKE_Y = 2.0f; // Y軸の揺れの強さ
}

class NormalShakeStrategy : public ICameraShakeStrategy
{
public:
	NormalShakeStrategy() = default;
	virtual ~NormalShakeStrategy() = default;

	virtual Vec4 Calculate(CameraShakeState& state) const override;// カメラの揺れを計算する関数
};

