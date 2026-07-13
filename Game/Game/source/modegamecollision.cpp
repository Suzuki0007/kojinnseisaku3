#include "pch.h"
#include "modegame.h"

// コリジョン判定で引っかかった時に、escapeTbl[]順に角度を変えて回避を試みる
bool ModeGame::EscapeCollision()
{
	// プレイヤーが空中なら処理しない
	auto* player = GetPlayer();
	if(!player || !player->GetLand())
	{
		return false;
	}

	// コリジョン判定で引っかかった時に、escapeTbl[]順に角度を変えて回避を試みる
	float escapeTbl[] =
	{
		0, -10, 10, -20, 20, -30, 30, -40, 40, -50, 50, -60, 60, -70, 70, -80, 80,
	};
	for(int i = 0; i < sizeof(escapeTbl) / sizeof(escapeTbl[0]); i++)
	{
		// 移動前の位置を保存
		Vec4 oldvPos = player->GetPos();
		Vec4 v = player->GetInputVector();
		Vec4 oldv = v;
		float rad = atan2((float)v.z, (float)v.x);
		float inputLen = sqrt(v.z * v.z + v.x * v.x);
		if(inputLen < 0.01f)
		{
			// 入力がほぼ無いなら処理しない
			break;
		}
		float length = player->GetMoveSpeed() * sqrt(v.z * v.z + v.x * v.x);
		float sx = _camera->_v_pos.x - _camera->_v_target.x;
		float sz = _camera->_v_pos.z - _camera->_v_target.z;
		float camrad = atan2(sz, sx);

		// escapeTbl[i]の分だけ移動量v回転
		float escape_rad = DEG2RAD(escapeTbl[i]);
		v.x = cos(rad + camrad + escape_rad) * length;
		v.z = sin(rad + camrad + escape_rad) * length;

		// vの分移動
		player->SetPos(v::VAdd(player->GetPos(), v));

		// コリジョン処理しないならループから抜ける
		if(!_d_use_collision)
		{
			//カメラも移動する
			break;
		}

		// 移動した先でコリジョン判定
		MV1_COLL_RESULT_POLY hitPoly;

		// 主人公の腰位置から下方向への直線
		hitPoly = VC::MV1CollCheckLine
		(
			_map->GetHandleMap(),
			_map->GetFrameMapCollision(),
			v::VAdd(player->GetPos(),
				VC::DxLibToVec(VGet(0, player->GetColSubY(), 0))),
			v::VAdd(player->GetPos(),
				VC::DxLibToVec(VGet(0, -99999.f, 0)))
		);
		if(hitPoly.HitFlag)
		{
			// 当たった
			// 当たったY位置をキャラ座標にする
			Vec4 tmpPos = player->GetPos();
			tmpPos.y = hitPoly.HitPosition.y;
			player->SetPos(tmpPos);

			// キャラが上下に移動した量だけ、移動量を修正
			v.y += player->GetPos().y - oldvPos.y;

			// ループiから抜ける
			break;
		}
		else
		{
			// 当たらなかった。元の座標に戻す
			player->SetPos(oldvPos);
			v = oldv;
		}
	}
	return true;
}

bool ModeGame::CharaToCharaCollision(CharaBase* c1, CharaBase* c2)
{
	if(!c1 || !c2) {
		return false;
	}

	// カプセル上下を生成（top: +Y, bottom: -Y）
	Vec4 c1_pos = c1->GetPos();
	Vec4 c2_pos = c2->GetPos();

	float c1_half = c1->GetColSubY();
	float c2_half = c2->GetColSubY();

	Vec4 c1_top = v::VAdd(c1_pos, v::VGet(0.0f, c1_half, 0.0f));
	Vec4 c1_bottom = v::VAdd(c1_pos, v::VGet(0.0f, -c1_half, 0.0f));
	Vec4 c2_top = v::VAdd(c2_pos, v::VGet(0.0f, c2_half, 0.0f));
	Vec4 c2_bottom = v::VAdd(c2_pos, v::VGet(0.0f, -c2_half, 0.0f));

	float c1_r = (float)c1->GetCollisionR();
	float c2_r = (float)c2->GetCollisionR();

	// カプセル同士が当たっていなければ終了
	if(!VC::HitCheckCapsuleToCapsule(c1_top, c1_bottom, c1_r, c2_top, c2_bottom, c2_r))
	{
		return false;
	}

	// 重さの軽い方をmove、重い方をstopにする（同じなら処理しない）
	CharaBase* move = nullptr;
	CharaBase* stop = nullptr;
	if(c1->GetCollisionWeight() < c2->GetCollisionWeight())
	{
		move = c1;
		stop = c2;
	}
	else if(c2->GetCollisionWeight() < c1->GetCollisionWeight())
	{
		move = c2;
		stop = c1;
	}
	else
	{
		return false;
	}

	// 設定が有効なら押し出す
	if(_d_use_collision)
	{
		PushChara(move, stop);
	}

	return true;
}

bool ModeGame::CharaToCubeCollision(CharaBase* chara, Cube* cube)
{
	auto player = GetPlayer();

	if(!chara || !cube)
	{
		return false;
	}

	if(cube->GetVertexCount() == 0)
	{
		return false;
	}

	if(!_d_use_collision && chara == player)
	{
		return false;
	}

	mymath::AABB box = cube->GetAABB();
	Vec4 pos = chara->GetPos();
	float r = (float)chara->GetCollisionR();

	if(!IsHitSphereAABB(pos, r, box))
	{
		return false;
	}

	float dxmin = box.min.x - (pos.x + r);
	float xmin = dxmin * dxmin;
	float dxmax = box.max.x - (pos.x - r);
	float xmax = dxmax * dxmax;
	float dymin = box.min.y - (pos.y + r);
	float ymin = dymin * dymin;
	float dymax = box.max.y - (pos.y - r);
	float ymax = dymax * dymax;
	float dzmin = box.min.z - (pos.z + r);
	float zmin = dzmin * dzmin;
	float dzmax = box.max.z - (pos.z - r);
	float zmax = dzmax * dzmax;

	float resolverx;// x方向の押し出し量
	if(xmin < xmax)
	{
		resolverx = dxmin;
	}
	else
	{
		resolverx = dxmax;
	}

	float resolvery;
	if(ymin < ymax)
	{
		resolvery = dymin;
	}
	else
	{
		resolvery = dymax;
	}

	float resolverz;
	if(zmin < zmax)
	{
		resolverz = dzmin;
	}
	else
	{
		resolverz = dzmax;
	}

	float absx = resolverx * resolverx;
	float absy = resolvery * resolvery;
	float absz = resolverz * resolverz;

	// 一番小さい押し出し量でキャラを押し出す
	if(absx <= absy && absx <= absz)
	{
		pos.x += resolverx;
	}
	else if(absy <= absx && absy <= absz)
	{
		pos.y += resolvery;
		_resolve_on_y = true;
		float land_y = (resolvery - dymax) * (resolvery - dymax);
		if(land_y == 0.0f)
		{
			_landed_on_up = true;
		}
	}
	else if(absz <= absx && absz <= absy)
	{
		pos.z += resolverz;
	}

	chara->SetPos(pos);

	if(_d_use_collision && _resolve_on_y && _landed_on_up)
	{
		// プレイヤーが「空中攻撃中」であれば即時に着地フラグを立てない（アニメーションが上書きされるのを防ぐ）
		bool suppressLand = false;
		if(player != nullptr)
		{
			// プレイヤーが攻撃ステータスかつ現在は空中扱い（GetLand() == false）なら抑止
			if(player->GetStatus() == CharaBase::STATUS::ATTACK && !player->GetLand())
			{
				suppressLand = true;
			}
		}

		// 位置はキューブ上に合わせるが、攻撃中なら着地フラグは次フレーム以降に任せる
		Vec4 tmpPos = chara->GetPos();
		float y_offset = 0.0f;  // 必要なら調整
		tmpPos.y = box.max.y + y_offset;
		chara->SetPos(tmpPos);

		if(!suppressLand)
		{
			player->SetLand(true);
		}
		return true;
	}

	if(_d_use_collision)
	{
		MV1_COLL_RESULT_POLY hitpoly;
		hitpoly = VC::MV1CollCheckLine(
			_map->GetHandleMap(),
			_map->GetFrameMapCollision(),
			v::VAdd(chara->GetPos(),
			v::VGet(0.0f, chara->GetColSubY(), 0.0f)),
			v::VAdd(chara->GetPos(), v::VGet(0.0f, -9999.f, 0.0f))
		);
		if(hitpoly.HitFlag)
		{
			float ground_y = hitpoly.HitPosition.y;

			if(_landed_on_up)
			{
				//当たったY位置をキャラ座標にする
				Vec4 tmpPos = chara->GetPos();
				tmpPos.y = hitpoly.HitPosition.y;
				chara->SetPos(tmpPos);
				player->SetLand(true);
			}
			else
			{
				if(chara->GetPos().y > ground_y)
				{
					player->SetLand(false);
				}
				else
				{
					Vec4 tmpPos = chara->GetPos();
					tmpPos.y = ground_y;
					chara->SetPos(tmpPos);
					player->SetLand(true);
				}
			}
			return true;
		}
	}
	return false;
}

bool ModeGame::LandCheck()
{
	auto player = GetPlayer();
	if(player->GetLand())
	{
		bool is_ground = false;

		Vec4 pos = player->GetPos();

		// 地面にいるか？
		if(pos.y <= 0.0f)
		{
			is_ground = true;
		}

		// マップにいるか？
		if(!is_ground)
		{
			Vec4 start = v::VAdd(pos, v::VGet(0.0f, 0.0f, 0.0f));
			Vec4 end = v::VAdd(pos, v::VGet(0.0f, -50.0f, 0.0f));

			MV1_COLL_RESULT_POLY hitpoly;
			hitpoly = VC::MV1CollCheckLine(
				_map->GetHandleMap(),
				_map->GetFrameMapCollision(),
				start,
				end
			);
			if(hitpoly.HitFlag)
			{
				is_ground = true;
			}
		}

		// キューブの上にいるか？ - _landed_on_upフラグで判定
		if(!is_ground && _d_use_collision)
		{
			is_ground = _landed_on_up;
		}

		// どの足場にも乗っていなければ、空中状態にする
		if(!is_ground)
		{
			player->SetLand(false);
		}
	}
	return false;
}

bool ModeGame::CheckEncount()
{
	auto* player = GetPlayer();
	auto& enemies = GetEnemies();

	for(int i = 0; i < enemies.size(); i++)
	{
		if(!_enemyAliveList[i])
		{
			continue;
		}

		auto& enemy = enemies[i];

		// 円同士の当たり判定チェック
		if(IsHitCircle(player, enemy.get()))
		{
			// 戦う敵の番号
			//_enemyIndexBattle = i;

			// バトルシーンへ
			//ChangeState(GameState::Battle, i);

			return true;
		}
	}
	return false;
}

// キャラ同士の押し出し処理
bool ModeGame::PushChara(CharaBase* move, CharaBase* stop)
{
	if(!move || !stop) {
		return false;
	}

	// 移動前の位置を保存
	Vec4 oldpos = move->GetPos();

	// [stop]の半径に当たらない位置まで、[move]を押し出す
	// [stop]の中心位置から、[move]の中心位置までの角度を得る
	// 角度は atan2() で求められる。ラジアン値なので注意
	float rad = atan2((float)(move->GetPos().z - stop->GetPos().z), (float)(move->GetPos().x - stop->GetPos().x));

	// [stop]の中心位置から、rad角度で [stop].r+[move].r の距離の位置に、[move]の中心位置を設定する
	float lenght = stop->GetCollisionR() + move->GetCollisionR() + 2.0f;
	Vec4 newPos = move->GetPos();
	newPos.x = stop->GetPos().x + cos(rad) * lenght;
	newPos.z = stop->GetPos().z + sin(rad) * lenght;
	move->SetPos(newPos);

	// コリジョン処理するか？
	if(_d_use_collision)
	{
		// 移動した場合、マップコリジョンから出ていないか？
		MV1_COLL_RESULT_POLY hitpoly;

		// 主人公の腰位置から下方向への直線
		hitpoly = VC::MV1CollCheckLine(
			_map->GetHandleMap(),
			_map->GetFrameMapCollision(),
			v::VAdd(move->GetPos(), 
			v::VGet(0.0f, move->GetColSubY(), 0.0f)),
			v::VAdd(move->GetPos(),
			v::VGet(0.0f, -9999.f, 0.0f))
		);
		if(hitpoly.HitFlag)
		{
			// 当たったY位置をキャラ座標にする
			Vec4 tmpPos = move->GetPos();
			tmpPos.y = hitpoly.HitPosition.y;
			move->SetPos(tmpPos);
		}
	}
	return true;
}

// ---------------------------------------------------------
// 攻撃判定の更新・追従・衝突チェックを一括で行う
// ---------------------------------------------------------
bool ModeGame::UpdateCheckAttackCollision()
{
	auto player = GetPlayer();
	// 1. プレイヤーの攻撃リストを参照させる（参照を明示的に取得）
	auto& attack_list = player->GetAttackCollisionList();

	// デバッグ: 攻撃リストのサイズを表示
	if(_d_view_collision)
	{
		DrawFormatString(10, 150, GetColor(255, 255, 0), "Attack List Size: %d", attack_list.size());
	}

	// 2. まず全ての攻撃を処理（範囲ベースforループ）
	//int index = 0;
	for(auto& attack : attack_list)
	{
		// 2-1.待機時間がある場合、待機時間を減らして次へ
		if(attack.waittime > 0)
		{
			attack.waittime--;
			//index++;
			continue;
		}
		// 2-2.有効時間がある場合、有効時間を減らして次へ
		if(attack.activetime > 0)
		{
			attack.activetime--;

			// 剣先に合わせてカプセル座標を更新
			if(attack.follow)
			{
				int model_handle = attack.capsule.modelhandle;
				int bone_num = attack.capsule.framenum;

				// フレーム(ボーン)のローカル行列を取得
				Mat4 frame_local = MC::MV1GetFrameLocalMatrix(model_handle, bone_num);
				// モデルのローカルワールド行列（SetPosition/Rotation 適用後）
				Mat4 model_world = MC::MV1GetMatrix(model_handle);
				// フレームのワールド行列 = フレームローカル × モデルワールド
				Mat4 frame_world = MC::MMult(frame_local, model_world);

				// 剣の根元と先端のローカル座標
				Vec4 local_under_pos = v::VGet(50.0f, 50.0f, -100.0f);
				Vec4 local_over_pos = v::VGet(50.0f, 50.0f, -100.0f);

				// ワールド座標に変換
				attack.capsule.underpos = v::VTransform(local_under_pos, frame_world);
				attack.capsule.overpos = v::VTransform(local_over_pos, frame_world);
			}

			// 敵との当たり判定チェック
			auto& enemies = GetEnemies();
			for(auto& enemy : enemies)
			{
				if(attack.isHit)
				{
					// すでに当たっているならスキップ
					continue;
				}

				// 当たり判定チェック
				Vec4 c1_top = attack.capsule.overpos;
				Vec4 c1_bottom = attack.capsule.underpos;
				float c1_r = attack.capsule.r;
				Vec4 c2_pos = enemy->GetPos();
				float c2_half = enemy->GetColSubY();
				Vec4 c2_top = v::VAdd(c2_pos, v::VGet(0.0f, c2_half, 0.0f));
				Vec4 c2_bottom = v::VAdd(c2_pos, v::VGet(0.0f, -c2_half, 0.0f));
				float c2_r = (float)enemy->GetCollisionR();

				if(VC::HitCheckCapsuleToCapsule(c1_top, c1_bottom, c1_r, c2_top, c2_bottom, c2_r))
				{
					// 当たった
					attack.isHit = true;
					// ダメージ処理
					enemy->Damage(attack.damage);


					break; // 1回当たったら終了
				}


			}
		}
	}

	// 3. 有効時間が終了した攻撃を削除（逆順にループして削除）
	for(int i = (int)attack_list.size() - 1; i >= 0; --i)
	{
		if(attack_list[i].waittime <= 0 && attack_list[i].activetime <= 0)
		{
			attack_list.erase(attack_list.begin() + i);
		}
	}

	return true;
}