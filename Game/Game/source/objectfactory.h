#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

template<typename T>
class ObjectFactory
{
public:
	using Creator = std::function<std::unique_ptr<T>()>;

	void RegisterType(const std::string& type, Creator creator)
	{
		_creators[type] = creator;
	}

	std::unique_ptr<T> Create(const std::string& type)
	{
		auto it = _creators.find(type);
		if(it != _creators.end())
		{
			return it->second();
		}
		return nullptr;
	}

protected:
	ObjectFactory() = default;
	virtual ~ObjectFactory() = default;

	std::unordered_map<std::string, Creator> _creators;
};

