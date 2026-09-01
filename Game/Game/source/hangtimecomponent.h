#pragma once
#include "charabase.h"

namespace air
{
	constexpr float AIR_TIME = 100.0f; // 滞空時間
	constexpr float DESCENT_SPEED = 0.5f; // 滞空中の下降速度
}

// 滞空時間コンポーネント
class HangTimeComponent final : public Component<CharaBase>
{
public:
	using Component::Component;
	HangTimeComponent() = delete;
	virtual ~HangTimeComponent() = default;

	void Start();
	void Update(float deltaTime) override;
	void Reset();
	bool IsActive() const { return _timer > 0.0f; }

protected:
	float _timer{ 0.0f };
};

