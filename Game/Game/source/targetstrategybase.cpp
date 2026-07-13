#include "pch.h"
#include "targetstrategybase.h"

std::optional<CharaBase*> TargetStrategyBase::SelectTarget
(
	const std::vector<CharaBase*>& target,	// 対象のキャラクターリスト
	const Vec4& selfPos,					// 自身の位置(今回はplayerの位置)
	Camera* camera
) const
{
	CharaBase* bestTarget = nullptr;
	std::optional<float> bestScore = std::nullopt;

	for(auto* chara : target)
	{
		if(!chara || !chara->IsAlive())
		{
			continue;
		}

		float distance = v::VSize(v::VSub(chara->GetPos(), selfPos));

		// 距離が最大距離を超えている場合はスキップ
		if(distance > _maxDistance)
		{
			continue;
		}

		std::optional<float> score = ComputeScore(target, selfPos, camera);

		// スコアが計算できない場合はスキップ
		if(!score.has_value())
		{
			continue;
		}

		// 最も高いスコアのキャラクターを選択
		if(!bestScore.has_value() || *score < *bestScore)
		{
			bestScore = score;
			bestTarget = chara;
		}
	}

	if(bestTarget)
	{
		return bestTarget;
	}

	return std::nullopt;
}