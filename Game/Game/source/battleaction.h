#pragma once
#include "battlereceiver.h"
#include "pch.h"

class BattleAction
{
public:
	virtual ~BattleAction() = default;

	virtual std::string GetName() const = 0;

	// 攻撃コマンドの実行(誰が誰に攻撃するのかを引数に取る)
	virtual void Execute(IBattleReceiver* user, IBattleReceiver* target) = 0;
};
