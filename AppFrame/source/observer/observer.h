#pragma once

template<typename T>
class IObserver
{
public:
	IObserver() = default;
	virtual ~IObserver() = default;

	virtual void OnNotify(const T& data) = 0;
};

