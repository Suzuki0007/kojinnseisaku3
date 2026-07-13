#pragma once
#include <variant>
#include <utility>

// typename ... はいくつもの型を受け取ることができる
template <typename... States>
class StateMachine
{
public:
	// コンストラクタは、初期状態を受け取る
	template <typename InitialStateType>
	explicit StateMachine(InitialStateType&& initialState)
		: _States(std::forward<InitialStateType>(initialState))
	{}

	// 現在の状態を取得する関数
	template <typename T, typename... Args>
	void ChangeState(Args&&... args)
	{	
		_States.emplace<T>(std::forward<Args>(args)...);
	}

	// 現在の状態が特定の型であるかどうかをチェックする関数
	template <typename T>
	bool IsInState() const
	{
		// std::holds_alternativeは、std::variantが特定の型を保持しているかどうかをチェックする
		return std::holds_alternative<T>(_States);
	}

	// 現在の状態に対して、visitorに適用する関数
	template <typename Visitor>
	auto Execute(Visitor&& visitor)
	{
		// std::visitは、std::variantが保持している値に対して、適用する
		return std::visit(std::forward<Visitor>(visitor), _States);
	}

	// constバージョンのExecute関数
	// const修飾子を付けることで、状態を変更せずにアクセスできるようにする
	template <typename Visitor>
	auto Execute(Visitor&& visitor) const
	{
		return std::visit(std::forward<Visitor>(visitor), _States);
	}

private:
	// std::variantは、複数の型のうちどれか1つを保持できる型
	std::variant<States...> _States;
};

