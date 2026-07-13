#pragma once
#include "mapbase.h"

namespace map
{
	static constexpr auto DEFAULT_SIZE = 100.0f;
	static constexpr auto GROUND_Z = 50.0f;
	static constexpr auto GROUND_X = 100.0f;
	static constexpr auto GROUND_POLYGON_SIZE = 300.0f;

	static constexpr std::array<Vec4, 4> GROUND_POS_LIST =
	{
		Vec4(0.0f, 0.0f, 0.0f),
		Vec4(0.0f, 0.0f, GROUND_POLYGON_SIZE),
		Vec4(GROUND_POLYGON_SIZE, 0.0f, 0.0f),
		Vec4(GROUND_POLYGON_SIZE, 0.0f, GROUND_POLYGON_SIZE)
	};

	static constexpr std::array<float, 4> U_LIST =
	{
		0.0f,
		0.0f,
		1.0f,
		1.0f
	};

	static constexpr std::array<float, 4> V_LIST =
	{
		0.0f,
		1.0f,
		0.0f,
		1.0f
	};

	static constexpr Vec4 GROUND_NORMAL = Vec4(0.0f, 1.0f, 0.0f);
}

class MapRenderComponent : public Component<MapBase>
{
public:
	using Component<MapBase>::Component;

	MapRenderComponent(MapBase* owener = nullptr);
	virtual ~MapRenderComponent() = default;

	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;

private:
	handle::GraphHandle _groundHandle{ -1 };
	float _halfPolygonSize{ 0.0f };
	COLOR_U8 _diffuse { GetColorU8(255, 255, 255, 255) };
	COLOR_U8 _specular{ GetColorU8(0, 0, 0, 0) };

	float _startX{ 0.0f };
	float _startZ{ 0.0f };
	std::vector<VERTEX3D> _groundVertex { };
	std::vector<unsigned short> _groundIndex { };
};

