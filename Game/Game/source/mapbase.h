#pragma once
#include "objectbase.h"

class ShadowMapComponent;

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

	auto GetShadowMapComponent() const { return _shadowMapComponent; }

	auto GetCamera() const { return _cam; }
	void SetCamera(Camera* cam) override { _cam = cam; }

	auto& GetHandleMap() { return _handle_map; }
	auto GetHandleSkySphere() const { return _handle_sky_sphere; }
	auto GetFrameMapCollision() const { return _frame_map_collision; }

protected:
	// マップ用
// しまって取り出すときにstd::mapで管理
	int _handle_map { -1 };// std::map<マップ名, ハンドル>
	int _handle_sky_sphere { -1 };
	int _frame_map_collision { -1 };

	ShadowMapComponent* _shadowMapComponent{ nullptr };
};

