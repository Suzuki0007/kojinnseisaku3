#pragma once

/*
* toluaにも読み込まれるヘッダファイル
*/

#include "ModeGame.h"

// ↓ここからtoluaで使われる
// TOLUA_BEGIN

enum
{
	ENEMYCOUNT = ENEMY_COUNT
};

void _LOG(const char* msg);
int	GetPad();
bool IsEnemyAlive(int enemyId);
float GetEnemyHP(int index);
void DrawEnemyStatus(int x, int y, int r, int g, int b, const char* FormatString, int id, double hp);
void DrawChara(int x, int y, int r, int g, int b, const char* FormatString);
const char* GetCharaClassName(int charaType, int charaId);
void SetCharaAttack(int charaType, int charaId);
float GetCharaSpeed(int charaType, int charaId);


// TOLUA_END
// ↑ここまでtoluaで使われる

