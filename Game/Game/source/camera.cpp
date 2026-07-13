#include "pch.h"
#include "camera.h"


bool Camera::Initialize()
{
	_v_pos = v::VGet(0.0f, 90.0f, -300.0f);
	_v_target = v::VGet(0.0f, 60.0f, 0.0f);
	_clip_near = 2.0f;
	_clip_far = 10000.0f;

	// 初期状態の角度と距離を計算
	float sx = _v_pos.x - _v_target.x;
	float sz = _v_pos.z - _v_target.z;
	_angle = atan2(sz, sx);
	_dist = sqrt(sz * sz + sx * sx);
	_height_y = _v_target.y;
	_height_offset = _v_pos.y - _v_target.y; // カメラ高さと注視点の差を保持

	return true;
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
