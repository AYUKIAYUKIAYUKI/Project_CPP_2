//============================================================================
// 
// ゲーム [game.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "game.h"
#include "result.h"
#include "manager.h"
#include "field_manager.h"
#include "HUD_manager.h"
#include "object_HUD.h"
#include "player.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"
#include "bright.h"

#include "title.h"
#include "object_PopUp.h"
#include "object_TextMesh.h"

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CGame::Update()
{
	// シーンクラスの更新処理
	CScene::Update();

	// 真実の蝶をプレイヤーに同期させる
	D3DXVECTOR3 Pos = m_pPlayerPtr->GetPos();			// 座標をコピーし
	Pos.y += utility::GetRandomValue<float>() * 0.01f;	// 少し上下にゆらす
	m_pTrueButterfly->SetPos(Pos);
	m_pTrueButterfly->SetRot(m_pPlayerPtr->GetRot());

	// フィールドマネージャーの更新
	CField_Manager::GetInstance()->Update();

	// HUDマネージャーの更新
	CHUD_Manager::GetInstance()->Update();

	// シーンの遷移フラグが立っていたら
	if (m_bTransition)
	{
		// 黒いフェードを表示させる
		if (m_pRenderFade->GetAlpha() >= 1.0f)
		{ // 完全に真っ黒で

			// 次のシーンへ遷移
			To_Next();
		}
		else
		{ // 真っ黒になってない

			// ポリゴンが画面を覆う場所に移動させる
			m_pRenderFade->SetPos({ 0.0f, 0.0f, 0.0f });
			m_pRenderFade->SetPosTarget({ 0.0f, 0.0f, 0.0f });

			// 黒いフェードの透明度を下げていく
			D3DXCOLOR Col = m_pRenderFade->GetCol();
			Col.a += 0.01f;
			m_pRenderFade->SetCol(Col);

			// 黒いフェードの目標透明度を設定
			D3DXCOLOR ColTarget = m_pRenderFade->GetColTarget();
			ColTarget.a = 1.0f;
			m_pRenderFade->SetColTarget(ColTarget);
		}
	}

#ifdef _DEBUG	// パラメータ確認用
	// リザルト画面へ
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		To_Next();
	}
#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CGame::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();

	// フィールドマネージャーの描画
	CField_Manager::GetInstance()->Draw();
}

//============================================================================
// 遷移開始
//============================================================================
void CGame::SetTransition()
{
	m_bTransition = true;
}

//============================================================================
// 次のシーンへ
//============================================================================
void CGame::To_Next()
{
	// 全オブジェクトを解放
	CObject::ReleaseAll();

	// リザルトシーンへ変更
	CScene_Manager::ChangeScene(CResult::Create());
}

//============================================================================
// ゲームを生成
//============================================================================
CGame* CGame::Create()
{
	// ゲームを生成
	CGame* pNew = DBG_NEW CGame();

	if (pNew == nullptr)
	{
		assert(false && "ゲームの生成に失敗");
	}

	// ゲームの初期設定
	if (FAILED(pNew->Init()))
	{
		assert(false && "ゲームの初期設定に失敗");
	}

	return pNew;
}

//============================================================================
// 
// protectedメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CGame::CGame() :
	CScene{},
	m_bTransition{ false },
	m_pRenderFade{ nullptr },
	m_pPlayerPtr{ nullptr },
	m_pTrueButterfly{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CGame::~CGame()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CGame::Init()
{
	// プレイヤーを生成
	CPlayer* pPlayer = CPlayer::Create();
	pPlayer->SetPos({ -100.0f, 0.0f, 0.0f });	/* ダメージ回避用 */
	m_pPlayerPtr = pPlayer;

	// 真実の蝶を生成
	m_pTrueButterfly = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::LATEDRAW, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\BUTTERFLY\\butterfly_motion.json"));
	m_pTrueButterfly->SetNowMotion(1);

	// フィールドマネージャーにプレイヤーをセット
	CField_Manager::GetInstance()->SetSyncPlayer(m_pPlayerPtr);

	// HUDマネージャーにプレイヤーをセット
	CHUD_Manager::GetInstance()->SetSyncPlayer(m_pPlayerPtr);

	// カメラ情報を変更
	CCamera* pCamera = CManager::GetManager()->GetCamera();
	pCamera->ChangeTrackPlayer(true);

	// フェード表示を生成
	m_pRenderFade = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\black.json"));

	// エネミーを生成
#if 1
	CEnemy* pEnemy = nullptr;

	// モンスター
	pEnemy = CMonster::Create();
	pEnemy->SetDirectionTarget(D3DX_PI * -0.4f);

	// ゴースト
	pEnemy = CGhost::Create();
	D3DXVECTOR3 PosTarget = {
		cosf(-(D3DX_PI * -0.5f)) * 300.0f,
		80.0f,
		sinf((D3DX_PI * -0.5f)) * 300.0f,
	};
	pEnemy->SetPos(PosTarget);
	pEnemy->SetPosTarget(PosTarget);

	// 閃光を生成
	CBright::Generate(pEnemy->GetPos());
	CBright::Generate(pEnemy->GetPosTarget());

	// フライヤー
	pEnemy = CFlyer::Create();
	pEnemy->SetPosTarget({ 0.0f, 50.0f, 0.0f });
	pEnemy->SetDirectionTarget(D3DX_PI * -0.6f);
#endif

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CGame::Uninit()
{
	// HUDマネージャーの破棄
	CHUD_Manager::Release();

	// フィールドマネージャーの破棄
	CField_Manager::Release();
}