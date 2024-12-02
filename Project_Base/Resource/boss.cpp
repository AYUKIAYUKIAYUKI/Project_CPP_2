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
#include "field_manager.h"
#include "object_parts.h"
#include "collision.h"
#include "player.h"
#include "sparks.h"
#include "scene.h"
#include "game.h"

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
	CCharacter{},
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
	}
	ImGui::End();
#endif	// _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CBoss::Draw()
{
	// キャラクタークラスの描画処理
	CCharacter::Draw();
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
	pNewInstance->SetMotion(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\pumpkin_motion.json"));

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

		// ランダムに行動をセット
		switch (1)
		{
		case 1:
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
	}

	// 継続期間をインクリメント
	++m_nDuration;

	// カメラ距離・俯瞰度合いを強制変更
	{
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
		fDinstance += (450.0f - fDinstance) * 0.1f;
		fUpAdjust += (150.0f - fUpAdjust) * 0.05f;
		pCamera->SetDistance(fDinstance);
		pCamera->SetUpAdjust(fUpAdjust);
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
	}

	// 3秒時点で
	if (m_nDuration == 180)
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
		// プレイヤーに1ダメージ
		CPlayer* pPlayer = nullptr;
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
		pPlayer->SetDamage(-1);
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