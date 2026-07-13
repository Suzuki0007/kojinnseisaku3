#pragma once

template<std::floating_point T>
class Quaternion
{
public:
	T x{};
	T y{};
	T z{};
	T w{ 1 };

	cosntexpr Quaternion() noexcept = default;

	constexpr Quaternion(T x, T y, T z, T w) noexcept
		: x(x), y(y), z(z), w(w)
	{
	}

	// 単位四元数の生成
	[[nodiscard]] constexpr Quaternion Identity() noexcept
	{
		return { 0, 0, 0, 1 };
	}

	// 四元数の長さの2乗を計算する関数
	[[nodiscard]] constexpr T LengthSquared() const noexcept
	{
		return x * x + y * y + z * z + w * w;
	}

	// 四元数の長さを計算する関数
	[[nodiscard]] constexpr T Length() const noexcept
	{
		return std::sqrt(LengthSquared());
	}

	[[nodiscard]] Quaternion Normalized() const noexcept
	{
		T len = Length();

		// 長さが非常に小さい場合は、単位四元数を返す
		// std::numeric_limits<T>::epsilon() は、型 T の最小の正の値を返す
		if(len < std::numeric_limits<T>::epsilon())
		{
			return Identity();
		}

		//	正規化された四元数を返す
		return { x / len, y / len, z / len, w / len };
	}

	// 四元数の共役(逆回転)を計算する関数
	[[nodiscard]] constexpr Quaternion Conjugate() const noexcept
	{
		return { -x, -y, -z, w };
	}

	// 四元数の逆数を計算する関数
	[[nodiscard]] constexpr Quaternion Inverse() const noexcept
	{
		T lenSq = LengthSquared();

		if(lenSq < std::numeric_limits<T>::epsilon())
		{
			return Identity();
		}
		return Conjugate() / lenSq;
	}

	// 四元数の加算演算子をオーバーロードする関数
	[[nodiscard]] constexpr Quaternion operator+(const Quaternion& rhs) const noexcept
	{
		return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
	}

	// 四元数の減算演算子をオーバーロードする関数
	[[nodiscard]] constexpr Quaternion operator-(const Quaternion& rhs) const noexcept
	{
		return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
	}

	// 四元数のスカラー乗算演算子をオーバーロードする関数
	[[nodiscard]] constexpr Quaternion operator*(T scalar) const noexcept
	{
		return { x * scalar, y * scalar, z * scalar, w * scalar };
	}

	// 四元数のスカラー除算演算子をオーバーロードする関数
	[[nodiscard]] constexpr Quaternion operator/(T scalar) const noexcept
	{
		return { x / scalar, y / scalar, z / scalar, w / scalar };
	}
	
	// 四元数の掛け算演算子をオーバーロードする関数
	[[nodiscard]] constexpr Quaternion operator*(const Quaternion& rhs) const noexcept
	{
		return 
		{
			w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
			w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
			w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
		};
	}

	constexpr Quaternion& operator*=(const Quaternion& rhs) noexcept
	{
		*this = *this * rhs;
		return *this;
	}

	[[nodiscard]] constexpr bool operator==(const Quaternion& rhs) const noexcept = default;

};

using Quat = Quaternion<float>;
