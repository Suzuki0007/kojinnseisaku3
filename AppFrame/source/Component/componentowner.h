#pragma once
#include <memory>
#include <vector>
#include "component.h"

template <class TOwner>
class ComponentOwner
{
public:
	// コンポーネントを追加する関数
	// Argsはコンポーネントのコンストラクタに渡す引数の型
	// Tはコンポーネントの型
	// std::derived_from:型が、クラスの子クラスであるかどうかを
	// コンパイル時にチェックする仕組み
	template <class T, class... Args>
		requires std::derived_from<T, Component<TOwner>>
	T* AddComponent(TOwner& owner, Args&&... args)
	{
		// std::make_unique:引数を渡して、T型のオブジェクトを作成する関数
		// std::forward:引数を完全転送するための関数
		auto component = std::make_unique<T>(owner, std::forward<Args>(args)...);
		auto& ref = *component; // 追加するコンポーネントの参照を取得
		_component.emplace_back(std::move(component)); // コンポーネントをベクターに追加
		printf("AddComponent owner = %p\n", &owner);
		return &ref;
	}

	void Initialize()
	{
		for(auto& component : _component)
		{
			component->Initialize(); // 各コンポーネントのInitialize関数を呼び出す
		}
	}

	void Terminate()
	{
		for(auto& component : _component)
		{
			component->Terminate(); // 各コンポーネントのTerminate関数を呼び出す
		}
	}

	void Update(float deltaTime)
	{
		for(auto& component : _component)
		{
			component->Update(deltaTime); // 各コンポーネントのUpdate関数を呼び出す
		}
		printf("Update owner = %p\n", this);
	}

	void Render()
	{
		for(auto& component : _component)
		{
			component->Render(); // 各コンポーネントのRender関数を呼び出す
		}
	}

	void Clear()
	{
		_component.clear(); // コンポーネントをクリアする
	}

private:
	std::vector<std::unique_ptr<Component<TOwner>>> _component; // コンポーネント
};