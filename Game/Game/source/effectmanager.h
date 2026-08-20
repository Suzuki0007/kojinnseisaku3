#pragma once
#include "effectbase.h"
#include "effectfactory.h"
#include "objectmanagerbase.h"

class EffectManager : public Singleton<EffekseerManager>, public ObjectManagerBase<EffectBase, EffectFactory>
{
	friend class Singleton<EffectManager>;

public:
	using OMB = ObjectManagerBase<EffectBase, EffectFactory>;

	void RegisterType();
	void CreateEffect(const std::string& type);
	
	std::vector<std::unique_ptr<EffectBase>>& GetEffects() { return GetAll(); }

private:
	EffectManager() = default;
	virtual ~EffectManager() = default;
};
