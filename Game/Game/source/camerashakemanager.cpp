#include "pch.h"
#include "camerashakemanager.h"
#include "normalshakestrategy.h"

namespace
{
	constexpr float MIN_SHAKE_INTENSITY = 5.0f; // 最小の揺れの強さ
	constexpr float INTENSITY_SCALE = 0.5f;// ダメージに応じた揺れの強さのスケーリング係数
	constexpr int SHAKE_DURATION = 30; // 揺れの持続時間
	constexpr float SHAKE_FREQUENCY = 0.5f; // 揺れの周波数
}

CameraShakeManager::CameraShakeManager(Subject<HitInfo>& hitSubject)
	:ObserverBase<HitInfo>(hitSubject),
	_cameraShake(std::make_unique<NormalShakeStrategy>())
{}

void CameraShakeManager::OnNotify(const HitInfo& data)
{
	_cameraShake.SetStrategy(std::make_unique<NormalShakeStrategy>());

	float intensity = max::MyMax(MIN_SHAKE_INTENSITY, data.damage * INTENSITY_SCALE); // ダメージに応じて揺れの強さを調整

	// カメラの揺れを開始
	_cameraShake.StartShake(
		CameraShakeParameter
		{
			.intensity = intensity,
			.durationFrame = SHAKE_DURATION,
			.frequency = SHAKE_FREQUENCY
		}
	);
}

