#pragma once
#include "vector4.h"
#include "matrix4.h"

namespace v
{
	// ベクトルの生成
	template<typename T>
	inline Vec4 VGet(T x, T y, T z, T w = 1.0f)
	{
		return Vec4(x, y, z, w);
	}

	// ベクトルの加算
	inline Vec4 VAdd(const Vec4& v1, const Vec4& v2)
	{
		return v1 + v2;
	}

	// ベクトルの減算
	inline Vec4 VSub(const Vec4& v1, const Vec4& v2)
	{
		return v1 - v2;
	}

	// ベクトルとスカラーの乗算
	template<typename T>
	inline Vec4 VScale(const Vec4& v, T scalar)
	{
		return v * scalar;
	}

	// ベクトルとスカラーの除算
	template<typename T>
	inline Vec4 VDiv(const Vec4& v, T scalar)
	{
		return v / scalar;
	}

	// ベクトルの長さ
	inline float VSize(const Vec4& v)
	{
		return v.Length();
	}

	// 正規化
	inline Vec4 VNorm(const Vec4& v)
	{
		return v.Normalized();
	}

	
	inline Vec4 VTransform(const Vec4& vec, const Mat4& mat) noexcept
	{
		return vec * mat;
	}

	inline float Dot(const Vec4& v1, const Vec4& v2)
	{
		return v1.Dot(v2);
	}

	inline Vec4 Cross(const Vec4& v1, const Vec4& v2)
	{
		return v1.Cross(v2);
	}
}