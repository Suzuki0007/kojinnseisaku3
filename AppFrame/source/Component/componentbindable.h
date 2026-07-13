#pragma once
#include "componentowner.h"

template <class TOwner>
class IComponentBindable
{
	friend class IComponentBindable<TOwner>;
public:
	virtual ~IComponentBindable() = default;

	template <class T, class ... Args>
	T* AddComponent(Args&& ... args)
	{
		auto* derived = static_cast<TOwner*>(this);
		return _comOwner.AddComponent<T>(*derived, std::forward<Args>(args)...);
	}

	void UpdateComponent(float deltaTime)
	{
		_comOwner.Update(deltaTime);
	}

	void RenderComponent()
	{
		_comOwner.Render();
	}

	void ClearComponent()
	{
		_comOwner.Clear();
	}

	//template <class T>
	//T* GetComponent()
	//{
	//	for(auto& component : _comOwner._component)
	//	{
	//		if(auto derived = dynamic_cast< T* >( component.get() ))
	//		{
	//			return derived;
	//		}
	//	}
	//	return nullptr;
	//}

protected:
	ComponentOwner<TOwner> _comOwner;
};