#pragma once
#include "componentbindable.h"

class Actor : public IComponentBindable<Actor>
{
public:
	Actor() = default;
	virtual ~Actor() = default;
	
	void Update(float deltaTime)
	{
		UpdateComponent(deltaTime);
	}

	void Render()
	{
		RenderComponent();
	}

	void Clear()
	{
		ClearComponent();
	}

};

