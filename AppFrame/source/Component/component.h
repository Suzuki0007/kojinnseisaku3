#pragma once
#include <concepts>
#include "../Handle/SafeHandle.h"

template <typename TOwner>
class Component
{
public:
	explicit Component(TOwner& owner) : _owner(&owner) {}
	virtual ~Component() = default;

	virtual bool Initialize() { return true; }
	virtual bool Terminate() { return true; }
	virtual bool Process() { return true; }
	virtual bool Render() { return true; }

	virtual void Update([[maybe_unused]] float deltaTime) {}

	void DrawModel()
	{
		if(_mHandle != -1)
		{
			MV1DrawModel(_mHandle);
		}
	}

protected:
	handle::ModelHandle _mHandle{ -1 };
	TOwner* _owner;
};

