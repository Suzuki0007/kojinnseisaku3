#pragma once
#include "mapbase.h"

class Map : public MapBase 
{
	using base = MapBase;
public:
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;



protected:


	// 地面のテクスチャ
	int _ground_handle;
	std::vector<VERTEX3D> _ground_vertex;
	std::vector<unsigned short> _ground_index;

	// 地面全体の中心を原点にする
	float _start_x;
	float _start_z;
	// 頂点用の固定情報
	VECTOR _ground_normal;
	//地面の正方形用のテーブル
	VECTOR _ground_pos_0;
	VECTOR _ground_pos_1;
	VECTOR _ground_pos_2;
	VECTOR _ground_pos_3;
	std::array<VECTOR, 4> _ground_pos_list;

};

