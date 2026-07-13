#include "pch.h"
#include "CFile.h"
#include "../Application/UtilMacro.h"


// 読み込み
CFile::CFile(const std::filesystem::path& filename)
	:_filename(filename)
{
	// ファイルを開く
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);	// 入力をバイナリ形式で
	if (!ifs)
	{
		// 開くの失敗
		return;
	}

	// ファイルサイズの取得
	ifs.seekg(0, std::ios_base::end);
	size_t size = ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);

	if (size == 0)
	{
		// ファイルが空
		return;
	}

	// データの読み込み
	_data.resize_and_overwrite(size, [ & ] (char* buf, size_t buf_size)
		{
			ifs.read
			(
				buf,// 読み込むバッファ
				static_cast< std::streamsize >( buf_size )// 読み込むサイズ
			);
			return buf_size;
		});

	// 処理成功
	_success = true;
}

// 書き込み
CFile::CFile(const std::filesystem::path& filename, std::span<const std::byte> data)
	:_filename(filename)
{


	// ファイルを開く
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);	// 出力をバイナリ形式で
	if (!ofs) 
	{
		// 開くの失敗
		return;
	}

	// データの書き込み
	// reinterpret_cast<const char*>で、std::byte*をchar*に変換してから書き込む
	ofs.write(reinterpret_cast<const char*>(data.data()), data.size_bytes());

	// 処理成功
	_success = true;
}

CFile::CFile(const std::filesystem::path& filename, std::string_view writestr)
	:_filename(filename)
{
	// ファイルを開く
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);	// 出力をバイナリ形式で
	if (!ofs)
	{
		// 開くの失敗
		return;
	}
	// データの書き込み
	ofs.write(writestr.data(),writestr.size());
	// 処理成功
	_success = true;
}