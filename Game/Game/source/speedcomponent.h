#pragma once
#include "pch.h"

template <typename TOwner>
class SpeedComponent : public Component<TOwner>
{
public:
	using base = Component<TOwner>;

	explicit SpeedComponent(TOwner& owner) : base(owner) {}

	float GetSpeed() const { return _speed; }
	void SetSpeed(float speed) { _speed = speed; }

	void AddSpeed(float value) { _speed += value; }
	void ResetSpeed() { _speed = _baseSpeed; }
	void SetBaseSpeed(float baseSpeed) { _baseSpeed = baseSpeed; _speed = baseSpeed; }

private:
	float _speed{ 0.0f };
	float _baseSpeed{ 0.0f };
};

