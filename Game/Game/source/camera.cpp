#include "pch.h"
#include "camera.h"


bool Camera::Initialize()
{
	_v_pos = v::VGet(0.0f, 90.0f, -300.0f);
	_v_target = v::VGet(0.0f, 60.0f, 0.0f);
	_clip_near = 2.0f;
	_clip_far = 10000.0f;

	SetupCamera_Perspective(_fovY);

	// 初期状態の角度と距離を計算
	float sx = _v_pos.x - _v_target.x;
	float sz = _v_pos.z - _v_target.z;
	_angle = atan2(sz, sx);
	_dist = sqrt(sz * sz + sx * sx);
	_height_y = _v_target.y;
	_height_offset = _v_pos.y - _v_target.y; // カメラ高さと注視点の差を保持

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
    // 右スティック入力の取得
    DINPUT_JOYSTATE di;
    GetJoypadDirectInputState(DX_INPUT_PAD1, &di);
    // 右スティックは Rx, Ry に入るのでそれを使う（Z/Rz はトリガ等になることがある）
    float rx = (float)di.Rx / 1000.f;  // 水平回転
    float ry = (float)di.Ry / 1000.f; // 高さ変更
	float analogMin = 0.3f;

	// ズーム設定
	const float zoomSpeed = 8.0f; // 1フレームあたりの距離変化量
	const float distMin = 50.0f;  // 最小距離（プレイヤーに近づきすぎないようにする）
	const float distMax = 2000.0f; // 最大距離

	// 1. 右スティックによるカメラ角度の変更（カメラクラスの責務）
	if(rx < -analogMin) { _angle -= 0.05f; } // 左回転
	if(rx > analogMin) { _angle += 0.05f; } // 右回転

    // 2a. 右スティック縦でズーム（プレイヤーに近づいたり遠ざかったり）
    if(ry > analogMin) { // スティックを下に入れるとカメラが遠ざかる
        _dist += zoomSpeed;
    }
    if(ry < -analogMin) { // スティックを上に入れるとカメラが近づく
        _dist -= zoomSpeed;
    }
    // 距離をクランプ
    if(_dist < distMin) _dist = distMin;
    if(_dist > distMax) _dist = distMax;

	// 座標の反映（注視点を基準に角度と距離から位置を算出）
	_v_target.y = _height_y;
	_v_pos.x = _v_target.x + cos(_angle) * _dist;
	_v_pos.z = _v_target.z + sin(_angle) * _dist;
	// カメラの高さは注視点の高さにオフセットを足す
	_v_pos.y = _v_target.y + _height_offset;

	return true;
}

void Camera::DrawDebugFov(float length) const
{
	Vec4 forward = GetForward();
	float halfFov = _fovY * 0.5f;

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

	constexpr int segments = 16; // 弧を何分割するか
	unsigned int colorEdge = GetColor(0, 255, 0);   // 緑: 画角の境界線
	unsigned int colorCenter = GetColor(0, 255, 0); // 黄: 中心線(forward)

	VC::DrawLine3D(_v_pos, leftEnd, colorEdge);
	VC::DrawLine3D(_v_pos, rightEnd, colorEdge);
	VC::DrawLine3D(_v_pos, centerEnd, colorCenter);

	Vec4 prevPoint = leftEnd;
	for(int i = 1; i <= segments; ++i)
	{
		float t = static_cast<float>(i) / static_cast<float>(segments);
		float rad = -halfFov + (halfFov * 2.0f) * t; // -halfFov 〜 +halfFov まで均等に分割

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
