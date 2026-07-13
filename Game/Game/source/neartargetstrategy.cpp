#include "pch.h"
#include "neartargetstrategy.h"

NearTargetScreenStrategy::NearTargetScreenStrategy(float maxDistance)
	: TargetStrategyBase(maxDistance)
{

}

// 画面中央に近いターゲットを選択する戦略
std::optional<float> NearTargetScreenStrategy::ComputeScore
(
	CharaBase* target,	// 対象のキャラクターリスト
	const Vec4& selfPos,					// 自身の位置(今回はplayerの位置)
	Camera* camera
) const
{
	if(!camera)
	{
		return std::nullopt;
	}

	Vec4 pos = target->GetPos();
	Vec4 viewDir = v::VNorm(v::VSub(camera->_v_target, camera->_v_pos));
	Vec4 toTarget = v::VSub(pos, camera->_v_pos);// カメラからターゲットへのベクトル

	float dot = v::Dot(viewDir, v::VNorm(toTarget));
}


NearTargetStrategy::NearTargetStrategy(float maxDistance)
	: TargetStrategyBase(maxDistance)
{

}

// 敵用
// ComputeScore関数は、ターゲットキャラクターと自身の位置を比較して距離を計算し、その距離をスコアとして返す
std::optional<float> NearTargetStrategy::ComputeScore
(
	CharaBase* target,	// 対象のキャラクターリスト
	const Vec4& selfPos,					// 自身の位置(今回はplayerの位置)
	Camera* camera
) const
{
	// もしターゲットがnullptrの場合、スコアを計算できないのでstd::nulloptを返す
	if(!target)
	{
		return s