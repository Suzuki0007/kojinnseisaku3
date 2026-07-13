#pragma once
#include "quaternion.h"
#include "matrix4.h"
#include "vector4.h"

template<std::floating_point T>
class QuaternionMatVec
{
public:

	QuaternionMatVec() = default;
	virtual ~QuaternionMatVec() = default;

	NDCE T Dot(const Quaternion<T>& q1, const Quaternion<T>& q2) const noexcept
	{
		return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	}
	
	NDCE T Angle(const Quaternion<T>& q1, const Quaternion<T>& q2) const noexcept
	{
		T d = std::clamp(std::abs(Dot(q1, q2)), T(0), T(1));
		return T(2) * std::acos(d);
	}

	NDCE Matrix4<T> ToMatrix4(const Quaternion<T>& q) const noexcept
	{
		T xx = q.x * q.x;
		T yy = q.y * q.y;
		T zz = q.z * q.z;
		T xy = q.x * q.y;
		T xz = q.x * q.z;
		T yz = q.y * q.z;
		T wx = q.w * q.x;
		T wy = q.w * q.y;
		T wz = q.w * q.z;
		return Matrix4<T>
		{
			{
				{1 - 2 * ( yy + zz ),     2 * ( xy - wz ),     2 * ( xz + wy ), 0 },
				{    2 * ( xy + wz ), 1 - 2 * ( xx + zz ),     2 * ( yz - wx ), 0 },
				{    2 * ( xz - wy ),     2 * ( yz + wx ), 1 - 2 * ( xx + yy ), 0 },
				{                  0,                   0,                   0, 1 }
			}
		};
	}

	// 四元数を行列から生成する関数

	[[nodiscard]] Quaternion FromMatrix(const Matrix4<T>& m) const noexcept
	{
		Quaternion<T> q{};

		T trace = m[0, 0] + m[1, 1] + m[2, 2];

		if(trace > 0)
		{
			T s = std::sqrt(trace + T(1)) * T(2);// s = 2w * 2 = 4w
			return 
			{
				( m[2, 1] - m[1, 2] ) / s ,
				( m[0, 2] - m[2, 0] ) / s ,
				( m[1, 0] - m[0, 1] ) / s ,
				s* T(0.25)// 0.25は元の値に戻すために必要
			}
		}

		int i = 0;
		if(m[1, 1] > m[0, 0])
		{
			i = 1;
		}
		if(m[2, 2] > m[i, i])
		{
			i = 2;
		}

		int j = ( i + 1 ) % 3;
		int k = ( j + 1 ) % 3;

		T s = std::sqrt(m[i, i] - m[j, j] - m[k, k] + T(1)) * T(2); // s = 4 * q[i]

		std::array<T, 3> comp{};
		comp[i] = s * T(0.25);
		comp[j] = ( m[j, i] + m[i, j] ) / s;
		comp[k] = ( m[k, i] + m[i, k] ) / s;
		T w = ( m[k, j] - m[j, k] ) / s;
		return { comp[0], comp[1], comp[2], w };
	}

	// 四元数を使ってベクトルを回転させる関数
	[[nodiscard]] constexpr Vector4<T> Rotate(const Quaternion<T>& q, const Vector4<T>& v) const noexcept
	{
		Vector4<T> qv{ q.x, q.y, q.z, 0 };
		Vector4<T> t = qv.Cross(v) * T(2);
		return v + t * q.w + qv.Cross(t);
	}

	// 軸と角度から四元数を生成する関数
	NDCE Quaternion<T> FromAxisAngle(const Vector4<T>& axis, T angle) const noexcept
	{
		T halfAngle = angle * T(0.5);
		T sin = std::sin(halfAngle);
		T cos = std::cos(halfAngle);
		return { axis.x * sin, axis.y * sin, axis.z * sin, cos };
	}

	// オイラー角から四元数を生成する関数
	// オイラー角とは、3次元空間における物体の回転を表すための角度の組み合わせ
	[[nodiscard]] Quaternion<T> FromEulerAngles(T pitch, T yaw, T roll) const noexcept
	{
		Quaternion<T> qx = FromAxisAngle({ 1, 0, 0, 0 }, pitch);
		Quaternion<T> qy = FromAxisAngle({ 0, 1, 0, 0 }, yaw);
		Quaternion<T> qz = FromAxisAngle({ 0, 0, 1, 0 }, roll);
		return qz * qy * qx;
	}

	// オイラー角に変換する関数
	[[nodiscard]] Vector4<T> ToEulerAngles(const Quaternion<T>& q) const noexcept
	{
		Matrix4<T> m = ToMatrix4(q);
		
		T pitch{};
		T yaw{};
		T roll{};

		T sinPitch = std::clamp(-m[2, 1], T(-1), T(1));

		// 100倍のイプシロンを使って、sinPitchが1に近いかどうかを判定する
		// イプシロンとは、浮動小数点数の計算における誤差の範囲を表す値
		T threshold = T(1) - (std::numeric_limits<T>::epsilon() * T(100));

		if(std::abs(sinPitch) > threshold)
		{
			pitch = std::asin(sinPitch);
			yaw = std::atan2(-m[2, 0], m[0, 0]);
			roll = 0;
		}
		else
		{
			pitch = std::asin(sinPitch);
			yaw = std::atan2(m[0, 2], m[1, 1]);
			roll = std::atan2(m[1, 0], m[0, 0]);
		}
		return { pitch, yaw, roll, 0 };
	}

	// 2つのベクトルから四元数を生成する関数
	[[nodiscard]] Quaternion<T> LookRotation(const Vector4<T>& forward, const Vector4<T>& up) const noexcept
	{
		Vector4<T> f = forward.Normalized();
		Vector4<T> r = up.Cross(f).Normalized();	// 右方向ベクトル
		Vector4<T> u = f.Cross(r);					// 上方向ベクトル

		Matrix4<T> m
		{
			{
				{ r.x, u.y, f.z, 0 }, // 右方向ベクトル
				{ u.x, u.y, u.z, 0 }, // 上方向ベクトル
				{ f.x, f.y, f.z, 0 }, // 前方向ベクトル
				{ 0,   0,   0,   1 }
			}
		};

		return FromMatrix(m);
	}

	NDCE Quaternion<T> Lerp(const Quaternion<T>& q1, const Quaternion<T>& q2, T t) const noexcept
	{
		return ( q1 * ( T(1) - t ) + q2 * t ).Normalized();
	}

	[[nodiscard]] Quaternion<T> Slerp(const Quaternion<T>& q1, const Quaternion<T>& q2, T t) const noexcept
	{
		T dot = Dot(q1, q2);

		if(dot < 0)
		{
			q1 = q1 * T(-1);
			dot = -dot;
		}

		// 4000倍のイプシロンを使って、dotが1に近いかどうかを判定する
		// float の場合、: 1.0 - (epsilon * 4000) = 0.9995
		T threshold = T(1) - (std::numeric_limits<T>::epsilon() * T(4000));

		if(dot > threshold)
		{
			return Lerp(q1, q2, t);
		}

		T theta0 = std::acos(dot);
		T theta = theta0 * t;
		T sinTheta0 = std::sin(theta0);
		T sinTheta = std::sin(theta);

		T s0 = std::cos(theta) - dot * sinTheta / sinTheta0;
		T s1 = sinTheta / sinTheta0;

		return ( q1 * s0 ) + ( q2 * s1 );
	}

	[[nodiscard]] Quaternion<T> RotateTowards(const Quaternion<T>& q1, const Quaternion<T>& q2, T maxAngle) const noexcept
	{
		T angle = Angle(q1, q2);
		if(angle < std::numeric_limits<T>::epsilon())
		{
			return q2;
		}

		T t = std::min(T(1), maxAngle / angle);
		return Slerp(q1, q2, t);
	}
	
};

