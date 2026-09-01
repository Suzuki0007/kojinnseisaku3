#pragma once
#include "effectbase.h"

namespace kick
{
	// 線
	static constexpr int SPEED_LINE_COUNT = 20;// 線の本数
	static constexpr float SPEED_LINE_LIFE = 1.5f; // 線の寿命
	static constexpr float SPEED_LINE_LIFE_VAR = 0.05f; // 線の寿命のばらつき
	static constexpr float SPEED_LINE_OFFSET_MAX = 8.0f; // 上下のオフセットの最大値
	static constexpr float SPEED_LINE_LENGTH_MIN = 0.0f; // 線の長さの最小値
	static constexpr float SPEED_LINE_LENGTH_MAX = 2000.0f; // 線の長さの最大値
	static constexpr float SPEED_LINE_GROWTH_MIN = 0.8f;// 線の成長率の最小値
	static constexpr float SPEED_LINE_GROWTH_MAX = 1.2f;// 線の成長率の最大値
	static unsigned int SPEED_LINE_COLOR_R = 150;
	static unsigned int SPEED_LINE_COLOR_G = 230;
	static unsigned int SPEED_LINE_COLOR_B = 255;

	// リング
	static constexpr float RING_DELAY = 0.1f; // 開始からリングが出るまでの時間(秒)
	static constexpr float RING_LIFE = 0.3f; // リングの寿命(秒)
	static constexpr float RING_START_RADIUS = 5.0f; // 出現時の半径
	static constexpr float RING_MAX_RADIUS = 45.0f; // 最大半径
	static constexpr int RING_SEGMENT = 24; // リングの分割数
	static constexpr unsigned int RING_COLOR_R = 140;
	static constexpr unsigned int RING_COLOR_G = 100;
	static constexpr unsigned int RING_COLOR_B = 255;

	// スパーク
	static constexpr float SPARK_DELAY = 0.28f; // 開始からスパークが出るまでの時間(秒)
	static constexpr float SPARK_LIFE = 0.2f; // スパークの寿命(秒)
	static constexpr float SPARK_LIFE_VAR_MINUS = 0.03f; // スパークの寿命のばらつきの最小値(秒)
	static constexpr float SPARK_LIFE_VAR_PLUS = 0.05f; // スパークの寿命のばらつきの最大値(秒)
	static constexpr int SPARK_COUNT = 8; // スパークの数
	static constexpr float SPARK_LENGTH_MIN = 4.0f;// スパークの長さの最小値
	static constexpr float SPARK_LENGTH_MAX = 8.0f;// スパークの長さの最大値
	static constexpr float SPARK_SPEED_MIN = 120.0f; // スパークの速度の最小値
	static constexpr float SPARK_SPEED_MAX = 220.0f; // スパークの速度の最大値
	static constexpr float SPARK_SPREAD_MIN = 0.2f; // スパークの広がりの最小値
	static constexpr float SPARK_SPREAD_MAX = 1.0f; // スパークの広がりの最大値
	static constexpr unsigned int SPARK_COLOR_R = 255;
	static constexpr unsigned int SPARK_COLOR_G = 255;
	static constexpr unsigned int SPARK_COLOR_B = 220;

	static constexpr float FADE_OUT_TIME = 0.5f; // フェードアウトの時間
}

class KickEffect final : public EffectBase
{
	using base = EffectBase;

public:
	KickEffect() = default;
	virtual ~KickEffect() = default;
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	void SetUp(const Vec4& startpos, const Vec4& dir, float kickdistance) override;
	void SetEffectPos(const Vec4& pos) override;

	void EndAttack();
	bool IsFinished() const;

private:

	// 線の情報
	struct SpeedLine
	{
		Vec4 offset;// 線のオフセット
		float lenght;// 線の長さ
		float growthRatio;// 線の成長率s
		float life;// 線の寿命
		float maxLife;// 線の最大寿命
		unsigned int color;// 線の色
	};

	// リングの情報
	struct Ring
	{
		Vec4 center;// リングの中心位置
		Vec4 right;// リングの右方向
		Vec4 up;// リングの上方向
		float radius;// リングの半径
		float maxRadius;// リングの最大半径
		float life;// リングの寿命
		float maxLife;// リングの最大寿命
		int segment;// リングの分割数
		unsigned int color;// リングの色
	};

	// スパークの情報
	struct Spark
	{
		Vec4 pos;// スパークの位置
		Vec4 dir;// スパークの方向
		float length;// スパークの長さ
		float speed;// スパークの速度
		float life;// スパークの寿命
		float maxLife;// スパークの最大寿命
		unsigned int color;// スパークの色
	};

	void SpawnSpeedLine();// 線の生成
	void SpawnRing();// リングの生成
	void SpawnSpark();// スパークの生成

	void UpdateSpeedLines(float deltaTime);// 線の更新
	void UpdateRings(float deltaTime);// リングの更新
	void UpdateSparks(float deltaTime);// スパークの更新

	void RenderSpeedLine();// 線の描画
	void RenderRing();// リングの描画
	void RenderSpark();// スパークの描画

	static float CalcAlpha(float life, float maxLife);// 透明度の計算
	float GetFadeAlpha() const;// フェードアウトの透明度の計算

	virtual const char* GetCharaClassName() const override { return ""; }
	virtual float GetSpeed() const override { return 0.0f; }
	void CommandAttack(IBattleReceiver* target) override
	{
		
	}

	bool IsExceutionAction() const override
	{
		return false;
	}

protected:
	std::vector<SpeedLine> _speedLine;// 線の情報を格納
	std::vector<Ring> _ring;// リングの情報を格納
	std::vector<Spark> _spark;// スパークの情報を格納

	Vec4 _basePos {0.0f, 0.0f, 0.0f};// エフェクトの基準位置
	Vec4 _right {1.0f, 0.0f, 0.0f};
	Vec4 _up{ 0.0f, 1.0f, 0.0f };
	float _kickDistanceCache{ 0.0f };// キックの距離のキャッシュ
	float _travelDistance{ 0.0f };// エフェクトの移動距離
	float _fadeElapsedTime{ 0.0f };// フェードアウトの経過時間

	float _elapsedTime{ 0.0f };// 経過時間
	bool _ringSpawned{ false };// リングが生成されたかどうか
	bool _sparkSpawned{ false };// スパークが生成されたかどうか
	bool _started{ false };// エフェクトが開始されたかどうか
	bool _ending{ false };// エフェクトが終了したかどうか

};

