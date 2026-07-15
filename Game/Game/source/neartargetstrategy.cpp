#include "pch.h"
#include "neartargetstrategy.h"

NearTargetScreenStrategy::NearTargetScreenStrategy(float maxDistance)
	: TargetStrategyBase(maxDistance)
{

}

// 画面中央に近いターゲットを選択する戦略
std::optional<TargetInfo> NearTargetScreenStrategy::ComputeScore
(
	CharaBase* target,	// 対象のキャラクターリスト
	const TargetContext& context,
	float distance
) const
{
	if(!context.camera)
	{
		return std::nullopt;
	}

	// カメラ前方向
	Vec4 forward = context.camera->GetForward();

	// カメラからターゲットへの方向ベクトル
	Vec4 toTarget = context.camera->GetToTarget(target->GetPos());


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
		return std::nullopt;
	}
	float distance = v::VSize(v::VSub(target->GetPos(), selfPos));
	return distance;
}