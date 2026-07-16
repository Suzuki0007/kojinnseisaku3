#include "pch.h"
#include "targetcomponent.h"

TargetComponent::TargetComponent
(
	CharaBase& owner,
	std::unique_ptr<ITargetStrategy> strategy,
	std::unique_ptr<ITargetProvider> provider
)
	: base(owner)
	, _strategy(std::move(strategy))
	, _provider(std::move(provider))
{

}

bool TargetComponent::AcquireTarget()
{
	_target = nullptr;
	_candidate.clear();

	if(!_strategy || !_camera || !_owner || !_provider)
	{
		return false;
	}

	auto targets = _provider->GetTargets();

	if(targets.empty())
	{
		return false;
	}

	TargetContext context
	{
		.target = targets,
		.selfPos = _owner->GetPos(),
		.camera = _camera
	};

	_candidate = _strategy->CollectCandidate(context);

	if(_candidate.empty())
	{
		return false;
	}
		
	TargetInfo* bast = &_candidate.front();

	for(auto& info : _candidate)
	{
		if(info.score < bast->score)
		{
			bast = &info;
		}
	}

	_target = bast->target;

	return _target != nullptr;
}

bool TargetComponent::CycleTarget(CycleDirection dir)
{
	if(!_provider)
	{
		return false;
	}

	auto targets = _provider->GetTargets();

	if(targets.empty())
	{
		_target = nullptr;
		return false;
	}

	if(_candidate.empty() || !_target)
	{
		return AcquireTarget();
	}

	TargetInfo* currentTarget = nullptr;

	for(auto& info : _candidate)
	{
		if(info.target == _target)
		{
			currentTarget = &info;
			break;
		}
	}

	if(currentTarget == nullptr)
	{
		return AcquireTarget();
	}

	TargetInfo* bestTarget = nullptr;
	float bestDelta = 0.0f;

	for(auto& info : _candidate)
	{
		if(info.target == _target)
		{
			continue;
		}

		float delta = 0.0f;

		// ターゲットのサイドを比較して、移動方向に応じた差分を計算する
		if(dir == CycleDirection::Left)
		{
			delta = currentTarget->side - info.side;
		}
		else
		{
			delta = info.side - currentTarget->side;
		}

		if(delta <= 0.0f)
		{
			continue;
		}

		// 最も近いターゲットを選択する
		if(!bestTarget || delta < bestDelta)
		{
			bestTarget = &info;
			bestDelta = delta;
			continue;
		}

		// スコアが同じ場合は、より近いターゲットを優先する
		// std::tieを使って、deltaとinfo.scoreを比較することで、より近いターゲットを優先する
		// std::tieとは、複数の値をまとめて比較するための関数で、タプルを作成して比較することができる
		if(std::tie(delta, info.score) < std::tie(bestDelta, bestTarget->score))
		{
			bestTarget = &info;
			bestDelta = delta;
		}
	}

	if(!bestTarget)
	{
		return false;
	}

	_target = bestTarget->target;

	return true;
}

void TargetComponent::RefreshCandidate()
{
	if(!_strategy || !_camera || !_owner || !_provider)
	{
		return;
	}
	
	auto targets = _provider->GetTargets();

	TargetContext context
	{
		.target = targets,
		.selfPos = _owner->GetPos(),
		.camera = _camera
	};

	_candidate = _strategy->CollectCandidate(context);

	if(_target)
	{
		// 現在のターゲットが候補リストに含まれているかどうかを確認する
		bool stillValid = std::ranges::contains
		(
			_candidate,
			_target,
			&TargetInfo::target
		);

		// 現在のターゲットが候補リストに含まれていない場合、ターゲットをクリアする
		if(!stillValid)
		{
			_target = nullptr;
		}
	}
}

bool TargetComponent::Render()
{
	if(!_target || !_camera)
	{
		return true;
	}

	Vec4 center = _target->GetPos();

	center.y += 120.0f; // キャラクターの頭上に表示するためにY座標を調整

	float size = 35.0f; // ターゲットマーカーのサイズ

	Vec4 right = _camera->GetRight() * size;

	Vec4 up = _camera->GetUp() * size;

	unsigned int color = GetColor(255, 0, 0); // 赤色

	VC::DrawLine3D(v::VSub(center, right), v::VAdd(center, right), color);
	VC::DrawLine3D(v::VSub(center, up), v::VAdd(center, up), color);

	return true;
}