#pragma once
#include "camerashakestrategy.h"

struct CameraShakeParameter
{
	float intensity { 0.0f }; // 揺れの強さ
	int durationFrame{ 1 }; // 揺れの持続時間
	float frequency{ 0.5f }; // 揺れの周波数
};

class CameraShake
{
public:
	explicit CameraShake(std::unique_ptr<ICameraShakeStrategy> strategy);
	virtual ~CameraShake() = default;

	void SetStrategy(std::unique_ptr<ICameraShakeStrategy> strategy);// カメラの揺れを設定

	void StartShake(const CameraShakeParameter& param);// カメラの揺れを開始
	void Update();
	Vec4 GetOffset() const { return _offset; }
	bool IsActive() const { return _active; }

protected:
	std::unique_ptr<ICameraShakeStrategy> _strategy;

	CameraShakeState _state;
	Vec4 _offset {};

	bool _active{ false }; // カメラの揺れが有効かどうか
};

