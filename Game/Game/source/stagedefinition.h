#pragma once
#include "pch.h"

struct StageDefinition
{
	std::vector<Vec4> cubePos;
	std::vector<Vec4> enemyPos;
	Vec4 goalPos;
};

namespace stage
{
	inline StageDefinition CreateStage()
	{
        return
        {
            {
				// キューブの位置
                v::VGet(-1204.0f, 0.0f, 1700.0f),
                v::VGet(-1500.0f, 250.0f, -1600.0f),
                v::VGet(-950.0f, 0.0f, -1600.0f),
                v::VGet(-1200.0f, 0.0f, 1700.0f),
                v::VGet(1070.0f, 0.0f, -35.0f),
                v::VGet(750.0f, 200.0f, -35.0f),
                v::VGet(6000.0f, 0.0f, 1500.0f)
            },
            {
				// 敵の位置
                v::VGet(-1350.0f, 400.0f, -1600.0f),
                v::VGet(650.0f, 0.0f, 1500.0f),
                v::VGet(1100.0f, 0.0f, 150.0f),
                v::VGet(-1200.0f, 100.0f, 1700.0f),
                v::VGet(150.0f, 0.0f, -3700.0f),
                v::VGet(6000.0f, 100.0f, 1500.0f)
            },
            {
				// ゴールの位置
                v::VGet(0.0f, 0.0f, 0.0f)
            }
        };
	}
}