#pragma once
#include "pch.h"
#include "charabase.h"
#include "camera.h"

struct TargetInfo
{
	CharaBase* target { nullptr };	// 対象のキャラクター

	float score{ 0.0f };			// スコア

	float angle { 0.0f };			// 角度
	float distance { 0.0f };		// 距離
	float side { 0.0f };			// 左右の位置関係

};

struct TargetContext
{
	const std::vector<CharaBase*>& target;	// 対象のキャラクターリスト
	const Vec4& selfPos;					// 自身の位置(今回はplayerの位置)
	Camera* camera;
};

class ITargetStrategy
{
public:
	ITargetStrategy() = default;
	virtual ~ITargetStrategy() = default;

	// ターゲットの切り替えやロック中の対象が有効かどうか
	virtual std::vector<TargetInfo> CollectCandidate
	(
		const TargetContext& context
	) const = 0;
};

