#pragma once
#include "pch.h"
#include "charabase.h"

// ターゲット提供者のインターフェース
// プロバイダーパターンとは、インターフェースを通じて、具体的な機能（実装）を提供する
class ITargetProvider
{
public:
	ITargetProvider() = default;
	virtual ~ITargetProvider() = default;

	virtual std::vector<CharaBase*> GetTargets() const = 0;
};

