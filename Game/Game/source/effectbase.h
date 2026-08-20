#pragma once
#include "charabase.h"
class EffectBase : public CharaBase
{
	using base = CharaBase;

public:
	EffectBase() = default;
	virtual ~EffectBase() = default;
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

};

