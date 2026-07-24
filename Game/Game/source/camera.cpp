#include "pch.h"
#include "camera.h"

namespace
{
	constexpr float kMinDistThreshold = 0.0001f;

	constexpr float analogMin = 0.3f;// アナログスティックの最小入力値（デッドゾーン）

	constexpr float pitchSpeed = 0.03f; // 上下スティックの感度
	constexpr float pitchMax = 1.2f;  // 下方向の制限（ラジアン）

	constexpr float rotateSpeed = 0.05f; // 左右スティックの感度
	constexpr float groundY = 0.0f; // 地面のY座標
	constexpr float groundMargin = 10.0f; // 地面との距離

	constexpr int segments = 16; // 弧を何分割するか
	unsigned int colorEdge = GetColor(0, 255, 0);   // 緑: 画角の境界線
	unsigned int colorCenter = GetColor(0, 255, 0); // 黄: 中心線(forward)

	constexpr float halffov = 0.5f;// 画角の半分
	constexpr float halfFov2 =2.0f;// 画角の半分の2倍（全体の画角）

	constexpr float clipNear = 2.0f;// クリップ面の近距離
	constexpr float clipFar = 10000.0f;// クリップ面の遠距離
}

bool Camera::Initialize()
{
	_v_pos = v::VGet(0.0f, 90.0f, -300.0f);
	_v_target = v::VGet(0.0f, 60.0f, 0.0f);
	_clip_near = clipNear;
	_clip_far = clipFar;

	SetupCamera_Perspective(_fovY);

	// 初期状態の角度と距離を計算
	float sx = _v_pos.x - _v_target.x;
	float sy = _v_pos.y - _v_target.y;
	float sz = _v_pos.z - _v_target.z;
	_angle = atan2(sz, sx);
	_dist = sqrt(sz * sz + sx * sx);

	_horizontalDist = sqrt(sx * sx + sz * sz);
	_pitch = atan2(sy, _horizontalDist);

	_height_y = _v_target.y;// 注視点の高さを保持

	return true;
}

Vec4 Camera::GetForward() const
{
	Vec4 dir = v::VSub(_v_target, _v_pos);
	return v::VNorm(dir);
}

Vec4 Camera::GetToTarget(const Vec4& targetPos) const
{
	Vec4 dir = v::VSub(targetPos, _v_pos);
	return v::VNorm(dir);
}

Vec4 Camera::GetRight() const
{
	return v::VNorm(v::VCross(GetForward(), Vec4::UnitY()));
}

Vec4 Camera::GetUp() const
{
	return v::VNorm(v::VCross(GetRight(), GetForward()));
}

bool Camera::Process()
{
	InputDevice& input = InputLocator::Get();

	float rx = input.GetRightStickX();
	float ry = input.GetRightStickY();

	// 1. 右スティックによるカメラ角度の変更（カメラクラスの責務）
	if(rx < -analogMin) { _angle -= rotateSpeed; } // 左回転
	if(rx > analogMin) { _angle += rotateSpeed; } // 右回転

	if(ry > analogMin) { _pitch -= pitchSpeed; } // 下方向
	if(ry < -analogMin) { _pitch += pitchSpeed; } // 上方向

	// 地面に埋まらないように

	float minHeightAboveTarget = (groundY + groundMargin) - _height_y; // 注視点の高さを考慮して最小高さを計算
	float sinPitchMin = clamp::MyClamp(minHeightAboveTarget / _dist);
	float pitchMin = std::asin(sinPitchMin);

	// ピッチ角度の制限
	if(_pitch < pitchMin) { _pitch = pitchMin; }
	if(_pitch > pitchMax) { _pitch = pitchMax; }

	// カメラの位置を計算（注視点を基準に角度と距離から位置を算出）
	Quate yawQ = QuateMatVec::FromAxisAngle(Vec4::UnitY(), _angle);

	Vec4 baseOffset = v::VGet(_dist, 0.0f, 0.0f);// カメラの距離をX軸方向に設定

	// Yaw回転を適用してカメラの位置を計算
	Vec4 yawedOffset = QuateMatVec::Rotate(yawQ, baseOffset);// Yaw回転を適用
	Vec4 rightAxis = QuateMatVec::Rotate(yawQ, Vec4::UnitZ());// Yaw回転後の右方向ベクトルを計算

	// ピッチ回転を適用
	Quate pitchQ = QuateMatVec::FromAxisAngle(rightAxis, _pitch);// ピッチ回転を適用するための四元数を作成
	Vec4 finalOffset = QuateMatVec::Rotate(pitchQ, yawedOffset);// ピッチ回転を適用して最終的なカメラの位置を計算

	// 座標の反映（注視点を基準に角度と距離から位置を算出）
	_v_target.y = _height_y;
	_v_pos = v::VAdd(_v_target, finalOffset);

	return true;
}

void Camera::DrawDebugFov(float length) const
{
	Vec4 forward = GetForward();
	float halfFov = _fovY * halffov;

	// forwardをY軸中心に±halfFovだけ回転させて、画角の左右境界ベクトルを作る
	auto rotateY = [](const Vec4& v, float rad)
		{
			float c = std::cos(rad);
			float s = std::sin(rad);
			return v::VGet
			(
				v.x * c + v.z * s,
				v.y,
				-v.x * s + v.z * c
			);
		};

	Vec4 leftDir = rotateY(forward, -halfFov);
	Vec4 rightDir = rotateY(forward, halfFov);

	Vec4 leftEnd = v::VAdd(_v_pos, v::VScale(leftDir, length));
	Vec4 rightEnd = v::VAdd(_v_pos, v::VScale(rightDir, length));
	Vec4 centerEnd = v::VAdd(_v_pos, v::VScale(forward, length));

	VC::DrawLine3D(_v_pos, leftEnd, colorEdge);
	VC::DrawLine3D(_v_pos, rightEnd, colorEdge);
	VC::DrawLine3D(_v_pos, centerEnd, colorCenter);

	Vec4 prevPoint = leftEnd;
	for(int i = 1; i <= segments; ++i)
	{
		float t = static_cast<float>(i) / static_cast<float>(segments);
		float rad = -halfFov + (halfFov * halfFov2) * t; // -halfFov 〜 +halfFov まで均等に分割

		Vec4 dir = rotateY(forward, rad);
		Vec4 point = v::VAdd(_v_pos, v::VScale(dir, length));

		VC::DrawLine3D(prevPoint, point, colorEdge);
		prevPoint = point;
	}
}

bool Camera::Render()
{
	/*int x = 0, y = 0, size = 16;
	SetFontSize(size);
	DrawFormatString(x, y, GetColor(255, 0, 0), "Camera:"); y += size;
	DrawFormatString(x, y, GetColor(255, 0, 0), "  target = (%5.2f, %5.2f, %5.2f)", _v_target.x, _v_target.y, _v_target.z); y += size;
	DrawFormatString(x, y, GetColor(255, 0, 0), "  pos    = (%5.2f, %5.2f, %5.2f)", _v_pos.x, _v_pos.y, _v_pos.z); y += size;
	float sx = _v_pos.x - _v_target.x;
	float sz = _v_pos.z - _v_target.z;
	float length = sqrt(sz * sz + sx * sx);
	float rad = atan2(sz, sx);
	float deg = RAD2DEG(rad);
	DrawFormatString(x, y, GetColor(255, 0, 0), "  len = %5.2f, rad = %5.2f, deg = %5.2f", length, rad, deg); y += size;*/
	return true;
}
