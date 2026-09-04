#pragma once

struct CameraShakeState
{
	float intensity { 0.0f }; // 揺れの強さ
	float frequency{ 0.0f }; // 揺れの周波数
	float phase{ 0.0f }; // 揺れの位相

	int elapsedFrame{ 0 }; // 経過フレーム数
	int durationFrame{ 1 }; // 揺れの継続フレーム数
};