#pragma once
#include "pch.h"
#include "observer.h"

template<typename T>
class Subject
{
public:
	using ObserverType = IObserver<T>;

	// 登録
	void Subscribe(ObserverType* observer)
	{
		_observer.push_back(observer);
	}

	// 登録解除
	void Unsubscribe(ObserverType* observer)
	{
		std::erase(_observer, observer);
	}

	// 通知
	void Notify(const T& data)
	{
		for(auto* observer : _observer)
		{
			observer->OnNotify(data);
		}
	}

private:
	std::vector<ObserverType*> _observer;
};

