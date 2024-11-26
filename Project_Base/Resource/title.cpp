//============================================================================
// 
// タイトル [title.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "motion_set.h"
#include "object_parts.h"
#include "object_X.h"

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CTitle::Update()
{
	// フレームをインクリメント
	m_nNowFrame++;

	// 基底クラスの更新処理
	CScene::Update();

	// 環境装飾の更新
	UpdateEnvironment();

	// 次のシーンへ
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		To_Next();
	}
#ifdef _DEBUG	// パラメータ確認用
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F7))
	{
		CObject::ReleaseAll();
		CScene_Manager::ChangeScene(Create());
	}
#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CTitle::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();
}

//============================================================================
// 次のシーンへ
//============================================================================
void CTitle::To_Next()
{
	// 全オブジェクトを解放
	CObject::ReleaseAll();

	// ゲームシーンへ変更
	CScene_Manager::ChangeScene(CGame::Create());
}

//============================================================================
// タイトルを生成
//============================================================================
CTitle* CTitle::Create()
{
	// タイトルを生成
	CTitle* pNew = DBG_NEW CTitle();

	if (pNew == nullptr)
	{
		assert(false && "タイトルの生成に失敗");
	}

	// タイトルの初期設定
	if (FAILED(pNew->Init()))
	{
		assert(false && "タイトルの初期設定に失敗");
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
CTitle::CTitle() :
	CScene{},
	m_pButterfly{ nullptr },
	m_Path{},
	m_nNowFrame{ 0 },
	m_nMaxFrame{ 0 },
	m_pTree{ nullptr },
	m_pHole{ nullptr }
{
	m_Path.clear();
}

//============================================================================
// デストラクタ
//============================================================================
CTitle::~CTitle()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CTitle::Init()
{
	// 環境装飾を生成
	{
		// 蝶
		m_pButterfly = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\butterfly_motion.json"));
		auto ButterflyParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\butterfly.json");
		m_pButterfly->SetRot(utility::JsonConvertToVec3(ButterflyParam["Rot"]));
		m_pButterfly->SetPos(utility::JsonConvertToVec3(ButterflyParam["Pos"]));
		m_pButterfly->SetNowMotion(1);
		m_Path.push_back(utility::JsonConvertToVec3(ButterflyParam["Path"]));
		m_nMaxFrame = ButterflyParam["MaxFrame"];

		// カメラ情報を変更
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetDistance(50.0f);
		pCamera->SetUpAdjust(0.0f);
		pCamera->SetPos(utility::JsonConvertToVec3(ButterflyParam["Pos"]));
		pCamera->SetPosTarget(utility::JsonConvertToVec3(ButterflyParam["Pos"]));

		// 樹
		m_pTree = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\tree_motion.json"));
		auto TreeParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\tree.json");
		m_pTree->SetRot(utility::JsonConvertToVec3(TreeParam["Rot"]));
		m_pTree->SetPos(utility::JsonConvertToVec3(TreeParam["Pos"]));

		// 穴
		//m_pHole = CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\hole.json"));
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTitle::Uninit()
{

}

//============================================================================
// 環境装飾の更新
//============================================================================
void CTitle::UpdateEnvironment()
{
	// 蝶の更新
	UpdateButterfly();

	if (m_nNowFrame >= m_nMaxFrame)
	{
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetPosTarget({ 100.0f, -50.0f, 0.0f });
		float fDistance = pCamera->GetDistance();

		if (fDistance < 300.0f)
		{
			pCamera->SetDistance(fDistance += 5.0f);
		}
	}
}

//============================================================================
// 蝶の更新
//============================================================================
void CTitle::UpdateButterfly()
{
	if (m_nNowFrame < m_nMaxFrame)
	{
		// 目標地点へ移動
		D3DXVECTOR3 NewPos = m_pButterfly->GetPos();
		int nFrameCoef = m_nMaxFrame - m_nNowFrame;
		NewPos += (m_Path[0] - NewPos) / nFrameCoef;
		m_pButterfly->SetPos(NewPos);

		// カメラ追従
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetPos(NewPos);
		pCamera->SetPosTarget(NewPos);
	}
	else if (m_nNowFrame == m_nMaxFrame)
	{
		m_pButterfly->SetNowMotion(0);
	}
}