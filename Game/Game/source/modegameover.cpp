#include "pch.h"
#include "modegameover.h"
#include "modetitle.h"

bool  ModeGameOver::Initialize()
{
	if(!base::Initialize()) { return false; }
	return true;
}

bool  ModeGameOver::Terminate()
{
	base::Terminate();
	return true;
}

bool  ModeGameOver::Process()
{
	base::Process();
	int trg = ApplicationBase::GetInstance()->GetTrg();

	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();

	// スタート（PAD_INPUT_1）でゲームモードへ遷移
	if(trg & PAD_INPUT_1)
	{
		// 重複登録防止
		if(ModeServer::GetInstance()->Get("title") == nullptr)
		{
			// レイヤーは title と重ならない適切な値を指定（例: 1）
			ModeServer::GetInstance()->Get("title");
			if(ModeServer::GetInstance()->Get("title") == nullptr)
			{
				ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
			}
		}

		// 自分を削除予約（安全に次フレームで削除される）
		ModeServer::GetInstance()->Del(this);
	}
	return true;
}

bool  ModeGameOver::Render()
{
	if(!base::Render()) { return false; }

	// 中央付近に Game Over と倒した数と残り時間を表示
	int cx = 300;
	int cy = 200;
	SetFontSize(64);
	DrawFormatString(cx + 200, cy, GetColor(255, 255, 255), "ゲームクリア");
	SetFontSize(32);
	// 操作説明
	DrawFormatString(cx + 50, cy + 300, GetColor(255, 255, 255), "キーボード:Zまだはパッド:Aでタイトルへ");
	// フォントサイズを戻す（他描画に影響しないように）
	SetFontSize(16);
	return true;
}