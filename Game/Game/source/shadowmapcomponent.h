#pragma once
#include "mapbase.h"

class ShadowMapComponent : public Component<MapBase>
{
	using base = Component<MapBase>;

public:

	explicit ShadowMapComponent(MapBase& owner)
		: base(owner)
	{}
	virtual ~ShadowMapComponent() = default;
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	void Begin(const Camera& camera);
	void End();

	int GetShadowMapHandle() const { return _shadowMapHandle; }

protected:
	handle::ShadowMapHandle _shadowMapHandle{ -1 };
};

