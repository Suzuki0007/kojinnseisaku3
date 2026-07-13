/*
** toluaでluaに追加するコマンド群
*/

#include "pch.h"
#include	"lua.hpp"
#include	"lua_cmd.h"

#include "ApplicationMain.h"
#include "PlayerManager.h"

void _LOG(const char* msg)
{
	if(!msg) return;

	// const char* を std::u8string_view に安全にキャスト
	std::u8string_view u8msg(reinterpret_cast<const char8_t*>(msg));

	// luaから来る文字列はUTF-8, Win32APIはS-JISなので変換
	std::string sSJIS = Utf8Sjis::ConvUTF8toSJIS(u8msg);
	OutputDebugString(sSJIS.c_str());		// Win32API, デバッグコンソールにメッセージを出す
}

// ModeGameにアクセスするため
static ModeGame* MODEGAME() 
{
	return (ModeGame*)(ModeServer::GetInstance()->Get("game"));
}

// パッド情報を得る
int	GetPad()
{
	int key = ApplicationMain::GetInstance()->GetKey();
	return key;
}

// 敵が生きているかを判定する関数
bool IsEnemyAlive(int enemyId)
{
	auto* game = MODEGAME();
	if(!game) { return false; }

	//auto& enemy = game->GetEnemies();
	//if(enemyId >= 0 && enemyId < static_cast<int>(enemy.size()))
	//{
	//	return enemy[enemyId]->IsAlive();
	//}
	//return false;

	return game->IsEnemyAliveFromList(enemyId);
}

// 敵のHPを取得する関数
float GetEnemyHP(int index)
{
	auto* game = MODEGAME();
	if(!game) { return 0.0f; }

	auto& enemies = game->GetEnemies();
	if(index >= 0 && index < static_cast<int>(enemies.size()))
	{
		return enemies[index]->GetHP();
	}
	return 0.0f;
}

void DrawEnemyStatus(int x, int y, int r, int g, int b, const char* FormatString, int id, double hp)
{
	unsigned int color = GetColor(r, g, b);

	// DxLibのDrawFormatStringにそのまま全ての引数を丸投げします！
	// これにより、Lua側が %d で送ってきたデータを完璧に描画できます。
	DrawFormatString(x, y, color, FormatString, id, hp);
}

void DrawChara(int x, int y, int r, int g, int b, const char* FormatString)
{
	if(!FormatString) return;

	// Lua(UTF-8) から DXライブラリ(S-JIS) への文字コード変換
	std::u8string_view u8msg(reinterpret_cast<const char8_t*>(FormatString));
	std::string sSJIS = Utf8Sjis::ConvUTF8toSJIS(u8msg);

	// DXライブラリの関数で画面に描画
	DrawFormatString(x, y, GetColor(r, g, b), "%s", u8msg.data());
}

const char* GetCharaClassName(int charaType, int charaId)
{
	if(charaType == 0) // プレイヤー
	{
		auto& player = PlayerManager::GetInstance()->GetPlayer();
		for(auto& p : player)
		{
			if(p->GetCharaId() == charaId)
			{
				return p->GetCharaClassName();
			}
		}
	}
	else if(charaType == 1) // 敵
	{
		auto& enemies = EnemyManager::GetInstance()->GetEnemies();
		for(auto& enemy : enemies)
		{
			if(enemy && enemy->GetCharaId() == charaId)
			{
				return enemy->GetCharaClassName(); // "Enemy" が返る
			}
		}
	}
	return "Unknown"; // 見つからなかった場合のデフォルト値
}

void SetCharaAttack(int charaType, int charaId)
{
	if(charaType == 0) // プレイヤー
	{
		auto& player = PlayerManager::GetInstance()->GetPlayer();
		for(auto& p : player)
		{
			if(p->GetCharaId() == charaId)
			{
				p->CommandAttack(nullptr); // 攻撃コマンドを実行（ターゲットはnullptr）
				return;
			}
		}
	}
	else if(charaType == 1) // 敵
	{
		auto& enemies = EnemyManager::GetInstance()->GetEnemies();
		for(auto& enemy : enemies)
		{
			if(enemy && enemy->GetCharaId() == charaId)
			{
				enemy->CommandAttack(nullptr); // 攻撃コマンドを実行（ターゲットはnullptr）
				return;
			}
		}
	}
}

float GetCharaSpeed(int charaType, int charaId)
{
	if(charaType == 0) // プレイヤー
	{
		auto& player = PlayerManager::GetInstance()->GetPlayer();
		for(auto& p : player)
		{
			if(p->GetCharaId() == charaId)
			{
				return p->GetSpeed(); // プレイヤーの速度を返す
			}
		}
	}
	else if(charaType == 1) // 敵
	{
		auto& enemies = EnemyManager::GetInstance()->GetEnemies();
		for(auto& enemy : enemies)
		{
			if(enemy && enemy->GetCharaId() == charaId)
			{
				return enemy->GetSpeed();
			}
		}
	}
	return 0.0f; // 見つからなかった場合のデフォルト値
}