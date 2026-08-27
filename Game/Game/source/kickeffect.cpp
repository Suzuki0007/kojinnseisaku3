#include "pch.h"
#include "kickeffect.h"

using namespace kick;

namespace
{
	static constexpr float COLOR_MAX = 255.0f;
	static constexpr float HALF_LENGTH = 0.5f;

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
	_started = true;

	

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

	Vec4 rightVec = qmv.Rotate(rot, Vec4::UnitX());// 回転を適用して右方向ベクトルを計算
	Vec4 upVec = qmv.Rotate(rot, Vec4::UnitY());// 回転を適用して上方向ベクトルを計算

	_right = v::VGet(rightVec.x, rightVec.y, rightVec.z);// 右方向ベクトルを設定
	_up = v::VGet(upVec.x, upVec.y, upVec.z);// 上方向ベクトルを設定

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

void KickEffect::SpawnSpark()
{
	Vec4 tip = v::VAdd(_basePos, v::VScale(_dir, _kickDistanceCache));// スパークの発生位置を計算

	for(int i = 0; i < kick::SPARK_COUNT; i++)
	{
		float angle = RandomRange(0.0f, TWO_PI);
		float spread = RandomRange(kick::SPARK_SPREAD_MIN, kick::SPARK_SPREAD_MAX);// スパークの広がりをランダムに設定

		Vec4 radial = v::VAdd(v::VScale(_right, std::cos(angle)), v::VScale(_up, std::sin(angle)));// ランダル方向を計算
		Vec4 sparkDir = v::VNorm(v::VAdd(v::VScale(_dir, 1.0f - spread), v::VScale(radial, spread)));// スパークの方向を計算
		float maxLife = kick::SPARK_LIFE + RandomRange(-kick::SPARK_LIFE_VAR_MINUS, kick::SPARK_LIFE_VAR_PLUS);// スパークの寿命をランダムに設定

		_spark.push_back(Spark
			{
				.pos = tip,// スパークの位置を設定
				.dir = sparkDir,// スパークの方向を設定
				.length = RandomRange(kick::SPARK_LENGTH_MIN, kick::SPARK_LENGTH_MAX),// スパークの長さをランダムに設定
				.speed = RandomRange(kick::SPARK_SPEED_MIN, kick::SPARK_SPEED_MAX),// スパークの速度をランダムに設定
				.life = maxLife,// スパークの寿命を設定
				.maxLife = maxLife,// スパークの最大寿命を設定
				.color = GetColor(kick::SPARK_COLOR_R, kick::SPARK_COLOR_G, kick::SPARK_COLOR_B)// スパークの色を設定
			});
	}
}

bool KickEffect::Process()
{
	if(!base::Process())
	{
		return false;
	}
	if(!_started)
	{
		return true;
	}

	float dt = TimeManager::GetInstance()->GetDeltaTime();

	_elapsedTime += dt;// 経過時間を更新

	// 線を1回だけ生成する
	if(!_ringSpawned && _elapsedTime >= kick::RING_DELAY)
	{
		SpawnRing();
		_ringSpawned = true;
	}

	// リングを1回だけ生成する
	if(!_sparkSpawned && _elapsedTime >= kick::SPARK_DELAY)
	{
		SpawnSpark();
		_sparkSpawned = true;
	}

	UpdateSpeedLines(dt);// 線の更新
	UpdateRings(dt);// リングの更新
	UpdateSparks(dt);// スパークの更新


	return true;
}

void KickEffect::UpdateSpeedLines(float deltaTime)
{
	// 線の寿命を減らす
	for(auto& line : _speedLine)
	{
		line.life -= deltaTime;
	}

	std::erase_if(_speedLine, [](const SpeedLine& line) { return line.life <= 0.0f; });// 寿命が0以下の線を削除
}

void  KickEffect::UpdateRings(float deltaTime)
{
	// リングの寿命を減らす
	for(auto& ring : _ring)
	{
		ring.life -= deltaTime;
		float elapsed = ring.maxLife - ring.life;// 経過時間を計算
		ring.radius = easing::EasingOutSine(elapsed, 0.0f, ring.maxRadius, ring.maxLife);// リングの半径をイージング関数で更新
	}
	std::erase_if(_ring, [](const Ring& ring) { return ring.life <= 0.0f; });// 寿命が0以下のリングを削除
}

void KickEffect::UpdateSparks(float deltaTime)
{
	// スパークの寿命を減らす
	for(auto& spark : _spark)
	{
		spark.life -= deltaTime;
		spark.pos = v::VAdd(spark.pos, v::VScale(spark.dir, spark.speed * deltaTime));// スパークの位置を更新
	}
	std::erase_if(_spark, [](const Spark& spark) { return spark.life <= 0.0f; });// 寿命が0以下のスパークを削除
}

bool KickEffect::IsFinished() const
{
	return _started && _speedLine.empty() && _ring.empty() && _spark.empty();
}

float KickEffect::CalcAlpha(float life, float maxLife)
{
	return life / maxLife;// 寿命に応じて透明度を計算
}

bool KickEffect::Render()
{
	if(!base::Render())
	{
		return false;
	}
	if(!_started)
	{
		return true;
	}
	RenderSpeedLine();// 線の描画
	RenderRing();// リングの描画
	RenderSpark();// スパークの描画
	return true;
}

void KickEffect::RenderSpeedLine()
{
	for(const auto& line : _speedLine)
	{
		float alpha = CalcAlpha(line.life, line.maxLife);// 線の透明度を計算
		Vec4 center = v::VAdd(_basePos, line.offset);// 線の中心位置を計算
		Vec4 p1 = v::VAdd(center, v::VScale(_dir, line.lenght * HALF_LENGTH));// 線の始点を計算
		Vec4 p2 = v::VAdd(center, v::VScale(_dir, -line.lenght * HALF_LENGTH));// 線の終点を計算
		RenderLineWithAlpha(p1, p2, line.color, alpha);// 線を描画
	}
}

void KickEffect::RenderRing()
{
	for(const auto& ring : _ring)
	{
		float alpha = CalcAlpha(ring.life, ring.maxLife);// リングの透明度を計算

		for(int i = 0; i < ring.segment; i++)
		{
			float angle1 = (static_cast<float>(i) / ring.segment) * TWO_PI;
			float angle2 = (static_cast<float>(i + 1) / ring.segment) * TWO_PI;
			Vec4 p1 = v::VAdd(ring.center, v::VAdd(v::VScale(ring.right, std::cos(angle1) * ring.radius), v::VScale(ring.up, std::sin(angle1) * ring.radius)));
			Vec4 p2 = v::VAdd(ring.center, v::VAdd(v::VScale(ring.right, std::cos(angle2) * ring.radius), v::VScale(ring.up, std::sin(angle2) * ring.radius)));
			RenderLineWithAlpha(p1, p2, ring.color, alpha);// リングの線を描画
		}
	}
}

void KickEffect::RenderSpark()
{
	for(const auto& spark : _spark)
	{
		float alpha = CalcAlpha(spark.life, spark.maxLife);// スパークの透明度を計算
		Vec4 tail = v::VAdd(spark.pos, v::VScale(spark.dir, -spark.length));// スパークの尾の位置を計算
		RenderLineWithAlpha(tail, spark.pos, spark.color, alpha);// スパークを描画
	}
}