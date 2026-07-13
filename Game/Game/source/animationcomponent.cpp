#include "pch.h"
#include "animationcomponent.h"

AnimationComponent::AnimationComponent(CharaBase& owner)
	: base(owner)
{
}

bool AnimationComponent::Initialize()
{
	return true;
}

bool AnimationComponent::Terminate()
{
	// has_value()で値があるかどうかを確認する
	if(_animId.has_value())
	{
		AnimationManager::GetInstance()->Stop(*_animId);
		_animId.reset();
	}
	return true;
}

void AnimationComponent::ChangeAnimation(Anim anim)
{
	if(_animId.has_value())
	{
		AnimationManager::GetInstance()->Stop(*_animId);
		_animId.reset();
	}

	auto& clip = _animation[std::to_underlying(anim)];

	if(clip.GetName().empty())
	{
		return;
	}

	int id = AnimationManager::GetInstance()->Play(
		_owner->GetModelHandle(),
		clip.GetName(),
		clip.IsLoop(),
		clip.GetSpeed()
	);

	if(id != -1)
	{
		_animId = id;
	}
}

bool AnimationComponent::IsAnimationEnd() const
{
	if(!_animId.has_value())
	{
		return true;
	}
	return !AnimationManager::GetInstance()->IsPlaying(*_animId);
}

float AnimationComponent::GetAnimPlayTime() const
{
	if(!_animId.has_value())
	{
		return 0.0f;
	}
	return AnimationManager::GetInstance()->GetPlayTime(*_animId);
}

float AnimationComponent::GetAnimTotalTime() const
{
	if(!_animId.has_value())
	{
		return 0.0f;
	}
	return AnimationManager::GetInstance()->GetTotalTime(*_animId);
}

void AnimationComponent::SetAnimation(
	const std::array<
	AnimationClip,
	std::to_underlying(Anim::COUNT)>& animation
)
{
	_animation = animation;
}

void AnimationComponent::Update(float deltaTime)
{
	auto status = _owner->GetStatus();

	if(status != _currentAnim)
	{
		_currentAnim = status;
		ChangeAnimation(_currentAnim);
	}
}

bool AnimationComponent::Render()
{
	return true;
}

