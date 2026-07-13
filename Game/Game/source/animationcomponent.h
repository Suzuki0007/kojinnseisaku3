#pragma once
#include "charabase.h"
#include "pch.h"

class AnimationComponent : public Component<CharaBase>
{
public:
	using base = Component<CharaBase>;
	using Anim = CharaBase::STATUS;

	explicit AnimationComponent(CharaBase& owner);
	virtual ~AnimationComponent() override = default;
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual void Update(float deltaTime) override;
	virtual bool Render() override;

	void SetAnimation(
		const std::array<
		AnimationClip,
		std::to_underlying(Anim::COUNT)>& animation
	);

	void ChangeAnimation(Anim anim);
	bool IsAnimationEnd() const;
	float GetAnimPlayTime() const;
	float GetAnimTotalTime() const;

private:
	std::array<AnimationClip,
		std::to_underlying(Anim::COUNT)> _animation;

	std::optional<int> _animId;
	Anim _currentAnim { Anim::NONE };
	float _playTime{ 0.0f };
	bool _loop{ true };

};

