#pragma once
#include "playerbase.h"
#include "playerfactory.h"
#include "objectmanagerbase.h"

class PlayerManager : public Singleton<PlayerManager>, public ObjectManagerBase<PlayerBase, PlayerFactory>
{
	friend class Singleton<PlayerManager>;
public:
	using OMB = ObjectManagerBase<PlayerBase, PlayerFactory>;

	void RegisterType();
	void CreatePlayer(const std::string& type);
	const std::vector<std::unique_ptr<PlayerBase>>& GetPlayer() { return GetAll(); }

private:
	PlayerManager() = default;
	virtual ~PlayerManager() = default;
};

