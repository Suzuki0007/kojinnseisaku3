#include "pch.h"
#include "maprendercomponent.h"
#include "resourcepath.h"

MapRenderComponent::MapRenderComponent(MapBase* owner)
	: Component<MapBase>(*owner)
{}

bool MapRenderComponent::Initialize()
{
	_groundHandle = Load::ImageGraph(path::Map("Plants"));
	_halfPolygonSize = map::GROUND_POLYGON_SIZE * 0.5f;
	_startX = -_halfPolygonSize * map::GROUND_X;
	_startZ = -_halfPolygonSize * map::GROUND_Z;

	return true;
}

bool MapRenderComponent::Terminate()
{
	return true;
}

bool MapRenderComponent::Process()
{
	size_t totalCell = static_cast<size_t>(map::GROUND_X * map::GROUND_Z);
	_groundVertex.resize(totalCell * 4);
	_groundIndex.resize(totalCell * 6);

	size_t vIndex = 0;
	size_t iIndex = 0;

	int limit = static_cast<int>(totalCell);
	for(int i = 0; i < limit; ++i)
	{
		int z = i / static_cast<int>(map::GROUND_X);
		int x = i % static_cast<int>(map::GROUND_X);

		auto offsetX = _startX + x * map::GROUND_POLYGON_SIZE;
		auto offsetZ = _startZ + z * map::GROUND_POLYGON_SIZE;

		for(int j = 0; j < 4; ++j)
		{
			Vec4 pos = map::GROUND_POS_LIST[j];
			pos.x += offsetX;
			pos.z += offsetZ;
			_groundVertex[vIndex++] = VERTEX3D
			{
				.pos = VC::VecToDxLib(pos),
				.norm = VC::VecToDxLib(map::GROUND_NORMAL),
				.dif = _diffuse,
				.spc = _specular,
				.u = map::U_LIST[j],
				.v = map::V_LIST[j]
			};
		}

		auto base = static_cast<unsigned short>(vIndex - 4);
		for(const auto offset : { 0, 1, 2, 2, 1, 3 })
		{
			_groundIndex[iIndex++] = base + offset;
		}
	}

	return true;
}

bool MapRenderComponent::Render()
{
	if(_groundHandle == -1)
	{
		return false;
	}

	auto vertexNum = static_cast<int>(_groundVertex.size());
	auto indexNum = static_cast<int>(_groundIndex.size());

	if(3 > vertexNum || 3 > indexNum)
	{
		return false;
	}

	auto polygonNum = indexNum / 3;
	DrawPolygonIndexed3D(_groundVertex.data(), vertexNum, _groundIndex.data(), polygonNum, _groundHandle, TRUE);
	return true;
}