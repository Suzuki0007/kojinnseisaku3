#pragma once
#include "targetstrategybase.h"

namespace dist
{
	static constexpr float MAX_DISTANCE = 3000.0f; // 最大距離
}

// 画面中央に近いターゲットを選択する戦略
class NearTargetScreenStrategy final : public TargetStrategyBase
{
public:
	explicit NearTargetScreenStrategy(float maxDistance = dist::MAX_DISTANCE);

protected:
	std::optional<TargetInfo> ComputeScore
	(
		CharaBase* target,	// 対象のキャラクターリスト
		const TargetContext& context,
		float distance
	) const override;
};


// 自身の位置に近いターゲットを選択する戦略
class NearTargetStrategy final : public TargetStrategyBase
{
public:
	explicit NearTargetStrategy(float maxDistance = dist::MAX_DISTANCE);

protected:
	std::optional<TargetInfo> ComputeScore
	(
		CharaBase* target,	// 対象のキャラクターリスト
		const TargetContext& context,
		float distance
	) const override;

};

