#pragma once
#include "charabase.h"
#include "waitstate.h"
#include "battlestate.h"

template<typename State, typename Owner>
//conceptはテンプレートの引数が特定の要件を満たすかどうかをコンパイル時にチェックできる
concept IsCharaState = requires(State state, Owner * owner)
{
	// std::same_asは、2つの型が同じであるかどうかを判定するための型特性
	{ state.Enter(owner) } -> std::same_as<void>;
	{ state.Update(owner) } -> std::same_as<void>;
	{ state.Exit(owner) } -> std::same_as<void>;
};

class StateManager : public Component<CharaBase>
{
	using base = Component<CharaBase>;
public:

	explicit StateManager(CharaBase* owner = nullptr);

	void ChangeBattleMode(bool isBattle);

	template <typename T>
	bool IsInState() const
	{
		return _stateMachine.IsInState<T>();
	}

	bool Process() override;

private:
	StateMachine<WaitState, BattleState> _stateMachine;
};

