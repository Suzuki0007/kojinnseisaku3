#include "pch.h"
#include "PlayerManager.h"
#include "player.h"
#include "player2.h"
#include "Player3.h"

void PlayerManager::RegisterType()
{
	OMB::RegisterType("Player", []() { return std::make_unique<Player>(); });
	OMB::RegisterType("Player2", []() { return std::make_unique<Player2>(); });
	OMB::RegisterType("Player3", []() { return std::make_unique<Player3>(); });

}

void PlayerManager::CreatePlayer(const std::string& type)
{
	Create(type);
}