#pragma once
#include "observer.h"
#include "subject.h"

template<typename T>
class ObserverBase : public IObserver<T>
{
public:
	ObserverBase(Subject<T>& subject) : _subject(subject)
	{
		_subject.Subscribe(this);
	}

	~ObserverBase()
	{
		_subject.Unsubscribe(this);
	}

	ObserverBase(const ObserverBase&) = delete;
	ObserverBase& operator=(const ObserverBase&) = delete;

protected:
	Subject<T>& _subject;
};

