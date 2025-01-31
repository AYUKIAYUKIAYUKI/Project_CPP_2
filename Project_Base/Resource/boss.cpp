//============================================================================
// 
// ボス [boss.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "boss.h"
#include "bounding_cylinder.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "field_manager.h"
#include "field_builder.h"
#include "object_parts.h"
#include "collision.h"

#include "player.h"
#include "sparks.h"
#include "scene.h"
#include "game.h"

#include "field_type.h"
#include "block.h"
#include "bright.h"
#include "enemy.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CBoss::CBoss() :
	CCharacter{ LAYER::DEFAULT },
	m_ActionType{ ACTION::HOLDCENTER },
	m_nCntActionCast{ 0 },
	m_nDuration{ 0 },
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() }
{

}

//============================================================================
// デストラクタ
//============================================================================
CBoss::~CBoss()
{
	// バウンディングシリンダーの破棄
	if (m_pBndCylinder != nullptr)
	{
		// メモリを解放
		delete m_pBndCylinder;

		// ポインタを初期化
		m_pBndCylinder = nullptr;
	}
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CBoss::Init()
{
	// 補正強度を設定
	SetCorrectCoef(CORRECT_COEF);

	// 初期体力を設定
	SetLife(MAX_LIFE);

	// キャラクタークラスの初期設定
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "ボスクラスの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CBoss::Uninit()
{
	// キャラクタークラスの終了処理
	CCharacter::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CBoss::Update()
{
	// 登場モーションが終了したら
	if (GetNowMotion() == 0 && GetStopState())
	{
		// 通常モーションへ
		SetNowMotion(1);

		// 行動キャストをリセット
		m_nCntActionCast = 0;

		// カメラのプレイヤー追従もこの時に戻す
		CManager::GetManager()->GetCamera()->ChangeTrackPlayer(true);
	}

	// 火の粉を猛発生させる
	CSparks::FuryGenerate();

	// 次の行動を決定
	SetNextAction();

	// 行動分岐
	BranchAction();

	// 円柱バウンディングの中心点をモデルの中心に
	m_pBndCylinder->SetCenterPos(GetPos());

	// キャラクタークラスの更新処理
	CCharacter::Update();

#ifdef _DEBUG
	// ウィンドウを表示
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Boss Param"))
	{
		ImGui::Text("ActionType:%d", m_ActionType);
		ImGui::Text("Direction:%.1f", GetDirection() * (180 / D3DX_PI));
		ImGui::Text("DirectionTarget:%.1f", GetDirectionTarget() * (180 / D3DX_PI));
		ImGui::Text("Rot:X %.1f:Y %.1f:Z %.1f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("RotTarget:X %.1f:Y %.1f:Z %.1f", GetRotTarget().x * (180 / D3DX_PI), GetRotTarget().y * (180 / D3DX_PI), GetRotTarget().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.1f:Y %.1f:Z %.1f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::Text("PosTarget:X %.1f:Y %.1f:Z %.1f", GetPosTarget().x, GetPosTarget().y, GetPosTarget().z);
		ImGui::Text("VelY:%.1f", GetVelY());
		ImGui::Text("Life:%d", GetLife());
		ImGui::End();
	}
#endif	// _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CBoss::Draw()
{

}

//============================================================================
// ダメージを受ける
//============================================================================
void CBoss::SetDamage(int nDamage)
{
	if (m_ActionType != ACTION::DAMAGEBACK && m_ActionType != ACTION::DEADEND)
	{
		// ダメージ量分、体力を変動
		int nNewLife = GetLife();
		nNewLife += nDamage;
		SetLife(nNewLife);

		// ダメージ喰らい行動を強制発生
		m_ActionType = ACTION::DAMAGEBACK;

		// 体力がゼロ以下なら死亡行動を強制発生
		if (nNewLife <= 0)
			m_ActionType = ACTION::DEADEND;
	}
}

//============================================================================
// 半径を取得
//============================================================================
float CBoss::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// 高さを取得
//============================================================================
float CBoss::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// バウンディングシリンダーを取得
//============================================================================
const CBounding_Cylinder* const CBoss::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// 生成
//============================================================================
CBoss* CBoss::Create()
{
	// インスタンスを生成
	CBoss* pNewInstance = DBG_NEW CBoss();

	// タイプを設定
	pNewInstance->SetType(TYPE::BOSS);

	// 初期設定
	pNewInstance->Init();

	// モーションをセット
	pNewInstance->SetMotion(LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\BOSS\\pumpkin_motion.json"));

	// 半径を設定
	pNewInstance->m_pBndCylinder->SetRadius(100.0f);

	// 高さを設定
	pNewInstance->m_pBndCylinder->SetHeight(60.0f);

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
//  
//============================================================================

//============================================================================
// 次の行動を決定
//============================================================================
void CBoss::SetNextAction()
{
	// キャストが一定値に達すると
	if (m_nCntActionCast > 240)
	{
		// 行動キャストをリセット
		m_nCntActionCast = 0;

		// ランダムに数値を抽選
		int nRand = rand() % 3;

		// 行動をセット
		switch (nRand)
		{
		case 0:
			m_ActionType = ACTION::DIRECTATTACK;
			break;

		case 1:
			m_ActionType = ACTION::SUMMONENEMY;
			break;

		case 2:
			m_ActionType = ACTION::DIRECTATTACK;
			break;

		default:	// 例外
#ifdef _DEBUG
			assert(false && "ボスの行動決定に例外");
#endif // _DEBUG
			break;
		}
	}
}

//============================================================================
// 行動分岐
//============================================================================
void CBoss::BranchAction()
{
	if (m_ActionType == ACTION::HOLDCENTER)
	{ // 行動が設定されていない時

		// 行動キャストをインクリメント
		++m_nCntActionCast;

		// 中央に待機
		HoldCenter();
	}
	else
	{ // 行動タイプを所持している時

		// タイプに応じて処理を変更
		switch (m_ActionType)
		{
			// 体当たり
		case ACTION::DIRECTATTACK:
			DirectAttack();
			break;

			// 壁づくり
		case ACTION::WALLATTACK:
			WallAttack();
			break;

			// 敵召喚
		case ACTION::SUMMONENEMY:
			SummonEnemy();
			break;

			// ダメージ喰らい
		case ACTION::DAMAGEBACK:
			DamageBack();
			break;

			// 死亡
		case ACTION::DEADEND:
			DeadEnd();
			break;

			// 例外
		case ACTION::HOLDCENTER:
		default:
#ifdef _DEBUG
			assert(false && "ボスの行動に例外発生");
#else
			m_ActionType = ACTION::HOLDCENTER;
#endif // _DEBUG
			break;
		}
	}
}

//============================================================================
// 中心で待機
//============================================================================
void CBoss::HoldCenter()
{
	// プレイヤーを取得
	CPlayer* pPlayer = nullptr;
	pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

	// プレイヤーの方向を向く
	Vec3 Rot = VEC3_INIT, Pos = pPlayer->GetPos();
	Rot.y = atan2f(-Pos.x, -Pos.z);
	SetRotTarget(Rot);	

	// 中央に浮かぶ
	Pos = VEC3_INIT;
	Pos.y = 100.0f;
	SetPosTarget(Pos);
}

//============================================================================
// 体当たり
//============================================================================
void CBoss::DirectAttack()
{
	if (GetNowMotion() != 2)
	{
		// ボスがブルブルモーションに変更
		SetNowMotion(2);

		// この瞬間のプレヤーの方角をコピー
		CPlayer* pPlayer = nullptr;
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
		SetDirectionTarget(pPlayer->GetDirection());

		// エネミー出現音を鳴らす
		CSound::GetInstance()->Play(CSound::LABEL::EAPPEAR);
	}

	// 継続期間をインクリメント
	++m_nDuration;

	if (m_nDuration <= 120)
	{
		// カメラ距離・俯瞰度合いを強制変更
		{
			CCamera* pCamera = CManager::GetManager()->GetCamera();
			float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
			fDinstance += (450.0f - fDinstance) * 0.1f;
			fUpAdjust += (150.0f - fUpAdjust) * 0.05f;
			pCamera->SetDistance(fDinstance);
			pCamera->SetUpAdjust(fUpAdjust);
		}
	}
	else
	{
		// カメラ距離・俯瞰度合いを強制変更
		{
			CCamera* pCamera = CManager::GetManager()->GetCamera();
			float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
			fDinstance += (250.0f - fDinstance) * 0.1f;
			fUpAdjust += (75.0f - fUpAdjust) * 0.05f;
			pCamera->SetDistance(fDinstance);
			pCamera->SetUpAdjust(fUpAdjust);
		}
	}

	// 1.5秒時点で
	if (m_nDuration == 90)
	{
		// 突進目標の座標を作成
		Vec3 PosTarget = {
			cosf(GetDirection()) * CField_Manager::FIELD_RADIUS,
			30.0f,
			sinf(GetDirection()) * CField_Manager::FIELD_RADIUS
		};

		// 目標座標に反映
		SetPosTarget(PosTarget);

		// ボスの攻撃音を鳴らす
		CSound::GetInstance()->Play(CSound::LABEL::BATK);
	}

	// 4秒時点で
	if (m_nDuration == 240)
	{
		// 継続期間をリセット
		m_nDuration = 0;

		// 中央待機に戻る
		m_ActionType = ACTION::HOLDCENTER;

		// 待機モーションに戻す
		SetNowMotion(1);
	}

	// 衝突判定に成功したら
	if (HitCheck())
	{
		// プレイヤーを取得
		CPlayer* pPlayer = nullptr;
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// 原点からキャラクターのいる場所への方向ベクトルを作成
		Vec3 CharacterPosVec = GetPos() - VEC3_INIT;
		D3DXVec3Normalize(&CharacterPosVec, &CharacterPosVec);

		// 原点からプレイヤーのいる場所へのベクトルを作成
		Vec3 PlayerVec = pPlayer->GetPos() - VEC3_INIT;
		D3DXVec3Normalize(&PlayerVec, &PlayerVec);

		// 2本の方向ベクトルの外積を作成
		float fCross = (CharacterPosVec.x * PlayerVec.z) - (CharacterPosVec.z * PlayerVec.x);

		// 半径に応じて衝撃量を作成
		float fImpact = GetRadius() * 0.001f;

		// お互いの位置関係に合わせて吹き飛ばす方角を変更
		if (fCross < 0.0f) fImpact *= -1.0f;

		// 1ダメージと衝撃力を与える
		pPlayer->SetDamage(-1, fImpact);
	}
}

//============================================================================
// 壁作り
//============================================================================
void CBoss::WallAttack()
{
	// 継続期間をインクリメント
	++m_nDuration;

	// カメラ距離・俯瞰度合いを強制変更
	{
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
		fDinstance += (300.0f - fDinstance) * 0.1f;
		fUpAdjust += (100.0f - fUpAdjust) * 0.05f;
		pCamera->SetDistance(fDinstance);
		pCamera->SetUpAdjust(fUpAdjust);
	}

	// ブロックのモデルタイプを事前に選出
	CX_Manager::TYPE Type = CX_Manager::TYPE::BLONORMAL + (rand() % 3);

	{ // 生成用の座標を決定

		// プレイヤーを取得
		CPlayer* pPlayer = nullptr;
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// プレイヤーの目標座標へのベクトルを作成
		Vec3 Norm = pPlayer->GetPosTarget() - pPlayer->GetPos();

		// プレイヤーがある程度移動していることを検知した時のみ
		if (Norm.x * Norm.x + Norm.z * Norm.z > 0.1f)
		{
			// 生成座標計算用 ((方角 + 扇形幅の角度)の場所が生成ポイント)
			float fDirection = pPlayer->GetDirection();		// プレイヤーの現在の方角をコピー
			//float fRange = m_pFan->GetRange();			// 扇形範囲の幅をコピー
			float fRange = 0.5f;
			Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// ブロック用の座標・向きを作成

			// 現在座標と目標座標に対し原点からの方向ベクトルを作成
			Vec3 OldVec = pPlayer->GetPos() - VEC3_INIT, NewVec = pPlayer->GetPosTarget() - VEC3_INIT;
			D3DXVec3Normalize(&OldVec, &OldVec);
			D3DXVec3Normalize(&NewVec, &NewVec);

			// 2本の方向ベクトルの外積を作成
			float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

			// 左に移動している場合角度を反転させる
			if (fCross < 0.0f)
				fRange = -fRange;

			// ①左右の場所は境界線の位置に
			NewPos.x = cosf(fRange + fDirection) * CField_Manager::FIELD_RADIUS;
			NewPos.z = sinf(fRange + fDirection) * CField_Manager::FIELD_RADIUS;

			// ②高さはランダム
			NewPos.y = fabsf(utility::GetRandomValue<float>());

			/**/
			bool bSafe = true;
#if 1
			{
				// 通常優先度のオブジェクトを取得
				CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

				while (pObj != nullptr)
				{
					// ブロックタイプのオブジェクトのとき
					if (pObj->GetType() == CObject::TYPE::BLOCK)
					{
						// オブジェクトをブロッククラスにダウンキャスト
						CBlock* pOther = nullptr;
						pOther = utility::DownCast(pOther, pObj);

						// 必要な情報を用意
						const CX_Manager::TYPE&
							OtherType = pOther->GetModelType();	// 対象のモデルタイプを取得
						const Vec3&
							Distance = pOther->GetPos() - NewPos,	// お互いの距離の差
							OtherSize = pOther->GetSize();			// 対象のサイズ
						float
							DistanceNorm = (Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);	// 距離の差の大きさ

						// 他のブロックとの距離がある程度近ければ条件を満たさない
						if (DistanceNorm <= CField_Type::GetAreaNorm() * 0.1f)
						{
							// 重複している、このブロックを生成しない
							bSafe = false;

							break;
						}
					}

					// 次のオブジェクトへ
					pObj = pObj->GetNext();
				}
			}
#endif

			if (bSafe)
			{
				// 向きを決定
				NewRot.y = atan2f(-NewPos.x, -NewPos.z);

				// ブロックを生成し、ブロックモデルを変更する
				CBlock* pBlock = CBlock::Create(NewPos, NewRot);	// ブロックのインスタンス生成
				pBlock->BindModel(Type);							// モデルを割り当て
				pBlock->SetSize(pBlock->GetModel()->Size);			// バウンディングサイズを揃える
			}
		}
	}

	// 通常レイヤーのオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロックタグにダウンキャスト
			CBlock* pBlock = utility::DownCast<CBlock, CObject>(pObj);

			// 対象物へのベクトルを出す
			Vec3 OtherVec = pBlock->GetPos() - VEC3_INIT;

			// 対象物へのベクトルを正規化
			D3DXVec3Normalize(&OtherVec, &OtherVec);

			// プレイヤーを取得
			CPlayer* pPlayer = nullptr;
			pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

			// プレイヤーの目標座標へのベクトルを作成
			Vec3 Norm = pPlayer->GetPosTarget() - pPlayer->GetPos();

			// 生成座標計算用 ((方角 + 扇形幅の角度)の場所が生成ポイント)
			float fDirection = pPlayer->GetDirection();		// プレイヤーの現在の方角をコピー
			float fRange = 0.45f;
			Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// ブロック用の座標・向きを作成
			
			// 現在座標と目標座標に対し原点からの方向ベクトルを作成
			Vec3 OldVec = pPlayer->GetPos() - VEC3_INIT, NewVec = pPlayer->GetPosTarget() - VEC3_INIT;
			D3DXVec3Normalize(&OldVec, &OldVec);
			D3DXVec3Normalize(&NewVec, &NewVec);

			// 2本の方向ベクトルの外積を作成
			float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

			// 左に移動している場合角度を反転させる
			if (fCross < 0.0f)
				fRange = -fRange;

			// 方角に合わせて範囲分の方向ベクトルを2本作成
			Vec3 DirVec[2] = {};
			DirVec[0] = { cosf(fDirection + fRange), 0, sinf(fDirection + fRange) };
			DirVec[1] = { cosf(fDirection - fRange), 0, sinf(fDirection - fRange) };

			// 片方の扇形方向のベクトルと対象物への方向ベクトルの外積を作成
			float fCross1 = DirVec[0].x * OtherVec.z - DirVec[0].z * OtherVec.x;

			// この時点で1つ目の方向ベクトルとの外積結果が正の角度なら範囲外
			if (fCross1 > 0.0f)
			{
				continue;
			}

			// もう片方の外積も作成
			float fCross2 = DirVec[1].x * OtherVec.z - DirVec[1].z * OtherVec.x;

			// 2つ目の方向ベクトルとの外積結果が負の角度なら範囲外
			if (fCross2 < 0.0f)
			{
				continue;
			}

			// 条件を満たしていれば扇形の範囲内、このブロックは破棄
			pBlock->SetRelease();
		}

		// 次のオブジェクトを取得
		pObj = pObj->GetNext();
	}
}

//============================================================================
// 敵召喚
//============================================================================
void CBoss::SummonEnemy()
{
	if (GetNowMotion() != 2)
	{
		// ボスがブルブルモーションに変更
		SetNowMotion(2);

		// 敵を大量生成
		for (WORD wCntLoop = 0; wCntLoop < 3; ++wCntLoop)
		{
			// 初期位置
			Vec3 InitPos = VEC3_INIT;

#if 1
			do
			{
				// 扇形範囲内に収まるように方角をランダム決定
				float fRand = fabsf(utility::GetRandomValue<float>()) * 0.01f;
				InitPos = utility::DirectionConvertVec3(D3DX_PI * fRand, 10.0f + fabsf(utility::GetRandomValue<float>()) * 0.25f, CField_Manager::FIELD_RADIUS);

				// 範囲外で再抽選
			} while (!CField_Manager::GetInstance()->GetFieldBuilder()->DetectInFanRange(InitPos));
#else
			// プレイヤーの方角を取得
			CPlayer* pPlayer = nullptr;
			pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
			if (pPlayer)
			{
				float fRand = utility::GetRandomValue<float>() * 0.005f;
				InitPos = utility::DirectionConvertVec3(pPlayer->GetDirection() + fRand, 10.0f + (45.0f * fabsf(fRand)), CField_Manager::FIELD_RADIUS);
			}
#endif

			// ランダムに数値を抽選
			CBright::CREATETYPE Type = CBright::CREATETYPE::NONE;
			int nRand = rand() % 2;

			if (nRand == 0)
			{
				Type = CBright::CREATETYPE::MONSTER;
			}
			else
			{
				Type = CBright::CREATETYPE::GHOST;
			}

			// その場所に、モンスターかゴーストを生成
			CBright::Generate(InitPos, Type);
		}
	}

	// プレイヤーの方向を見続ける
	CPlayer* pPlayer = nullptr;
	pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	if (pPlayer)
	{
		Vec3 Rot = VEC3_INIT, Pos = pPlayer->GetPos();
		Rot.y = atan2f(-Pos.x, -Pos.z);
		SetRotTarget(Rot);
	}

	// カメラ距離・俯瞰度合いを強制変更
	{
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
		fDinstance += (450.0f - fDinstance) * 0.1f;
		fUpAdjust += (150.0f - fUpAdjust) * 0.05f;
		pCamera->SetDistance(fDinstance);
		pCamera->SetUpAdjust(fUpAdjust);
	}

	// 火の粉を猛発生させる
	for (WORD wCntLoop = 0; wCntLoop < 2; ++wCntLoop)
	{
		CSparks::FuryGenerate();
	}

	// エネミータイプのオブジェクトを取得
	CObject* pObjEnemy = CObject::FindSpecificObject(CObject::TYPE::ENEMY);
	CObject* pObjBright = CObject::FindSpecificObject(CObject::TYPE::BRIGHT);

	// エネミーの掃討を確認したら
	if (!pObjEnemy && !pObjBright)
	{
		// 継続期間をリセット
		m_nDuration = 0;

		// 中央待機に戻る
		m_ActionType = ACTION::HOLDCENTER;

		// 待機モーションに戻す
		SetNowMotion(1);
	}
}

//============================================================================
// ダメージ喰らい
//============================================================================
void CBoss::DamageBack()
{
	if (GetNowMotion() != 3)
	{ // ダメージモーションを再生していなければ

		// ボスがダメージモーションに変更
		SetNowMotion(3);

		// ボスのダメージ音を鳴らす
		CSound::GetInstance()->Play(CSound::LABEL::BDAMAGE);
	}
	else if (GetStopState() == true)
	{ // ダメージモーションが再生終了した時

		// 行動キャストカウントをリセット
		m_nCntActionCast = 0;

		// 継続カウントを元に戻す
		m_nDuration = 0;

		// 目標座標を中央へ戻す
		Vec3 Pos = VEC3_INIT;
		Pos.y = 150.0f;
		SetPosTarget(Pos);

		// 中央待機に戻る
		m_ActionType = ACTION::HOLDCENTER;

		// 待機モーションに戻す
		SetNowMotion(1);
	}
}

//============================================================================
// 衝突検出
//============================================================================
bool CBoss::HitCheck()
{
	// プレイヤーのパラメータをコピー
	CPlayer* pPlayer = nullptr;
	pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	const CBounding_Cylinder* Other = pPlayer->GetBndCylinder();

	// 自分とプレイヤーの円柱バウンディングで比べる
	if (collision::HitCylinderToCylinder(m_pBndCylinder, Other))
		return true;

	return false;
}

//============================================================================
// 死亡
//============================================================================
void CBoss::DeadEnd()
{
	if (GetNowMotion() != 4)
	{ // 死亡モーションを再生していなければ

		// ボスが死亡モーションに変更
		SetNowMotion(4);

		// 中央へ移動
		Vec3 Pos = VEC3_INIT;
		Pos.y = 150.0f;
		SetPosTarget(Pos);
	}
	else
	{ // 死亡モーション再生中

		// 火の粉を猛発生
		for (int nCnt = 0; nCnt < 2; ++nCnt)
			CSparks::FuryGenerate();

		// カメラ距離・俯瞰度合いを強制変更
		{
			CCamera* pCamera = CManager::GetManager()->GetCamera();
			float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
			fDinstance += (800.0f - fDinstance) * 0.1f;
			fUpAdjust += (300.0f - fUpAdjust) * 0.05f;
			pCamera->SetDistance(fDinstance);
			pCamera->SetUpAdjust(fUpAdjust);

			// カメラを振動
			pCamera->SetVibration(0.05f);
		}

		// モーションの再生が終了したらゲームシーンにゲーム終了を通知する
		if (GetStopState())
		{
			// ゲームシーン取得
			CGame* const pScene = dynamic_cast<CGame*>(CScene_Manager::GetInstance()->GetScene());

			// シーン遷移開始
			pScene->SetTransition();
		}
	}
}