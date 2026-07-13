#include "pch.h"
#pragma once

#include "matrix4.h"
#include "Dxlib.h"

namespace MC
{
	inline MATRIX Matrix4ToDxlib(const Mat4& mat) noexcept
	{
		MATRIX result{};
		std::memcpy(&result, &mat.m, sizeof(MATRIX));
		return result;
	}

	inline Mat4 DxlibToMatrix4(const MATRIX& mat) noexcept
	{
		Mat4 result{};
		std::memcpy(&result.m, &mat, sizeof(MATRIX));
		return result;
	}

	inline Mat4 MV1GetFrameLocalMatrix(int modelHandle, int frameIndex) noexcept
	{
		return DxlibToMatrix4(::MV1GetFrameLocalMatrix(modelHandle, frameIndex));
	}

	inline Mat4 MV1GetMatrix(int modelHandle) noexcept
	{
		return DxlibToMatrix4(::MV1GetMatrix(modelHandle));
	}

	inline Mat4 MMult(const Mat4& m1, const Mat4& m2) noexcept
	{
		return DxlibToMatrix4(::MMult(Matrix4ToDxlib(m1), Matrix4ToDxlib(m2)));
	}
}