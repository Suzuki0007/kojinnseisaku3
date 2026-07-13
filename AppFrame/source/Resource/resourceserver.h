#pragma once
#include "pch.h"

class ResourceServer
{
public:

	// 文字列のハッシュ関数を定義する構造体
	struct StringViewHash
	{
		using is_transparent = void;// これを定義することで、unordered_mapがstd::string_viewをキーとして直接検索できるようになる
		// std::string_viewのハッシュ値を計算する関数
		size_t operator()(std::string_view sv) const
		{
			// std::string_viewのハッシュ値を計算する関数
			return std::hash<std::string_view>{}(sv);
		}
		// std::stringのハッシュ値を計算する関数
		size_t operator()(const std::string& s) const
		{
			return std::hash<std::string_view>{}(s);
		}
	};

	ResourceServer() = delete;

	using Storage = std::unordered_map<
		std::string,				// ファイルパスをキーとする
		int,						// 読み込んだリソースのハンドルを値とする
		StringViewHash,				// カスタムハッシュ関数を使用
		std::equal_to<>				// 文字列の比較用
	>;

	template<auto LoadFunc, typename... Args>
	static int GetHandle(const std::string_view filePath, Args&&... args)
	{
		static Storage storage;

		auto it = storage.find(filePath);
		if(it != storage.end())
		{
			return it->second; // すでに読み込まれている場合はハンドルを返す
		}

		std::string path(filePath); // string_viewをstringに変換
		int handle = -1;

		// もし呼び出そうとしているのが「DxLib::LoadGraph」だったら、自動的に後ろに TRUE を足して呼び出す！
		// std::is_same_v で型を比較して、LoadFunc が int( * )( const TCHAR*, int ) 型かどうかを判定する
		if constexpr(std::is_same_v<decltype( LoadFunc ), int( * )( const TCHAR*, int )>)
		{
			handle = LoadFunc(path.c_str(), TRUE);
		}
		else
		{
			handle = LoadFunc(path.c_str()); // それ以外の関数（LoadSoundMem等）はそのまま引数1つで呼ぶ
		}

		if(handle != -1)
		{
			storage.emplace(std::move(path), handle);
		}
		return handle;
	}

	template<>
	static int GetHandle<DxLib::MV1LoadModel>(const std::string_view filePath)
	{
		static Storage storage;

		auto it = storage.find(filePath);
		if(it != storage.end())
		{
			// モデルを複製して返す
			return DxLib::MV1DuplicateModel(it->second);
		}

		std::string path(filePath);
		int masterHandle = DxLib::MV1LoadModel(path.c_str()); // モデルを読み込む
		if(masterHandle != -1)
		{
			storage.emplace(std::move(path), masterHandle); // 安全に保存
			return DxLib::MV1DuplicateModel(masterHandle);  // 複製したものを返す
		}
		return -1;
	}
};