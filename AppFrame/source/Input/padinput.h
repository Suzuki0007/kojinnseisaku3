#pragma once
#include "inputdevice.h"
#include "DxLib.h"
#include <array>

namespace xInput
{
	static constexpr int XINPUT_DEAD_ZONE = 100;// ジョイスティックのデッドゾーンの値
}

class PadInput : public InputDevice
{
public:
	PadInput();
	virtual ~PadInput() = default;
	virtual void Update() override;
	virtual bool IsPress(InputButton button) const override;
	virtual bool IsTrigger(InputButton button) const override;
	virtual bool IsRelease(InputButton button) const override;

private:
	// 通常ボタン128個、スティック4方向、ハットスイッチ4方向の合計140個の状態を保存する配列
	std::array<char, 140> m_PadStatus{};// パッドの状態を保存する変数
	std::array<char, 140> m_PadStatusOld{};// 前フレームのパッドの状態を保存する変数

	std::array<int, std::to_underlying(InputButton::Max) > m_PadMap{};

protected:
	int padX{ 0 };// ジョイスティックのX軸の値
	int padY{ 0 };// ジョイスティックのY軸の値
};
