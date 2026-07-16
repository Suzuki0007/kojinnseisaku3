#include "pch.h"
#include "keyboardinput.h"


KeyboardInput::KeyboardInput()
{
	// std::to_underlyingはenumを配列の数字として安全に使うための
	m_KeyMap[std::to_underlying(InputButton::StickUp)] = KEY_INPUT_UP;
	m_KeyMap[std::to_underlying(InputButton::StickDown)] = KEY_INPUT_DOWN;
	m_KeyMap[std::to_underlying(InputButton::StickLeft)] = KEY_INPUT_LEFT;
	m_KeyMap[std::to_underlying(InputButton::StickRight)] = KEY_INPUT_RIGHT;

	m_KeyMap[std::to_underlying(InputButton::CommandUp)] = KEY_INPUT_UP;
	m_KeyMap[std::to_underlying(InputButton::CommandDown)] = KEY_INPUT_DOWN;
	m_KeyMap[std::to_underlying(InputButton::CommandLeft)] = KEY_INPUT_LEFT;
	m_KeyMap[std::to_underlying(InputButton::CommandRight)] = KEY_INPUT_RIGHT;

	m_KeyMap[std::to_underlying(InputButton::Decide)] = KEY_INPUT_SPACE;

	m_KeyMap[std::to_underlying(InputButton::SceneDebug)] = KEY_INPUT_RETURN;

	m_KeyMap[std::to_underlying(InputButton::RightTarget)] = KEY_INPUT_Q;
	m_KeyMap[std::to_underlying(InputButton::LeftTarget)] = KEY_INPUT_W;
}

void KeyboardInput::Update()
{
	m_KeyStatusOld = m_KeyStatus;// 前フレームのキーの状態を保存

	GetHitKeyStateAll(m_KeyStatus.data());// 現在のキーの状態を取得
}

bool KeyboardInput::IsPress(InputButton button) const
{
	int keyCode = m_KeyMap[std::to_underlying(button)];
	return m_KeyStatus[keyCode] == 1;// キーが押されているかどうかを返す
}

bool KeyboardInput::IsTrigger(InputButton button) const
{
	int keyCode = m_KeyMap[std::to_underlying(button)];
	return m_KeyStatus[keyCode] == 1 && m_KeyStatusOld[keyCode] == 0;// キーが今フレームで押されたかどうかを返す
}

bool KeyboardInput::IsRelease(InputButton button) const
{
	int keyCode = m_KeyMap[std::to_underlying(button)];
	return m_KeyStatus[keyCode] == 0 && m_KeyStatusOld[keyCode] == 1;// キーが今フレームで離されたかどうかを返す
}