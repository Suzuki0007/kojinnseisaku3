#pragma once
#include "mapbase.h"

class ShadowMapComponent : public Component<MapBase>
{
public:
	using Component<MapBase>::Component;

	ShadowMapComponent(MapBase& owner) : Component<MapBase>(owner) {}
	virtual ~ShadowMapComponent() = default;
	virtual bool Initialize() override;
	virtual bool Process() override;
	virtual bool Render() override;

protected:
	handle::ShadowMapHandle _shadowMapHandle{ -1 };
};

