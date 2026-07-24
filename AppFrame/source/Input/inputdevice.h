#pragma once

// 入力デバイスの抽象クラス
enum class InputButton
{
	StickUp,
	StickDown,
	StickLeft,
	StickRight,

	CommandUp,
	CommandDown,
	CommandLeft,
	CommandRight,

	RightTarget,
	LeftTarget,

	Decide,

	SceneDebug,
	Menu,

	Max
};


class InputDevice
{
public:
	InputDevice() = default;
	virtual ~InputDevice() = default;

	virtual void Update() = 0;

	virtual bool IsPress(InputButton button) const = 0;
	virtual bool IsTrigger(InputButton button) const = 0;
	virtual bool IsRelease(InputButton button) const = 0;

	virtual float GetRightStickX() const = 0;
	virtual float GetRightStickY() const = 0;
};