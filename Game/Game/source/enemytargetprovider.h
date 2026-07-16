#pragma once
#include "targetprovider.h"

class EnemyTargetProvider final : public ITargetProvider
{
public:
    EnemyTargetProvider() = default;
    virtual ~EnemyTargetProvider() = default;

    std::vector<CharaBase*> GetTargets() const override;
};

