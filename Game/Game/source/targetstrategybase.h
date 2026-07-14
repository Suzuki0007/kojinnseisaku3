#pragma once
#include "targetstrategy.h"

class TargetStrategyBase : public ITargetStrategy
{
public:
	explicit TargetStrategyBase(float maxDistance) : _maxDistance(maxDistance) {}

	// ターゲットの切り替えやロック中の対象が有効かどうか
	std::vector<TargetInfo> CollectCandidate
	(
		const TargetContext& context
	) const override final;

protected:
	
	// 対象のキャラクターリストからスコアを計算し、最も高いスコアのキャラクターを選択する
	// スコアとは、キャラクターの位置や距離、カメラの視点などを考慮して計算される値であり、最も適切なターゲットを選択する
	virtual std::optional<TargetInfo> ComputeScore
	(
		CharaBase* target,	// 対象のキャラクターリスト
		const TargetContext& context,
		float distance
	) const = 0;

	float _maxDistance;
};

