#pragma once
#include <cmath>
#include <algorithm>
#include <string>
#include <format>
#include <concepts>

#define NDCE [[nodiscard]] constexpr

template<typename T>
// Tが整数型または浮動小数点型であることを要求するコンセプト
requires std::integral<T> || std::floating_point<T>
class Vector4
{
public:
	//T x{ 0 };
	//T y{ 0 };
	//T z{ 0 };
	//T w{ 0 };

	constexpr Vector4() : data{ 0, 0, 0, 0 }
	{
	}

	constexpr Vector4(T x, T y, T z, T w = {T(1)})
		: data{ x, y, z, w }
	{
	}

	union
	{
		struct
		{
			T x;
			T y;
			T z;
			T w;
		};
		std::array<T, 4> data;
	};

	// 演算子オーバーロード
	// ベクトル同士の加算
	NDCE Vector4 operator+(const Vector4& other) const
	{
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	// ベクトル同士の減算
	NDCE Vector4 operator-(const Vector4& other) const
	{
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	// ベクトルとスカラーの乗算
	NDCE Vector4 operator*(T scalar) const
	{
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}
	// ベクトルとスカラーの除算
	NDCE Vector4 operator/(T scalar) const
	{
		if constexpr(std::floating_point<T>)
		{
			// 浮動小数点型の場合、ゼロ除算を防ぐために絶対値が小さい場合はゼロベクトルを返す
			// std::numeric_limits<T>::epsilon()は、T型の最小の正の値で、これより小さい値はゼロとみなされる
			if(std::abs(scalar) < std::numeric_limits<T>::epsilon())
			{
				return Zero();
			}
			else
			{
				if(scalar == 0)
				{
					return Zero();

				}
			}
		}
		return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	// 加算代入演算子
	constexpr Vector4& operator+=(const Vector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	// 減算代入演算子
	constexpr Vector4& operator-=(const Vector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	constexpr T& operator[](size_t index)
	{
		return data[index];
	}

	constexpr const T& operator[](size_t index) const
	{
		return data[index];
	}

	// 比較演算子の自動生成
	NDCE bool operator==(const Vector4&) const = default;

	// ベクトルの長さ
	NDCE T Length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}
	
	// ベクトルの長さの2乗（高速判定用）
	NDCE T LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	// 正規化
	NDCE Vector4 Normalized() const
	{
		auto len = Length();
		if(len < std::numeric_limits<T>::epsilon())
		{
			return Zero();
		}
		return *this / len;
	}

	// 内積
	NDCE T Dot(const Vector4& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	// 外積
	NDCE Vector4 Cross(const Vector4& other, T resultW = {0}) const
	{
		return Vector4
		(
			(y * other.z) - (z * other.y),
			(z * other.x) - (x * other.z),
			(x * other.y) - (y * other.x),
			resultW
		);
	}

	// 同次座標の正規化
	NDCE Vector4 Homogenize() const
	{
		if(w == 0)
		{
			return *this;
		}
		return *this / w;
	}

	// 2点間の距離
	NDCE static T Distance(const Vector4& a, const Vector4& b)
	{
		return (a - b).Length();
	}

	// 2点間の距離の2乗（軽量判定用)
	NDCE static T DistanceSquared(const Vector4& a, const Vector4& b)
	{
		return (a - b).LengthSquared();
	}

	// 線形補間 (Lerp)
	NDCE static Vector4 Lerp(const Vector4& a, const Vector4& b, T t)
	{
		return a + (b - a) * t;
	}

	// 反射ベクトル
	NDCE static Vector4 Reflect(const Vector4& velocity, const Vector4& normal)
	{
		return velocity - normal * (T{ 2 } * velocity.Dot(normal));
	}

	// 射影ベクトル
	NDCE static Vector4 Project(const Vector4& vector, const Vector4& onto)
	{
		T ontoLengthSquared = onto.LengthSquared();
		if(ontoLengthSquared < std::numeric_limits<T>::epsilon())
		{
			return Zero();
		}
		T projectionScale = vector.Dot(onto) / ontoLengthSquared;
		return onto * projectionScale;
	}

	// 各成分の最小値
	NDCE static Vector4 Min(const Vector4& a, const Vector4& b)
	{
		return
		{
			(std::ranges::min)(a.x, b.x),
			(std::ranges::min)(a.y, b.y),
			(std::ranges::min)(a.z, b.z),
			(std::ranges::min)(a.w, b.w)
		};
	}
	// 各成分の最大値
	NDCE static Vector4 Max(const Vector4& a, const Vector4& b)
	{
		return Vector4
		(
			(std::ranges::max)(a.x, b.x),
			(std::ranges::max)(a.y, b.y),
			(std::ranges::max)(a.z, b.z),
			(std::ranges::max)(a.w, b.w)
		);
	}

	// 各成分のクランプ
	NDCE static Vector4 Clamp(const Vector4& value, const Vector4& min, const Vector4& max)
	{
		return Vector4
		(
			(std::ranges::clamp)(value.x, min.x, max.x),
			(std::ranges::clamp)(value.y, min.y, max.y),
			(std::ranges::clamp)(value.z, min.z, max.z),
			(std::ranges::clamp)(value.w, min.w, max.w)
		);
	}

	// 文字列変換
	[[nodiscard]] std::string ToString() const
	{
		if constexpr(std::floating_point<T>)
		{
			return std::format("({:.2f}, {:.2f}, {:.2f}, {:.2f})", x, y, z, w);

		}
		else
		{
			return std::format("Vector4({}, {}, {}, {})", x, y, z, w);
		}
	}

	// ゼロベクトル
	NDCE static Vector4 Zero()
	{
		return Vector4(0, 0, 0, 0);
	}
	// 単位ベクトル
	NDCE static Vector4 One()
	{
		return Vector4(1, 1, 1, 1);
	}
	// X軸単位ベクトル
	NDCE static Vector4 UnitX()
	{
		return Vector4(1, 0, 0, 0);
	}
	// Y軸単位ベクトル
	NDCE static Vector4 UnitY()
	{
		return Vector4(0, 1, 0, 0);
	}
	// Z軸単位ベクトル
	NDCE static Vector4 UnitZ()
	{
		return Vector4(0, 0, 1, 0);
	}

};

using Vec4 = Vector4<float>;
using Vec4d = Vector4<double>;
using Vec4i = Vector4<int>;