#include "pch.h"
#include "kickeffect.h"

using namespace kick;

namespace
{
	static constexpr float COLOR_MAX = 255.0f;

	// 乱数の範囲指定
	float RandomRange(float min, float max)
	{
		return min + (max - min) * (std::rand() / static_cast<float>(RAND_MAX));
	}

	// 線をアルファ付きで描画する関数
	void RenderLineWithAlpha(const Vec4& p1, const Vec4& p2, unsigned int color, float alpha)
	{
		if(alpha <= 0.0f)
		{
			return;
		}

		int a = static_cast<int>(clamp::MyClamp2(alpha) * COLOR_MAX);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, a);
		VC::DrawLine3D(p1, p2, color);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

bool KickEffect::Initialize()
{
	if(!base::Initialize())
	{
		return false;
	}

	_speedLine.clear();
	_ring.clear();
	_spark.clear();

	SpawnSpeedLine();

	return true;
}

void KickEffect::SetUp(const Vec4& startpos, const Vec4& dir, float kickdistance)
{
	_basePos = startpos;
	_dir = v::VNorm(dir);

	Vec4 up = Vec4::UnitY();

	QuateMatVec qmv;
	Quate rot = qmv.LookRotation(_dir, up);// 方向ベクトルから回転を計算

	Vec4 right = qmv.Rotate(rot, Vec4::UnitX());// 回転を適用して右方向ベクトルを計算
	Vec4 up = qmv.Rotate(rot, Vec4::UnitY());// 回転を適用して上方向ベクトルを計算

	_right = v::VGet(right.x, right.y, right.z);// 右方向ベクトルを設定
	_up = v::VGet(up.x, up.y, up.z);// 上方向ベクトルを設定

	_kickDistanceCache = kickdistance;
}

bool KickEffect::Terminate()
{
	base::Terminate();

	_speedLine.clear();
	_ring.clear();
	_spark.clear();
	_started = false;

	return true;
}

void KickEffect::SpawnSpeedLine()
{
	for(int i = 0; i < kick::SPEED_LINE_COUNT; i++)
	{
		// ランダムにすることで静止した幾何学模様のような見た目を避けるため
		float offsetAmount = RandomRange(-kick::SPEED_LINE_OFFSET_MAX, kick::SPEED_LINE_OFFSET_MAX);// 上下のオフセットをランダムに設定

		float maxLife = kick::SPEED_LINE_LIFE + RandomRange(-kick::SPEED_LINE_LIFE_VAR, kick::SPEED_LINE_LIFE_VAR);// 寿命をランダムに設定

		_speedLine.push_back(SpeedLine
			{
				.offset = v::VScale(_up, offsetAmount),// 上下のオフセットを設定
				.lenght = RandomRange(kick::SPEED_LINE_LENGTH_MIN, kick::SPEED_LINE_LENGTH_MAX),// 長さをランダムに設定
				.life = maxLife,// 寿命を設定
				.maxLife = maxLife,// 最大寿命を設定
				.color = GetColor(kick::SPEED_LINE_COLOR_R, kick::SPEED_LINE_COLOR_G, kick::SPEED_LINE_COLOR_B)// 色を設定
			});
	}
}

void KickEffect::SpawnRing()
{
	_ring.push_back(Ring
		{
			.center = _basePos,// リングの中心位置を設定
			.right = _right,// リングの右方向を設定
			.up = _up,// リングの上方向を設定
			.radius = RING_START_RADIUS,// リングの半径を初期化
			.maxRadius = _kickDistanceCache * 0.5f,// リングの最大半径を設定
			.life = kick::RING_LIFE,// リングの寿命を設定
			.maxLife = kick::RING_LIFE,// リングの最大寿命を設定
			.segment = kick::RING_SEGMENT,// リングの分割数を設定
			.color = GetColor(kick::RING_COLOR_R, kick::RING_COLOR_G, kick::RING_COLOR_B)// リングの色を設定
		});
}