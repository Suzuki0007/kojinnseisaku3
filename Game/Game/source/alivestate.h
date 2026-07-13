#pragma once

class AliveState
{
public:
	AliveState() = default;
	~AliveState() = default;

	auto GetHP() const { return _hp; }	// HP取得用ゲッター
	auto GetMaxHP() const { return _maxHp; } // 最大HP取得用ゲッター

	void SetHP(float hp) { this->_hp = hp; } // HP設定用セッター
	void SetMaxHP(float maxHp) { this->_maxHp = maxHp; } // 最大HP設定用セッター

	bool ApplyDamage(float damage);

private:
	float _hp{ 100.0f }; // 初期HP
	float _maxHp{ 100.0f }; // 最大HP
};
