#pragma once

class DeadState
{
public:
	DeadState() = default;
	~DeadState() = default;

	auto GetHP() const { return 0.0f; }	// HP取得用ゲッター
	auto GetMaxHP() const { return 0.0f; } // 最大HP取得用ゲッター
	void SetHP(float hp) {} // HP設定用セッター
	bool ApplyDamage(float damage) { return false; } // ダメージを受けても何もしない
};
