#pragma once
#include <string>
#include <string_view>
#include <filesystem>
#include <span>

class CFile
{
public:
	// 読み込み
	explicit CFile(const std::filesystem::path& filename);
	// 書き込み
	CFile(const std::filesystem::path& filename, std::span<const std::byte> data);		// バイナリデータ
	CFile(const std::filesystem::path& filename, std::string_view writestr);	// 文字列データ

	~CFile() = default;
	std::filesystem::path Filename() { return _filename; }
	const char* Data() { return _data.data(); }
	std::string_view DataStr() const { return _data; }
	size_t Size() const { return _data.size(); }
	bool Success() { return _success; }

private:
	std::filesystem::path		_filename;
	std::string _data{};
	bool	_success { false };
};
