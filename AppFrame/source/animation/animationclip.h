#pragma once
#include "pch.h"

// アニメーションの情報を保持するクラス
class AnimationClip
{
public:
	AnimationClip() = default;
	AnimationClip(
		const std::string_view& name,
		bool loop = true, 
		float speed = 1.0f,
		float speedVariance = 0.0f,// 再生速度のばらつき
		int startOffsetMax = 0// 開始オフセットの最大値
	)
		: _name(name), _loop(loop), _speed(speed), _speedVariance(speedVariance), _startOffsetMax(startOffsetMax)
	{
		_totalTime = 0.0f;
	}

	const std::string_view& GetName() const { return _name; }
	bool IsLoop() const { return _loop; }
	float GetSpeed() const { return _speed; }
	float GetTotalTime() const { return _totalTime; }
	float GetSpeedVariance() const { return _speedVariance; }
	int GetStartOffsetMax() const { return _startOffsetMax; }

private:
	std::string_view _name;
	bool _loop{ true };// ループ再生するかどうか
	float _speed{ 1.0f };// 再生速度
	float _totalTime{ 0.0f };// 総再生時間
	float _speedVariance{ 0.0f };// 再生速度のばらつき
	int _startOffsetMax{ 0 };// 開始オフセットの最大値
};

