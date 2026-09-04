#include "pch.h"
#include "normalshakestrategy.h"

Vec4 NormalShakeStrategy::Calculate(CameraShakeState& state) const
{
	float elapsed = static_cast<float>(state.elapsedFrame);
	float duration = static_cast<float>(state.durationFrame);

	// 揺れの強さを時間経過に応じて減衰させるためのイージング関数を使用
	float envelope = easing::EasingOutSine
	(
		elapsed,
		duration
	);

	float x = std::sin(state.phase * shake::SHAKE_X);
	float y = std::sin(state.phase * shake::SHAKE_Y);

	return v::VGet
	(
		x * state.intensity * envelope, // X軸の揺れ
		y * state.intensity * envelope, // Y軸の揺れ
		0.0f							// Z軸の揺れはなし
	);
}