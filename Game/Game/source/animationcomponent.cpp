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

void AnimationComponent::ChangeAnimation(Anim anim, bool forceRestart)
{
	// すでに同じアニメーションが再生中で、強制的に再生しない場合は何もしない
	if(!forceRestart && _currentAnim == anim && _animId.has_value())
	{
		return;
	}
	
	// すでにアニメーションが再生中の場合は停止する
	if(_animId.has_value())
	{
		AnimationManager::GetInstance()->Stop(*_animId);
		_animId.reset();
	}

	_currentAnim = anim;

	auto& clip = _animation[std::to_underlying(anim)]; // アニメーションを取得

	// アニメーション名が空の場合は何もしない
	if(clip.GetName().empty())
	{
		return;
	}

	int id = AnimationManager::GetInstance()->Play(
		_owner->GetHandle(),
		clip.GetName(),
		clip.IsLoop(),
		clip.GetSpeed(),
		clip.GetSpeedVariance()
	);

	if(id != -1)
	{
		_animId = id;

		// 再生開始オフセットが設定されている場合は、ランダムにオフセットを設定する
		if(clip.GetStartOffsetMax() > 0)
		{
			float offset = static_cast<float>(rand() % clip.GetStartOffsetMax());
			AnimationManager::GetInstance()->SetTime(id, offset);
		}
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
		//_currentAnim = status;
		ChangeAnimation(status, false);
	}
}

bool AnimationComponent::Render()
{
	return true;
}

