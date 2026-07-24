#include "pch.h"
#include "modegame.h"
#include "menuitembase.h"

// デバックの初期化
bool ModeGame::DebugInitialize()
{
	// その他初期化
	_d_view_collision = false;
	_d_use_collision = true;
	_d_view_camera_info = true;
	_d_view_shadow_map = false;

	return true;
}

bool ModeGame::DebugProcess()
{
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	InputDevice& input = InputLocator::Get();
	// ESCキーでメニューを開く
	if(input.IsTrigger(InputButton::Menu))
	{
		ModeMenu* modemenu = new ModeMenu();
		// ModeGameより上のレイヤーにメニューを登録する
		ModeServer::GetInstance()->Add(modemenu, 99, "menu");
		// ModeMenuにメニュー項目を追加する
		modemenu->Add(new MenuItemViewCollision(this, "ViewCollision"));
		modemenu->Add(new MenuItemUseCollision(this, "UseCollision"));
		modemenu->Add(new MenuItemViewCameraInfo(this, "ViewCameraInfo"));
		modemenu->Add(new MenuItemLaunchEffekseer(this, "Effekseer"));
		modemenu->Add(new MenuItemViewShadowMap(this, "ShadowMapView"));
	}

	// デバッグ機能
	if(trg & PAD_INPUT_6)
	{
		_d_view_collision = !_d_view_collision;
	}
	if(_d_view_collision)
	{
			MV1SetFrameVisible(_map->GetHandleMap(), _map->GetFrameMapCollision(), TRUE);
		
	}
	else
	{
			MV1SetFrameVisible(_map->GetHandleMap(), _map->GetFrameMapCollision(), FALSE);
	}

	return true;
}

// デバック用の描画
bool ModeGame::DebugRender()
{

	// 0,0,0を中心に線を引く
	{
		float linelenght = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelenght, 0, 0)), VAdd(v, VGet(linelenght, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelenght, 0)), VAdd(v, VGet(0, linelenght, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelenght)), VAdd(v, VGet(0, 0, linelenght)), GetColor(0, 0, 255));
	}

	// カメラターゲットを中心に短い線を引く
	{
		float linelength = 10.f;
		if(_camera)
		{
			VC::DrawLine3D(v::VAdd(_camera->_v_target, v::VGet(-linelength, 0.0f, 0.0f)), v::VAdd(_camera->_v_target, v::VGet(linelength, 0.0f, 0.0f)), GetColor(255, 255, 0));
			VC::DrawLine3D(v::VAdd(_camera->_v_target, v::VGet(0.0f, -linelength, 0.0f)), v::VAdd(_camera->_v_target, v::VGet(0.0f, linelength, 0.0f)), GetColor(255, 255, 0));
			VC::DrawLine3D(v::VAdd(_camera->_v_target, v::VGet(0.0f, 0.0f, -linelength)), v::VAdd(_camera->_v_target, v::VGet(0.0f, 0.0f, linelength)), GetColor(255, 255, 0));
		}
	}

	if(_d_view_collision)
	{
		auto* player = PlayerManager::GetInstance()->GetPlayer().front().get();
		if(!player) { return false; }
		auto& attack_list = player->GetAttackCollisionList();
		for(auto& attack : attack_list)
		{
			// 有効レンジのみ描画
			if(attack.waittime > 0 || attack.activetime <= 0) continue;

			Vec4 c1_top = attack.capsule.overpos;
			Vec4 c1_bottom = attack.capsule.underpos;
			float c1_r = attack.capsule.r;

			// 攻撃カプセル: ワイヤー
			VC::DrawCapsule3D(c1_top, c1_bottom, c1_r, 8, GetColor(255, 0, 0), GetColor(255, 255, 255), DX_FILL_WIREFRAME);

			// 敵カプセルを全敵描画（ワイヤー）
			auto& enemies = GetEnemies();
			for(auto& enemy : enemies)
			{
				Vec4 c2_pos = enemy->GetPos();
				float c2_half = enemy->GetColSubY();
				Vec4 c2_top = v::VAdd(c2_pos, v::VGet(0.0f, c2_half, 0.0f));
				Vec4 c2_bottom = v::VAdd(c2_pos, v::VGet(0.0f, -c2_half, 0.0f));
				float c2_r = (float)enemy->GetCollisionR();
				VC::DrawCapsule3D(c2_top, c2_bottom, c2_r, 8, GetColor(0, 0, 255), GetColor(255, 255, 255), DX_FILL_WIREFRAME);
			}

			// ヒット時の強調（塗りつぶし）
			if(attack.isHit)
			{
				VC::DrawCapsule3D(c1_top, c1_bottom, c1_r, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), DX_FILL_SOLID);
			}
		}
	}

	// シャドウマップの表示
	if(_d_view_shadow_map)
	{
		TestDrawShadowMap(_map->GetHandleShadowMap(), 0, 0, 512, 512);
	}

	if(_d_view_camera_info)
	{
		_camera->Render();
	}

	{
		int x = 10;
		int y = 200;
		int color = GetColor(255, 255, 255);
		SetFontSize(16);
		// 表示内容：Aボタンがダッシュ、2回押しで空中ダッシュ、Bボタンが攻撃、Xボタンがドッジロール
		DrawFormatString(x, y, color, "A: ダッシュ (2回押しで空中ダッシュ)");
		DrawFormatString(x, y + 18, color, "B: 攻撃(ジャンプ攻撃可)");
		DrawFormatString(x, y + 36, color, "X: ドッジロール");
		DrawFormatString(x, y + 54, color, "敵に当たったらコマンドバトル");
	}

	return true;
}

