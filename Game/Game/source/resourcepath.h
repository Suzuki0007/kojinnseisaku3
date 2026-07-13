#pragma once
#include "pch.h"

// std::string_view_literalsとはstd::string_view型を一瞬で作成することができる。
using namespace std::string_view_literals;

class ResourcePath
{
public:
	using Record = std::pair<std::string_view, std::string_view>;

	ResourcePath() = delete;

	inline constexpr static std::array Enemy =
	{
		// svはstring_viewのリテラルを作るためのもの
		std::make_pair("Enemy"sv, "res/PoorEnemyMelee/PoorEnemy.mv1"sv),
	};

	inline constexpr static std::array Player =
	{
		std::make_pair("Player"sv, "res/Player/Player.mv1"sv),
		std::make_pair("Player2"sv, "res/Player/Player.mv1"sv),
		std::make_pair("Player3"sv, "res/Player/Player.mv1"sv),
	};

	inline constexpr static std::array Map =
	{
		std::make_pair("Plants"sv, "res/Texture/Groundplants1_D.jpg"sv),
		std::make_pair("SkySphere"sv, "res/SkySphere/skysphere.mv1"sv),
	};

	constexpr static std::string_view GetEnemyPath(std::string_view name)
	{
		return FindPath(Enemy, name);
	}

	constexpr static std::string_view GetPlayerPath(std::string_view name)
	{
		return FindPath(Player, name);
	}

	constexpr static std::string_view GetMapPath(std::string_view name)
	{
		return FindPath(Map, name);
	}

	// 配列の中から指定された名前に対応するパスを返す関数
	template<std::size_t N>
	constexpr static std::string_view FindPath
	(
		const std::array<Record, N>& records, // 配列の参照
		std::string_view name //	検索する名前
	)
	{
		for(const auto& [key, path] : records)
		{
			if(key == name)
			{
				return path;
			}
		}
		return ""sv; // 見つからなかった場合は空文字列を返す
	}

};

namespace path
{ 
	// エネミーのあだ名
	inline constexpr auto Enemy = [](std::string_view name)
	{
		return ResourcePath::GetEnemyPath(name);
	};

	// プレイヤーのあだ名
	inline constexpr auto Player = [](std::string_view name)
	{
		return ResourcePath::GetPlayerPath(name);
	};

	// マップのあだ名
	inline constexpr auto Map = [](std::string_view name)
	{
		return ResourcePath::GetMapPath(name);
	};
}