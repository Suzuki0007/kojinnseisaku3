#pragma once
#include "pch.h"
#include <concepts>
#include <string>
#include <format>
#include <ranges>
#include <array>
#include "vector4.h"

#define NDSTCE [[nodiscard]] static constexpr
#define NDFRCE [[nodiscard]] friend constexpr

template <typename T>
	requires std::integral<T> || std::floating_point<T>
class Matrix4
{
public:
	std::array<std::array<T, 4>, 4> m;// 4x4の行列

	// noexcept:例外を投げないことを宣言するキーワード
	constexpr Matrix4() noexcept = default;

	// 行列の要素にアクセスするための演算子オーバーロード
	NDCE T& operator[](size_t row, size_t col)noexcept
	{
		return m[row][col];
	}

	NDCE const T& operator[](size_t row, size_t col) const noexcept
	{
		return m[row][col];
	}

	// 単位行列
	NDSTCE Matrix4 Identity() noexcept
	{
		// 1つ目の{}はMatrix4の初期化
		return
		{
			// 2つ目はメンバ変数である外側の std::arrayのカッコ
			{
				// 3つ目は内側のstd::arrayのカッコ
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 }
			}
		};
	}

	// ゼロ行列
	NDSTCE Matrix4 Zero() noexcept
	{
		return Matrix4{ {} };
	}

	// 平行移動行列の生成(バラバラに渡すバージョン)
	NDSTCE Matrix4 CreateTranslation(T x, T y, T z) noexcept
	{
		return Matrix4
		{
			{
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{x, y, z, 1}
			}
		};
	}

	// 平行移動行列の作成(ベクトルで渡すバージョン)
	NDSTCE Matrix4 CreateTranslation(const Vector4<T>& v) noexcept
	{
		return CreateTranslation(v.x, v.y, v.z);
	}

	// 拡大縮小行列の作成(バラバラに渡すバージョン)
	NDSTCE Matrix4 CreateScale(T x, T y, T z) noexcept
	{
		return Matrix4
		{
			{
				{x, 0, 0, 0},
				{0, y, 0, 0},
				{0, 0, z, 0},
				{0, 0, 0, 1}
			}
		};
	}

	// 拡大縮小行列の作成(ベクトルで渡すバージョン)
	NDSTCE Matrix4 CreateScale(const Vector4<T>& v) noexcept
	{
		return CreateScale(v.x, v.y, v.z);
	}

	// ベクトルと行列の掛け算
	NDFRCE Vector4<T> operator*(const Vector4<T>& v, const Matrix4 mat) noexcept
	{
		Vector4<T> result{};

		for(size_t c : std::views::iota(0u, 4u))
		{
			result[c] = v.x * mat[0, c] +
						v.y * mat[1, c] +
						v.z * mat[2, c] +
						v.w * mat[3, c];
		}
		return result;
	}

	// 行列同士の掛け算
	NDCE Matrix4 operator*(const Matrix4& rhs) const noexcept
	{
		Matrix4 result{};
		Matrix4& lhs = *this;

		for(auto [r, c] : std::views::cartesian_product(std::views::iota(0u, 4u), std::views::iota(0u, 4u)))
		{
			result[r, c] =	lhs[r, 0] * rhs[0, c] +
							lhs[r, 1] * rhs[1, c] +
							lhs[r, 2] * rhs[2, c] +
							lhs[r, 3] * rhs[3, c];
		}
		return result;
	}

	// 複合代入演算子(行列同士の掛け算)
	constexpr Matrix4& operator*=(const Matrix4& rhs) noexcept
	{
		*this = *this * rhs;
		return *this;
	}

	// 転置行列
	NDCE Matrix4 Transpose() const noexcept
	{
		Matrix4 result{};
		Matrix4 lhs = *this;

		for(auto [r, c] : std::views::cartesian_product(std::views::iota(0u, 4u), std::views::iota(0u, 4u)))
		{
			result[r, c] = lhs[c, r];
		}
		return result;
	}

	// X軸回転行列の作成
	NDSTCE Matrix4 CreateRotationX(T angle) noexcept
	{
		T cos = std::cos(angle);
		T sin = std::sin(angle);
		return Matrix4
		{
			{
				{1,	  0,    0, 0},
				{0,  cos, sin, 0},
				{0, -sin, cos, 0},
				{0,    0,   0, 1}
			}
		};
	}

	// Y軸回転行列の作成
	NDSTCE Matrix4 CreateRotationY(T angle) noexcept
	{
		T cos = std::cos(angle);
		T sin = std::sin(angle);
		return Matrix4
		{
			{
				{ cos, 0, -sin, 0},
				{   0, 1,    0, 0},
				{ sin, 0,  cos, 0},
				{   0, 0,    0, 1}
			}
		};
	}

	// Z軸回転行列の作成
	NDSTCE Matrix4 CreateRotationZ(T angle) noexcept
	{
		T cos = std::cos(angle);
		T sin = std::sin(angle);
		return Matrix4
		{
			{
				{ cos, sin, 0, 0},
				{-sin, cos, 0, 0},
				{   0,   0, 1, 0},
				{   0,   0, 0, 1}
			}
		};
	}

	//行列と数値の掛け算
	NDCE Matrix4 operator*(T factor) const noexcept
	{
		Matrix4 result{};
		Matrix4 lhs = *this;
		for(auto [r, c] : std::views::cartesian_product(std::views::iota(0u, 4u), std::views::iota(0u, 4u)))
		{
			result[r, c] = lhs[r, c] * factor;
		}
		return result;
	}

	// 数値と行列の掛け算
	NDFRCE Matrix4 operator*(T factor, const Matrix4& mat) noexcept
	{
		return mat * factor;
	}

	// 複合代入演算子(数値と行列の掛け算)
	constexpr Matrix4& operator*=(T factor) noexcept
	{
		for(auto [r, c] : std::views::cartesian_product(std::views::iota(0u, 4u), std::views::iota(0u, 4u)))
		{
			m[r][c] *= factor;
		}
		return *this;
	}

	using AffineComponent = std::tuple<Vector4<T>, Vector4<T>, Vector4<T>>;

	// 拡大縮小、回転、平行移動をまとめて行うアフィン変換行列の作成
	NDSTCE Matrix4 CreateAffine(const AffineComponent& srt) noexcept
	{
		auto& [s, r, t] = srt;
		
		Matrix4 Scale = CreateScale(s);
		Matrix4 RotationX = CreateRotationX(r.x);
		Matrix4 RotationY = CreateRotationY(r.y);
		Matrix4 RotationZ = CreateRotationZ(r.z);
		Matrix4 Translation = CreateTranslation(t);

		return Scale * RotationX * RotationY * RotationZ * Translation;
	}

	// 拡大縮小、回転、平行移動をまとめて行うアフィン変換行列の作成(ベクトルで渡すバージョン)
	NDSTCE Matrix4 CreateAffine(const Vector4<T>& s, const Vector4<T>& r, const Vector4<T>& t) noexcept
	{
		return CreateAffine({s, r, t});
	}
};

using Mat4 = Matrix4<float>;
using Mat4d = Matrix4<double>;
