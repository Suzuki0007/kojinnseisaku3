#pragma once
#include "targetstrategy.h"


class TargetStrategyBase : public ITargetStrategy
{
public:
	explicit TargetStrategyBase(float maxDistance) : _maxDistance(maxDistance) {}

	std::optional<CharaBase*> SelectTarget
	(
		const std::vector<CharaBase*>& target,	// 対象のキャラクターリスト
		const Vec4& selfPos,					// 自身の位置(今回はplayerの位置)
		Camera* camera
	) const override final;

protected:
	
	// 対象のキャラクターリストからスコアを計算し、最も高いスコアのキャラクターを選択する
	// スコアとは、キャラクターの位置や距離、カメラの視点などを考慮して計算される値であり、最も適切なターゲットを選択する
	virtual std::optional<float> ComputeScore
	(
		CharaBase* target,	// 対象のキャラクターリスト
		const Vec4& selfPos,					// 自身の位置(今回はplayerの位置)
		Camera* camera
	) const = 0;

	float _maxDistance;
};

