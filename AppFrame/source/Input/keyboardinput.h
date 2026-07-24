#pragma once
#include "inputdevice.h"
#include "DxLib.h"
#include <array>

class KeyboardInput : public InputDevice
{
public:
	KeyboardInput();
	virtual ~KeyboardInput() = default;

	virtual void Update() override;

	virtual bool IsPress(InputButton button) const override;
	virtual bool IsTrigger(InputButton button) const override;
	virtual bool IsRelease(InputButton button) const override;

	virtual float GetRightStickX() const override { return 0.0f; }
	virtual float GetRightStickY() const override { return 0.0f; }

private:
	std::array<char, 256> m_KeyStatus {};// キーの状態を保存する配列
	std::array<char, 256> m_KeyStatusOld {};// 前フレームのキーの状態を保存する配列
	std::array<int, std::to_underlying(InputButton::Max)> m_KeyMap{};// InputButtonとDxLibのキーコードを対応させる配列
};

