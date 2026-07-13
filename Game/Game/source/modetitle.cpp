#include "pch.h"
#include "modetitle.h"
#include "modegame.h"

// 初期化
bool ModeTitle::Initialize()
{
	if(!base::Initialize()) {
		return false;
	}
	return true;
}

// 終了
bool ModeTitle::Terminate()
{
	base::Terminate();
	return true;
}

// 計算処理
bool ModeTitle::Process()
{
	base::Process();
	int trg = ApplicationBase::GetInstance()->GetTrg();

	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();

	// スタート（PAD_INPUT_1）でゲームモードへ遷移
	if(trg & PAD_INPUT_1)
	{
		// 重複登録防止
		if(ModeServer::GetInstance()->Get("game") == nullptr)
		{
			// レイヤーは game と重ならない適切な値を指定（例: 1）
			ModeServer::GetInstance()->Get("game");
			if(ModeServer::GetInstance()->Get("game") == nullptr)
			{
				ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
			}
		}

		// 自分を削除予約（安全に次フレームで削除される）
		ModeServer::GetInstance()->Del(this);
	}
	return true;
}

// 描画処理
bool ModeTitle::Render()
{
	base::Render();
	int x = 200, y = 200, size = 64;
    SetFontSize(size);
    DrawFormatString(350, y, GetColor(255, 255, 0), "ルアコマンド");
    y += 64;
    // 操作説明はフォントを小さくする
    SetFontSize(size / 2);
    DrawFormatString(x + 100, y + 200, GetColor(255, 255, 255), "キーボード:Zまだはパッド:Aでスタート");
	return true;
}