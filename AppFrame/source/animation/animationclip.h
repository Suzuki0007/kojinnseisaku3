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
		float speed = 1.0f
	)
		: _name(name), _loop(loop), _speed(speed)
	{
		_totalTime = 0.0f;
	}

	const std::string_view& GetName() const { return _name; }
	bool IsLoop() const { return _loop; }
	float GetSpeed() const { return _speed; }
	float GetTotalTime() const { return _totalTime; }

private:
	std::string_view _name;
	bool _loop{ true };
	float _speed{ 1.0f };
	float _totalTime{ 0.0f };
};

