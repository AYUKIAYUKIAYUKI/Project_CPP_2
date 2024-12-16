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
	m_pRenderFade{ nullptr }
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
	CField_Manager::GetInstance()->SetSyncPlayer(pPlayer);

	// HUDマネージャーにプレイヤーをセット
	CHUD_Manager::GetInstance()->SetSyncPlayer(pPlayer);

	// カメラ情報を変更
	CCamera* pCamera = CManager::GetManager()->GetCamera();
	pCamera->ChangeTrackPlayer(true);

	// フェード表示を生成
	m_pRenderFade = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\black.json"));

	/* ポップアップを表示 */
	JSON Json = utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_0.json");
	CObject_PopUp* pPopUp = CObject_PopUp::Create(Json);

	/* テスト1 */
	Json = utility::OpenJsonFile("Data\\JSON\\debug_param.json");
	CObject_TextMesh* pTest1 = CObject_TextMesh::Create();
	pTest1->SetText(utility::JsonConvertToSJIS(Json["Text"][0]));
	pTest1->SetMeshSize(utility::JsonConvertToVec3(Json["MeshSize"]));
	pTest1->SetPos({ 0.0f, 10.0f, -CField_Manager::FIELD_RADIUS });

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