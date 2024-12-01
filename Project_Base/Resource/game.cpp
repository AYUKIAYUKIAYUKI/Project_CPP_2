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

#include "title.h"

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

	// HUDマネージャーの更新
	CHUD_Manager::GetInstance()->Update();

	// ボス登場カウントをインクリメント
	++DBG_nCntAppearBoss;

	// とりあえず30フレーム経過で
	if (DBG_nCntAppearBoss == 30)
	{
		// ボス登場
		CField_Manager::GetInstance()->AppearBoss();
	}

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
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F7))
	{
		CObject::ReleaseAll();
		CScene_Manager::ChangeScene(CTitle::Create());
		//CField_Manager::Release();
		//this->Uninit();
		//CField_Manager::Create();
		//CField_Manager::GetInstance()->InitForTitle();
		//this->Init();
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
	m_bTransition{ false },
	m_pRenderFade{ nullptr },
	CScene{},
	DBG_nCntAppearBoss{ 0 }
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

	// フィールドマネージャーにプレイヤーをセット
	CField_Manager::GetInstance()->SetPlayer(pPlayer);

	// HUDマネージャーにプレイヤーをセット
	CHUD_Manager::GetInstance()->SetSyncPlayer(pPlayer);

	// フェード表示を生成
	m_pRenderFade = CObject_HUD::Create("Data\\JSON\\HUD\\black.json");

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CGame::Uninit()
{
	// HUDマネージャーの破棄
	CHUD_Manager::Release();
}