#pragma once
#include "effectbase.h"
#include "effectfactory.h"
#include "objectmanagerbase.h"

class EffectManager : public Singleton<EffectManager>, public ObjectManagerBase<EffectBase, EffectFactory>
{
	friend class Singleton<EffectManager>;

public:
	using OMB = ObjectManagerBase<EffectBase, EffectFactory>;

	void RegisterType();
	EffectBase* CreateEffect(
		const std::string& type,
		const Vec4& pos,
		const Vec4& dir = Vec4::Zero(),
		float distance = 0.0f);

	
	std::vector<std::unique_ptr<EffectBase>>& GetEffects() { return GetAll(); }

private:
	EffectManager() = default;
	virtual ~EffectManager() = default;
};
