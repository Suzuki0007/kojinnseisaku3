#pragma once
#include "appframe.h"
#include "applicationmain.h"


class Camera
{
public:
	virtual bool Initialize();
	virtual bool Process();
	virtual bool Render();

    // 外部からターゲット位置を設定できるように追加
    void SetTargetPosition(const Vec4& t) { _v_target = t; _height_y = t.y; }

	// 外部から角度を取得・設定できるように追加
	float GetAngle() const { return _angle; }
	void SetAngle(float angle) { _angle = angle; }
	float GetFovY() const { return _fovY; }
	void SetFovY(float fovY) { _fovY = fovY; }

	Vec4 GetForward() const;
	Vec4 GetToTarget(const Vec4& targetPos) const;
	Vec4 GetRight() const;
	Vec4 GetUp() const;

	Vec4 _v_pos;
	Vec4 _v_target;
	float _clip_near;
	float _clip_far;


protected:
	float _fovY { DEG2RAD(60.0f) };   // カメラの垂直視野角
	float _angle;    // カメラの水平角度（ラジアン）
	float _dist;     // ターゲットとの距離
	float _height_y; // ターゲットの注視点の高さ
    float _height_offset; // 注視点とカメラ高さの差
};

