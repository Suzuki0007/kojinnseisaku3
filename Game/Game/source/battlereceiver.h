#pragma once

class IBattleReceiver
{
public:
	virtual ~IBattleReceiver() = default;
	
	// 攻撃コマンドの実行(攻撃対象を引数に取る)
	virtual void CommandAttack(IBattleReceiver* target) = 0;

	// 攻撃コマンドの実行中かどうかを返す
	virtual bool IsExceutionAction() const = 0;
};

