//============================================================================
// 
// リザルト [result.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "result.h"
#include "motion_set.h"
#include "object_PopUp.h"

#include "manager.h"
#include "title.h"

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CResult::Update()
{
	// 基底クラスの更新処理
	CScene::Update();

	// フェイクプレイヤーの調整
#if 0
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Playerfake"))
	{
		D3DXVECTOR3 Pos = m_pFakePlayer->GetPos();
		ImGui::DragFloat("Pos:x", &Pos.x);
		ImGui::DragFloat("Pos:y", &Pos.y);
		ImGui::DragFloat("Pos:z", &Pos.z);
		m_pFakePlayer->SetPos(Pos);

		D3DXVECTOR3 Rot = m_pFakePlayer->GetRot();
		ImGui::DragFloat("Rot:x", &Rot.x);
		ImGui::DragFloat("Rot:y", &Rot.y);
		ImGui::DragFloat("Rot:z", &Rot.z);
		m_pFakePlayer->SetRot(Rot);

		ImGui::End();
	}
#endif

	// ポップアップウィンドウの調整
#if 0
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("PopUpWindow"))
	{
		// サイズ
		D3DXVECTOR3 Size = m_pPopUpWindow->GetSize();
		ImGui::DragFloat("Size:x", &Size.x);
		ImGui::DragFloat("Size:y", &Size.y);
		ImGui::DragFloat("Size:z", &Size.z);
		m_pPopUpWindow->SetSize(Size);
	
		// 座標
		D3DXVECTOR3 Pos = m_pPopUpWindow->GetPos();
		ImGui::DragFloat("Pos:x", &Pos.x);
		ImGui::DragFloat("Pos:y", &Pos.y);
		ImGui::DragFloat("Pos:z", &Pos.z);
		m_pPopUpWindow->SetPos(Pos);

		ImGui::End();
	}
#endif

	// フェイクプレイヤーの更新
	UpdateFakePlayer();

	// コントローラーを取得
	CInputPad* pPad = CManager::GetPad();

	// 次のシーンへ
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputPad::JOYKEY::A) ||
		pPad->GetTrigger(CInputPad::JOYKEY::START))
	{
		To_Next();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CResult::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();
}

//============================================================================
// 次のシーンへ
//============================================================================
void CResult::To_Next()
{
	// 全オブジェクトを解放
	CObject::ReleaseAll();

	// タイトルシーンへ変更
	CScene_Manager::ChangeScene(CTitle::Create());
}

//============================================================================
// リザルトを生成
//============================================================================
CResult* CResult::Create()
{
	// リザルトを生成
	CResult* pNew = DBG_NEW CResult();

	if (pNew == nullptr)
	{
		assert(false && "リザルトの生成に失敗");
	}

	// リザルトの初期設定
	if (FAILED(pNew->Init()))
	{
		assert(false && "リザルトの初期設定に失敗");
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
CResult::CResult() :
	m_Path{ 0.0f, 0.0f, 0.0f },
	m_pFakePlayer{ nullptr },
	m_pPopUpWindow{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CResult::~CResult()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CResult::Init()
{
	// カメラのパラメータを設定
	CCamera* pCamera = CManager::GetManager()->GetCamera();
	pCamera->ChangeTrackPlayer(false);
	pCamera->SetRot({ 0.0f, 0.0f, 0.0f });
	pCamera->SetRotTarget({ 0.0f, 0.0f, 0.0f });
	pCamera->SetPos({ 0.0f, 0.0f, 0.0f });
	pCamera->SetPosTarget({ 0.0f, 0.0f, 0.0f });
	pCamera->SetDistance(50.0f);
	pCamera->SetUpAdjust(0.0f);

	// フェイクプレイヤーの生成
	m_pFakePlayer = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\player_motion.json"));
	auto FakePlayerParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\fakeplayer.json");
	m_pFakePlayer->SetNowMotion(1);
	m_pFakePlayer->SetRot(utility::JsonConvertToVec3(FakePlayerParam["Rot"]));
	m_Path = utility::JsonConvertToVec3(FakePlayerParam["Pos"]);
	m_pFakePlayer->SetPos({ 0.0f, -1024.0f, 0.0f });

	// ポップアップウィンドウの生成
	m_pPopUpWindow = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\resultwindow.json"));

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CResult::Uninit()
{
	// フェイクプレイヤーの破棄
	if (m_pFakePlayer != nullptr)
	{
		m_pFakePlayer->SetRelease();
		m_pFakePlayer = nullptr;
	}

	// ポップアップウィンドウの破棄
	if (m_pPopUpWindow != nullptr)
	{
		m_pPopUpWindow->SetRelease();
		m_pPopUpWindow = nullptr;
	}
}

//============================================================================
// フェイクプレイヤーの更新処理
//============================================================================
void CResult::UpdateFakePlayer()
{
	D3DXVECTOR3 NewPos = m_pFakePlayer->GetPos();
	NewPos += (m_Path - NewPos) * 0.1f;
	m_pFakePlayer->SetPos(NewPos);
}