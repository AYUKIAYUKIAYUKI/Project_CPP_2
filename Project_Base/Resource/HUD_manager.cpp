//============================================================================
// 
// HUDマネージャー [HUD_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "HUD_manager.h"
#include "object_HUD.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CHUD_Manager* CHUD_Manager::m_pHUDManager = nullptr;	// HUDマネージャーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CHUD_Manager::Update()
{
	// HUDオブジェクトに異常があれば更新を行わない
	if (DetectError())
		return;

	// ゲームシーンで無ければ更新を行わない
	if (typeid(*CScene_Manager::GetInstance()->GetScene()) != typeid(CGame))
		return;

	// プレイヤーの情報が無ければ更新を行わない
	if (!m_pSyncPlayer)
		return;

	// プレイヤーの体力量をコピー
	WORD wSyncPlayerLife = static_cast<WORD>(m_pSyncPlayer->GetLife());

	// 体力量分が描画されるように座標を調整
	for (WORD wCntLife = CPlayer::MAX_LIFE; wCntLife > 0; --wCntLife)
	{
		// 目標座標をコピー
		Vec3 NewPosTarget = m_pPlayerLife[wCntLife - 1]->GetPosTarget();

		// プレイヤーの体力に一致する量でなければ画面外へ移動
		if (wCntLife > wSyncPlayerLife)
			NewPosTarget.y = -100.0f;
		else
			NewPosTarget.y = 75.0f;

		// 座標を反映
		m_pPlayerLife[wCntLife - 1]->SetPosTarget(NewPosTarget);
	}

#if 0	// HUD挙動の確認
	if (CManager::GetKeyboard()->GetPress(DIK_Z))
		m_pPlayerGaugeWindow->SetVibration();
	else if (CManager::GetKeyboard()->GetTrigger(DIK_X))
		m_pPlayerGaugeWindow->SetWaving();
#endif
}

//============================================================================
// 描画処理
//============================================================================
void CHUD_Manager::Draw()
{
	/* 現在は無し */
}

//============================================================================
// プレイヤーをセット
//============================================================================
void CHUD_Manager::SetSyncPlayer(const CPlayer* const pPlayer)
{
	m_pSyncPlayer = pPlayer;
}

//============================================================================
// 生成
//============================================================================
HRESULT CHUD_Manager::Create()
{
	// 既に生成されていたら
	if (m_pHUDManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("HUDマネージャーは既に生成されています", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// インスタンスを生成
	m_pHUDManager = DBG_NEW CHUD_Manager();

	// 生成失敗
	if (m_pHUDManager == nullptr)
	{
		return E_FAIL;
	}

	// HUDマネージャーの初期設定
	if (FAILED(m_pHUDManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CHUD_Manager::Release()
{
	if (m_pHUDManager != nullptr)
	{
		// 終了処理
		m_pHUDManager->Uninit();

		// メモリを解放
		delete m_pHUDManager;

		// ポインタを初期化
		m_pHUDManager = nullptr;
	}
}

//============================================================================
// HUDマネージャーを取得
//============================================================================
CHUD_Manager* CHUD_Manager::GetInstance()
{
	// 本体が存在しなければ
	if (m_pHUDManager == nullptr)
	{
		// 生成
		if (FAILED(Create()))
		{
			assert(false && "HUDマネージャーの取得に失敗");
		}
	}

	return m_pHUDManager;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CHUD_Manager::CHUD_Manager() :
	m_pSyncPlayer{ nullptr }, 
	m_pMap{ nullptr },
	m_pPlayerGauge{ nullptr },
	m_pPlayerGaugeWindow{ nullptr }
{
	// プレイヤーの体力表示を初期化
	for (WORD wCnt = 0; wCnt < CPlayer::MAX_LIFE; ++wCnt)
	{
		m_pPlayerLife[wCnt] = nullptr;
	}
}

//============================================================================
// デストラクタ
//============================================================================
CHUD_Manager::~CHUD_Manager()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CHUD_Manager::Init()
{
	{ // マップ表示を作成
		m_pMap = CObject_HUD::Create("Data\\JSON\\HUD\\map.json");
		m_pMap->BindTex(CTexture_Manager::TYPE::MAP);
	}

	{ // プレイヤーのゲージを生成
		m_pPlayerGauge = CObject_HUD::Create("Data\\JSON\\HUD\\playergauge.json");
		m_pPlayerGauge->BindTex(CTexture_Manager::TYPE::PLAYERGAUGE);
	}

	{ // プレイヤーのゲージウィンドウを生成
		m_pPlayerGaugeWindow = CObject_HUD::Create("Data\\JSON\\HUD\\playergaugewindow.json");
		m_pPlayerGaugeWindow->BindTex(CTexture_Manager::TYPE::PLAYERGAUGEWINDOW);
	}

	// プレイヤーの体力表示を生成
	for (WORD wCntLife = 0; wCntLife < CPlayer::MAX_LIFE; ++wCntLife)
	{
		std::string FilePath = "Data\\JSON\\HUD\\playerlife\\" + to_string(wCntLife) + ".json";
		m_pPlayerLife[wCntLife] = CObject_HUD::Create(FilePath);
		m_pPlayerLife[wCntLife]->BindTex(CTexture_Manager::TYPE::CIRCLE);
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CHUD_Manager::Uninit()
{

}

//============================================================================
// エラー検出
//============================================================================
bool CHUD_Manager::DetectError()
{
	// エラー検出用
	bool bError = 0;

	if (!m_pMap)
	{
		CRenderer::GetRenderer()->SetDebugString("マップ表示が出来ません");
		bError = 1;
	}

	if (!m_pPlayerGauge)
	{
		CRenderer::GetRenderer()->SetDebugString("プレイヤーのゲージ表示が出来ません");
		bError = 1;
	}

	if (!m_pPlayerGaugeWindow)
	{
		CRenderer::GetRenderer()->SetDebugString("プレイヤーのゲージ枠表示が出来ません");
		bError = 1;
	}

	if (m_pPlayerLife == nullptr)
	{
		CRenderer::GetRenderer()->SetDebugString("プレイヤーの体力表示が出来ません");
		bError = 1;
	}

	// エラー検出状況を返す
	return bError;
}