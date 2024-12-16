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
#include "manager.h"
#include "title.h"
#include "object_HUD.h"
#include "motion_set.h"

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

	// スプラインテストの更新処理
	//m_pSpline->Update();

#if 0
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

#if 0
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Text"))
	{
		D3DXVECTOR3 Pos = m_pText->GetPos();
		ImGui::DragFloat("Pos:x", &Pos.x);
		ImGui::DragFloat("Pos:y", &Pos.y);
		ImGui::DragFloat("Pos:z", &Pos.z);
		m_pText->SetPos(Pos);
		m_pText->SetPosTarget(Pos);

		D3DXVECTOR3 Rot = m_pText->GetRot();
		ImGui::DragFloat("Rot:x", &Rot.x);
		ImGui::DragFloat("Rot:y", &Rot.y);
		ImGui::DragFloat("Rot:z", &Rot.z);
		m_pText->SetRot(Rot);
		m_pText->SetRotTarget(Rot);

		D3DXVECTOR3 Size = m_pText->GetSize();
		ImGui::DragFloat("Size:x", &Size.x);
		ImGui::DragFloat("Size:y", &Size.y);
		ImGui::DragFloat("Size:z", &Size.z);
		m_pText->SetSize(Size);
		m_pText->SetSizeTarget(Size);

		ImGui::End();
	}
#endif

	// フェイクプレイヤーの更新
	UpdateFakePlayer();

	// 次のシーンへ
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
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

	// スプラインの描画処理
	//m_pSpline->Draw();
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
	m_pText{ nullptr }
	//m_pSpline{ nullptr }
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
	pCamera->SetRot({ 0.0f, 0.0f, 0.0f });
	pCamera->SetRotTarget({ 0.0f, 0.0f, 0.0f });
	pCamera->SetPos({ 0.0f, 0.0f, 0.0f });
	pCamera->SetPosTarget({ 0.0f, 0.0f, 0.0f });
	pCamera->SetDistance(50.0f);
	pCamera->SetUpAdjust(0.0f);

	// フェイクプレイヤーの生成
	m_pFakePlayer = CMotion_Set::Create(CObject::LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\player_motion.json"));
	auto FakePlayerParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\fakeplayer.json");
	m_pFakePlayer->SetNowMotion(1);
	m_pFakePlayer->SetRot(utility::JsonConvertToVec3(FakePlayerParam["Rot"]));
	m_Path = utility::JsonConvertToVec3(FakePlayerParam["Pos"]);
	m_pFakePlayer->SetPos({ 0.0f, -1000.0f, 0.0f });

	// テキストの生成
	m_pText = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\result.json"));
	m_pText->BindTex(CTexture_Manager::TYPE::RESULT);

	/* スプラインテストの初期設定 */
	//m_pSpline = DBG_NEW CSpline_Test();
	//m_pSpline->Init();

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

	///* スプラインテストの終了処理 */
	//if (m_pSpline != nullptr)
	//{
	//	m_pSpline->Uninit();
	//	delete m_pSpline;
	//	m_pSpline = nullptr;
	//}
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