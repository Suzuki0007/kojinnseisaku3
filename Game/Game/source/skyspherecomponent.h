#pragma once
#include "mapbase.h"

class SkySphereComponent : public Component<MapBase>
{
public:
	SkySphereComponent() = default;
	virtual ~SkySphereComponent() override;

	virtual bool Initialize() override;
	virtual bool Process() override;
	virtual bool Render() override;
};

