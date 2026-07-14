#pragma once
#include "vector4operator.h"
#include "Dxlib.h"

namespace VC
{
	// Vec4からDxLibのVECTOR型の変換関数
	inline VECTOR VecToDxLib(Vec4 v1)
	{
		return VGet(v1.x, v1.y, v1.z);
	}

	// DxLibのVECTOR型からVec4への変換関数
	inline Vec4 DxLibToVec(VECTOR v1)
	{
		return Vec4(v1.x, v1.y, v1.z, 0.0f);
	}

	inline int MV1SetPosition(int handle, Vec4 pos)
	{
		return MV1SetPosition(handle, VecToDxLib(pos));
	}

	inline int MV1SetRotationXYZ(int handle, Vec4 rot)
	{
		return MV1SetRotationXYZ(handle, VecToDxLib(rot));
	}

	inline MV1_COLL_RESULT_POLY MV1CollCheckLine(int handle, int frame, Vec4 v1, Vec4 v2)
	{
		return MV1CollCheck_Line(handle, frame, VecToDxLib(v1), VecToDxLib(v2));
	}

	inline int HitCheckCapsuleToCapsule(Vec4 t1, Vec4 b1, float r1, Vec4 t2, Vec4 b2, float r2)
	{
		return HitCheck_Capsule_Capsule(VecToDxLib(t1), VecToDxLib(b1), r1, VecToDxLib(t2), VecToDxLib(b2), r2);
	}

	inline HITRESULT_LINE HitCheckLineToTriangle(Vec4 linepos1, Vec4 linepos2, Vec4 triV1, Vec4 triV2, Vec4 triV3)
	{
		return HitCheck_Line_Triangle(VecToDxLib(linepos1), VecToDxLib(linepos2), VecToDxLib(triV1), VecToDxLib(triV2), VecToDxLib(triV3));
	}

	inline int DrawLine3D(Vec4 v1, Vec4 v2, unsigned int color)
	{
		return ::DrawLine3D(VecToDxLib(v1), VecToDxLib(v2), color);
	}

	inline int SetShadowMapDrawArea(int handle, Vec4 min, Vec4 max)
	{
		return ::SetShadowMapDrawArea(handle, VecToDxLib(min), VecToDxLib(max));
	}

	inline MV1_COLL_RESULT_POLY_DIM HitCapsuleToCollision(int modelHandle, int frameIndex, Vec4 capTop, Vec4 capBottom, float radius)
	{
		return MV1CollCheck_Capsule(modelHandle, frameIndex, VecToDxLib(capTop), VecToDxLib(capBottom), radius);
	}

	inline int DrawTriangle3D(Vec4 v1, Vec4 v2, Vec4 v3, unsigned int color, int fill)
	{
		return ::DrawTriangle3D(VecToDxLib(v1), VecToDxLib(v2), VecToDxLib(v3), color, fill);
	}

	inline int DrawCapsule3D(Vec4 top, Vec4 bottom, float radius, int division, unsigned int edgeColor, unsigned int capColor, int fillFlag)
	{
		return ::DrawCapsule3D(VecToDxLib(top), VecToDxLib(bottom), radius, division, edgeColor, capColor, fillFlag);
	}

	inline Vec4 ConvWorldPosToScreenPos(const Vec4& worldPos)
	{
		return DxLibToVec(ConvWorldPosToScreenPos(VecToDxLib(worldPos)));
	}
}