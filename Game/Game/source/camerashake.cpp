#include "pch.h"
#include "camerashake.h"

CameraShake::CameraShake(std::unique_ptr<ICameraShakeStrategy> strategy)
	: _strategy(std::move(strategy))
{}

void CameraShake::SetStrategy(std::unique_ptr<ICameraShakeStrategy> strategy)
{
	_strategy = std::move(strategy);
}

void CameraShake::StartShake(const CameraShakeParameter& param)
{
	_state.intensity = param.intensity;// 揺れの強さを設定
	_state.frequency = param.frequency;// 揺れの周波数を設定
	_state.phase = 0.0f;// 揺れの位相を初期化
	_state.elapsedFrame = 0;// 経過フレーム数を初期化
	_state.durationFrame = max::MyMax(1, param.durationFrame);// 揺れの継続フレーム数を1以上に制限
	_offset = Vec4::Zero();
	_active = true;
}

void CameraShake::Update()
{
	if(!_active || !_strategy)
	{
		_offset = Vec4::Zero();
		return;
	}

	_offset = _strategy->Calculate(_state);// カメラの揺れの計算

	_state.elapsedFrame++;// 経過フレーム数を増加
	_state.phase += _state.frequency;// 揺れの位相(どの位置にいるのか)を増加

	// 揺れの継続フレーム数を超えたら揺れを終了
	if(_state.elapsedFrame >= _state.durationFrame)
	{
		_active = false;
		_offset = Vec4::Zero();
	}
}