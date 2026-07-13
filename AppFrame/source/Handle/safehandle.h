#pragma once

template <void (*DeleteFunc)(int)>
class SafeHandle
{
public:
	SafeHandle() = delete;
	SafeHandle(int handle) : _handle(handle) {}

	~SafeHandle() { Reset(); }

	// コピー禁止
	SafeHandle(const SafeHandle&) = delete;
	SafeHandle& operator=(const SafeHandle&) = delete;

	// ムーブ許可
	SafeHandle(SafeHandle&& other) noexcept : _handle(other._handle)
	{
		other._handle = -1;
	}
	SafeHandle& operator=(SafeHandle&& other) noexcept
	{
		if(this != &other)
		{
			Reset();
			_handle = other._handle;
			other._handle = -1;
		}
		return *this;
	}

	// int型として扱えるようにする
	SafeHandle& operator=(int handle)
	{
		Reset();
		_handle = handle;
		return *this;
	}

	// int型として扱えるようにする
	// これにより、SafeHandleオブジェクトをint型の変数に代入することができる
	operator int() const { return _handle; }

	// ハンドルをリセットする
	void Reset()
	{
		if(_handle != -1)
		{
			DeleteFunc(_handle);
			_handle = -1;
		}
	}

private:
	int _handle { -1 };
};

namespace handle
{
	inline void ModelDelete(int handle) { DxLib::MV1DeleteModel(handle); }
	inline void GraphDelete(int handle) { DxLib::DeleteGraph(handle); }
	inline void ShadowMapDelete(int handle) { DxLib::DeleteShadowMap(handle); }

	using ModelHandle = SafeHandle<ModelDelete>;
	using GraphHandle = SafeHandle<GraphDelete>;
	using ShadowMapHandle = SafeHandle<ShadowMapDelete>;
}