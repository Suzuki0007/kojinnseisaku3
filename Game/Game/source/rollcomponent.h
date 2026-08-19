#pragma once
#include "charabase.h"

namespace roll
{
	static constexpr float ROLL_SPEED = 50.0f; // ドッジロール速度
	static constexpr float ROLL_TIME = 14.0f; // ドッジロール継続時間
}

class RollComponent final : public Component<CharaBase>
{
public:
	using Component::Component;
	using STA = CharaBase::STATUS;

	RollComponent() = delete;
	virtual ~RollComponent() = default;
	
	void RequestRoll(const Vec4& inputV, const Vec4& fallbackDir);

	void Update(float deltaTime) override;

	bool IsRolling() const { return _isRolling; } // ドッジロール中かどうかを返す
	Vec4 GetRollDirection() const { return _rollDirection; } // ドッジロール方向を返す

	void SetRollSpeed(float speed) { _rollSpeed = speed; } // ドッジロール速度を設定する
	void SetRollTime(float time) { _rollTime = time; } // ドッジロール継続時間を設定する

private:
	Vec4 ResolveDirecton(const Vec4& inputV, const Vec4& fallbackDir);
	float _rollSpeed{ roll::ROLL_SPEED };				// ドッジロール速度
	float _rollTime{ roll::ROLL_TIME };					// ドッジロール継続時間

	float _rollTimer{ 0.0f };							// ドッジロール残り時間
	bool _isRolling{ false };							// ドッジロール中かどうか
	Vec4 _rollDirection{ v::VGet(0.0f, 0.0f, 0.0f) };	// ドッジロール方向
};

