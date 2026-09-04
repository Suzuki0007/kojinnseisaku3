#pragma once
#include "pch.h"

class CharaBase;

struct HitInfo
{
	CharaBase* attacker;
	CharaBase* target;
	float damage;
	Vec4 hitPos;
	float hitStopDuration = 1.0f; // ヒットストップの持続時間
};