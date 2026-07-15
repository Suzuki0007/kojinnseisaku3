#include "pch.h"
#include "targetstrategybase.h"

std::vector<TargetInfo> TargetStrategyBase::CollectCandidate
(
	const TargetContext& context
) const
{
	std::vector<TargetInfo> result;

	result.reserve(context.target.size());

	for(auto* target : context.target)
	{
		if(!target || !target->IsAlive())
		{
			continue;
		}

		float distance = Vec4::Distance(
			target->GetPos(),
			context.selfPos
		);

		if(distance > _maxDistance)
		{
			continue;
		}

		auto score = ComputeScore(target, context, distance);

		if(!score.has_value())
		{
			continue;
		}

		result.emplace_back(std::move(*score));
	}
	return result;
}