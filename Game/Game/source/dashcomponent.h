#pragma once
#include "charabase.h"

namespace dash
{
	static constexpr float DASH_SPEED = 20.0f; // ダッシュ速度
	static constexpr float DASH_TIME = 12.0f; // ダッシュ継続時間
}

class DashComponent final : public Component<CharaBase>
{
public:
	using Component::Component;
	using STA = CharaBase::STATUS;

	DashComponent() = delete;
	virtual ~DashComponent() = default;

	// 正規化した方向でダッシュを開始する
	void RequestDash(const Vec4& inputV, const Vec4& fallbackDir);

	void Update(float deltaTime) override;

	void CancelDash(); // ダッシュをキャンセルする

	bool IsDashing() const { return _isDashing; } // ダッシュ中かどうかを返す
	Vec4 GetDashDirection() const { return _dashDirection; } // ダッシュ方向を返す

	void SetDashSpeed(float speed) { _dashSpeed = speed; } // ダッシュ速度を設定する
	void SetDashTime(float time) { _dashTime = time; } // ダッシュ継続時間を設定する

private:

	// 入力ベクトルがゼロベクトルの場合は、fallbackDirを返す
	Vec4 ResolveDashDirection(const Vec4& inputV, const Vec4& fallbackDir);

	bool _isDashing{ false };							// ダッシュ中かどうか
	Vec4 _dashDirection{ v::VGet(0.0f, 0.0f, 0.0f) };	// ダッシュ方向
	float _dashSpeed{ dash::DASH_SPEED };				// ダッシュ速度
	float _dashTime{ dash::DASH_TIME };					// ダッシュ継続時間
	float _dashTimer{ 0.0f };							// ダッシュ残り時間
};

