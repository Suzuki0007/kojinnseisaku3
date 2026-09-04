#pragma once
#include "targetprovider.h"

class Cube;

class EnemyTargetProvider final : public ITargetProvider
{
public:
	EnemyTargetProvider() = delete;
    explicit EnemyTargetProvider(const std::vector<std::shared_ptr<Cube>>& cubes);

    virtual ~EnemyTargetProvider() = default;

    std::vector<CharaBase*> GetTargets() const override;

	// 2点間の線分が障害物に遮られているかどうかを判定する関数
	bool IsBlocked(const Vec4& from, const Vec4& to) const override;

private:
    const std::vector<std::shared_ptr<Cube>>& _cubes;
};

