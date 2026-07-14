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


	}
}
