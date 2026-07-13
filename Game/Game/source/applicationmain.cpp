#include "pch.h"
#include "applicationmain.h"
#include "applicationglobal.h"
#include "modegame.h"
#include "modetitle.h"

// 実体
ApplicationMain g_application_main;

// 初期化
bool ApplicationMain::Initialize(HINSTANCE hInstance)
{
	if(!base::Initialize(hInstance)) { return false; }

	auto padInput = std::make_shared<PadInput>();
	auto keyboardInput = std::make_shared<KeyboardInput>();

	if(GetJoypadNum() > 0)
	{
		InputLocator::Provide(padInput);
	}
	else
	{
		InputLocator::Provide(keyboardInput);
	}

	// アプリケーショングローバルの初期化
	gGlobal.Init();

	// モードの登録
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
	ModeServer::GetInstance()->Add(new ModeGame(), 0, "game");

	return true;
}

// 終了
bool ApplicationMain::Terminate()
{
	base::Terminate();
	return true;
}

// 入力処理
bool ApplicationMain::Input()
{
	base::Input();
	return true;
}

// 計算処理
bool ApplicationMain::Process()
{
	base::Process();
	return true;
}

// 描画処理
bool ApplicationMain::Render()
{
	base::Render();
	return true;
}
