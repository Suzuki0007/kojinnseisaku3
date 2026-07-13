#pragma once
#include "objectbase.h"
#include "pch.h"

class Cube : public ObjectBase
{
	typedef ObjectBase base;
public:
	enum class FACE_TYPE
	{
		FRONT,
		Right,
		BACK,
		LEFT,
		TOP,
		BOTTOM
	};

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	const mymath::AABB& GetAABB() const { return _aabb; }

	bool UpdateAABB();

	size_t GetVertexCount() const { return _cube_vertex.size(); }

	// 位置操作
	const Vec4& GetPosition() const { return _cube_pos; }
	void SetPosition(const Vec4& pos) { _cube_pos = pos; }

protected:
	int _cube_handle;
	std::vector<VERTEX3D> _cube_vertex;
	std::vector<unsigned short> _cube_index;

	// 頂点用の固定情報
	Vec4 _cube_normal_front;
	Vec4 _cube_normal_right;
	Vec4 _cube_normal_back;
	Vec4 _cube_normal_left;
	Vec4 _cube_normal_top;
	Vec4 _cube_normal_bottom;
	std::array<Vec4, 6> _cube_normal_list;
	// 基本の8頂点の座標
	Vec4 _cube_pos_0;
	Vec4 _cube_pos_1;
	Vec4 _cube_pos_2;
	Vec4 _cube_pos_3;
	Vec4 _cube_pos_4;
	Vec4 _cube_pos_5;
	Vec4 _cube_pos_6;
	Vec4 _cube_pos_7;
	std::array<Vec4, 4> _cube_pos_front;
	std::array<Vec4, 4> _cube_pos_right;
	std::array<Vec4, 4> _cube_pos_back;
	std::array<Vec4, 4> _cube_pos_left;
	std::array<Vec4, 4> _cube_pos_top;
	std::array<Vec4, 4> _cube_pos_bottom;

	std::vector<std::array<Vec4, 4>> _cube_pos_list;

	Vec4 _cube_pos;

	mymath::AABB _aabb;

};

