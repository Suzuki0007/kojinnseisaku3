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

	// 正規化
	// std::clampを使って、内積の値を-1.0fから1.0fの範囲に制限する
	float dot = std::clamp(forward.Dot(toTarget), -1.0f, 1.0f);

	float angle = std::acos(dot); // ラジアンで角度を計算

	float halfFov = context.camera->GetFovY() * 0.5f; // カメラの半分の視野角を取得

	if(angle > halfFov)
	{
		return std::nullopt; // ターゲットが視野外の場合はスコアを計算しない
	}

	Vec4 right = forward.Cross(Vec4::UnitY()).Normalized();// カメラの右方向ベクトル

	float side = right.Dot(toTarget); // 右方向に対する位置関係を計算

	return TargetInfo
	{
		.target = target,
		.score = distance, // 近いほどスコアが高くなるように距離をスコアとして使用
		.angle = angle,
		.distance = distance,
		.side = side
	};
}


NearTargetStrategy::NearTargetStrategy(float maxDistance)
	: TargetStrategyBase(maxDistance)
{

}

// 敵用
// ComputeScore関数は、ターゲットキャラクターと自身の位置を比較して距離を計算し、その距離をスコアとして返す
std::optional<TargetInfo> NearTargetStrategy::ComputeScore
(
	CharaBase* target,	// 対象のキャラクターリスト
	const TargetContext& context,
	float distance
) const
{
	// もしターゲットがnullptrの場合、スコアを計算できないのでstd::nulloptを返す
	if(!target)
	{
		return std::nullopt;
	}
	return TargetInfo
	{
		.target = target,
		.score = distance,
		.angle = 0.0f,
		.distance = distance,
		.side = 0.0f
	};
}