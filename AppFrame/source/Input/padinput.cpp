#include "pch.h"
#include "padinput.h"


PadInput::PadInput()
{
	m_PadMap[std::to_underlying(InputButton::StickUp)] = 128;
	m_PadMap[std::to_underlying(InputButton::StickDown)] = 129;
	m_PadMap[std::to_underlying(InputButton::StickLeft)] = 130;
	m_PadMap[std::to_underlying(InputButton::StickRight)] = 131;

	m_PadMap[std::to_underlying(InputButton::CommandUp)] = 132;
	m_PadMap[std::to_underlying(InputButton::CommandDown)] = 133;
	m_PadMap[std::to_underlying(InputButton::CommandLeft)] = 134;
	m_PadMap[std::to_underlying(InputButton::CommandRight)] = 135;

	m_PadMap[std::to_underlying(InputButton::SceneDebug)] = 5;// RBボタン
	m_PadMap[std::to_underlying(InputButton::Decide)] = 0;// Aボタン
	m_PadMap[std::to_underlying(InputButton::RightTarget)] = 6;// RTボタン
	m_PadMap[std::to_underlying(InputButton::LeftTarget)] = 7;// LTボタン

	m_PadMap[std::to_underlying(InputButton::Menu)] = 11; // STARTボタン
}

void PadInput::Update()
{
	m_PadStatusOld = m_PadStatus;// 前フレームのパッドの状態を保存

	DINPUT_JOYSTATE di;
	if(GetJoypadDirectInputState(DX_INPUT_PAD1, &di) == -1)
	{
		m_PadStatus.fill(0);// パッドの状態を全て0にする
		return;
	}

	for(size_t i = 0; i < 128; ++i)
	{
		if(di.Buttons[i] != 0)
		{
			m_PadStatus[i] = 1;// 押されていたら1にする
		}
		else
		{
			m_PadStatus[i] = 0;// 離していたら0にする
		}
	}

	if(di.Y < -xInput::XINPUT_DEAD_ZONE)
	{
		m_PadStatus[128] = 1;// スティックが上に倒されていたら1にする
	}
	else
	{
		m_PadStatus[128] = 0;// スティックが上に倒されていなかったら0にする
	}

	if(di.Y > xInput::XINPUT_DEAD_ZONE)
	{
		m_PadStatus[129] = 1;// スティックが下に倒されていたら1にする
	}
	else
	{
		m_PadStatus[129] = 0;// スティックが下に倒されていなかったら0にする
	}

	if(di.X < -xInput::XINPUT_DEAD_ZONE)
	{
		m_PadStatus[130] = 1;// スティックが左に倒されていたら1にする
	}
	else
	{
		m_PadStatus[130] = 0;// スティックが左に倒されていなかったら0にする
	}

	if(di.X > xInput::XINPUT_DEAD_ZONE)
	{
		m_PadStatus[131] = 1;// スティックが右に倒されていたら1にする
	}
	else
	{
		m_PadStatus[131] = 0;// スティックが右に倒されていなかったら0にする
	}

	int pov = di.POV[0];
	if(pov >= 0 && (pov <= 4500 || pov >= 31500))
	{
		m_PadStatus[132] = 1;// ハットスイッチが上に倒されていたら1にする
	}
	else
	{
		m_PadStatus[132] = 0;// ハットスイッチが上に倒されていなかったら0にする
	}

	if(pov >= 13500 && pov <= 22500)
	{
		m_PadStatus[133] = 1;// ハットスイッチが下に倒されていたら1にする
	}
	else
	{
		m_PadStatus[133] = 0;// ハットスイッチが下に倒されていなかったら0にする
	}

	if(pov >= 22500 && pov <= 31500)
	{
		m_PadStatus[134] = 1;// ハットスイッチが左に倒されていたら1にする
	}
	else
	{
		m_PadStatus[134] = 0;// ハットスイッチが左に倒されていなかったら0にする
	}

	if(pov >= 4500 && pov <= 13500)
	{
		m_PadStatus[135] = 1;// ハットスイッチが右に倒されていたら1にする
	}
	else
	{
		m_PadStatus[135] = 0;// ハットスイッチが右に倒されていなかったら0にする
	}

	// 右スティック
	float rawRightX = static_cast< float >( di.Z ) / 1000.0f;
	float rawRightY = static_cast< float >( di.Rz ) / 1000.0f;

	// デッドゾーンの処理
	if(std::abs(rawRightX) > xInput::DINPUT_DEAD_ZONE)
	{
		_rightStickX = rawRightX;
	}
	else
	{
		_rightStickX = 0.0f;
	}

	if(std::abs(rawRightY) > xInput::DINPUT_DEAD_ZONE)
	{
		_rightStickY = rawRightY;
	}
	else
	{
		_rightStickY = 0.0f;
	}
}

bool PadInput::IsPress(InputButton button) const
{
	int index = m_PadMap[std::to_underlying(button)];
	return m_PadStatus[index] == 1;// ボタンが押されているかどうかを返す
}

bool PadInput::IsTrigger(InputButton button) const
{
	int index = m_PadMap[std::to_underlying(button)];
	return m_PadStatus[index] == 1 && m_PadStatusOld[index] == 0;// ボタンが今フレームで押されたかどうかを返す
}

bool PadInput::IsRelease(InputButton button) const
{
	int index = m_PadMap[std::to_underlying(button)];
	return m_PadStatus[index] == 0 && m_PadStatusOld[index] == 1;// ボタンが今フレームで離されたかどうかを返す
}