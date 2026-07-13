#pragma once
#include "pch.h"
#include "charabase.h"
#include "camera.h"

class ITargetStrategy
{
public:
	ITargetStrategy() = default;
	virtual ~ITargetStrategy() = default;

	virtual std::optional<CharaBase*> SelectTarget
	(
		const std::vector<CharaBase*>& target,	// 対象のキャラクターリスト
		const Vec4& selfPos,					// 自身の位置(今回はplayerの位置)
		Camera* camera
	) const = 0;
};

