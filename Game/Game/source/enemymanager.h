#pragma once
#include "enemyfactory.h"
#include "objectmanagerbase.h"

class EnemyManager : public Singleton<EnemyManager>, public ObjectManagerBase<EnemyBase, EnemyFactory>
{
    friend class Singleton<EnemyManager>;

public:
	using OMB = ObjectManagerBase<EnemyBase, EnemyFactory>;

    void RegisterType();
    void CreateEnemy(const std::string& type);
	std::vector<std::unique_ptr<EnemyBase>>& GetEnemies() { return GetAll(); }

private:
    EnemyManager() = default;
	virtual ~EnemyManager() = default;
};
