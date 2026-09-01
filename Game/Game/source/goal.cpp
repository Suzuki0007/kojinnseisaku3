#include "pch.h"
#include "goal.h"
#include "resourcepath.h"

namespace
{
	constexpr float GOAL_CX = 0.5f;
	constexpr float GOAL_CY = 0.5f;
	constexpr float GOAL_SIZE = 150.0f;
	constexpr float GOAL_ANGLE = 0.0f;
}

bool Goal::Initialize()
{
	base::Initialize();

	auto path = path::Goal("Goal");

	_handle = LoadGraph(path.data());

	// Goal の位置
	_pos = v::VGet(0.0f, 200.0f, 100.0f);

	// カプセルの設定
	_col_sub_y = 80.0f;
	_collision_r = 60.0f;
	_collision_weight = 100000.0f;

	return true;
}

bool Goal::Terminate()
{
	base::Terminate();

	if(_handle != -1)
	{
		DeleteGraph(_handle);
		_handle = -1;
	}

	return true;
}

bool Goal::Process()
{
	base::Process();
	return true;
}

bool Goal::Render()
{
	base::Render();

	VC::DrawBillboard
	(
		_pos,
		GOAL_CX,
		GOAL_CY,
		GOAL_SIZE,
		GOAL_ANGLE,
		_handle,
		TRUE
	);


	return true;
}