#pragma once
#include "charabase.h"

namespace air
{
	constexpr float AIR_TIME = 20.0f; // 滞空時間
}

// 滞空時間コンポーネント
class HangTimeComponent : public Component<CharaBase>
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

