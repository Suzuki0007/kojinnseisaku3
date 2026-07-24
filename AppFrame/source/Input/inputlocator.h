#pragma once
#include "inputdevice.h"
#include <memory>

class NullInputDevice : public InputDevice
{
public:
	void Update() override{}
	bool IsPress(InputButton button) const override { return false; }
	bool IsTrigger(InputButton button) const override { return false; }
	bool IsRelease(InputButton button) const override { return false; }

	float GetRightStickX() const override { return 0.0f; }
	float GetRightStickY() const override { return 0.0f; }
};

// 入力デバイスのロケータークラス
class InputLocator
{
public:
	InputLocator() = delete;
	virtual ~InputLocator() = default;

	// デバイスの切り替え
	static void Provide(const std::shared_ptr<InputDevice>& device);
	// デバイスの取得
	static InputDevice& Get();

private:
	static std::shared_ptr<InputDevice> s_CurrentDevice;// 現在の入力デバイス
	static NullInputDevice s_NullDevice;// デバイスが提供されていない場合に使用するNullオブジェクト

};

