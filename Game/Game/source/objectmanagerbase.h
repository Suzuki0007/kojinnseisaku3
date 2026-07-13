#pragma once
#include <memory>
#include <string>
#include "objectfactory.h"

template<typename T, typename Factory = ObjectFactory<T>>
class ObjectManagerBase
{
public:
	using Creator = typename Factory::Creator;
	void RegisterType(const std::string& type, Creator creator)
	{
		_factory.RegisterType(type, std::move(creator));
	}

	void Create(const std::string& type)
	{
		auto object = _factory.Create(type);
		if(object != nullptr)
		{
			_object.emplace_back(std::move(object));
		}
	}

	T* Get(size_t index = 0) const
	{
		if(index >= _object.size())
		{
			return nullptr;
		}
		return _object[index].get(); 
	}
	std::vector<std::unique_ptr<T>>& GetAll() { return _object; }
	const std::vector<std::unique_ptr<T>>& GetOwner() { return _object; }

protected:
	ObjectManagerBase() = default;
	virtual ~ObjectManagerBase() = default;
	Factory _factory;// オブジェクトのファクトリー
	std::vector<std::unique_ptr<T>> _object;// 管理するオブジェクト
};

