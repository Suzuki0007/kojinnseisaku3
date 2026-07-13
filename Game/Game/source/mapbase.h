#pragma once
#include "objectbase.h"

class MapBase : public ObjectBase, public IComponentBindable<MapBase>
{
	typedef ObjectBase base;
public:

	MapBase() = default;
	virtual ~MapBase() = default;

	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	void SetCamera(Camera* cam) override { _cam = cam; }
};

