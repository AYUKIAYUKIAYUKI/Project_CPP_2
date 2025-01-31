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
#include "field_manager.h"
#include "field_builder.h"
#include "object_HUD.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"
#include "boss.h"
#include "item.h"

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

#ifdef _DEBUG
	// 調整
	ParamControl();
#endif // _DEBUG

	// プレイヤーマップシンボルの更新
	UpdateMapSymbolPlayer();

	// アイテムマップシンボルの更新
	UpdateMapSymbolItem();

	// プレイヤーライフの更新
	UpdatePlayerLife();

	// ボスゲージ背景の更新
	UpdateBossGaugeBack();

	// ボスゲージバーの更新
	UpdateBossGaugeBar();
}

//============================================================================
// 描画処理
//============================================================================
void CHUD_Manager::Draw()
{
	/* 現在は無し */
}

//============================================================================
// マップシンボルの特殊挙動
//============================================================================
void CHUD_Manager::SpecialMapSymbol(float fMove)
{
	// プレイヤーの方角を疑似作成
	float fAngle = -D3DX_PI * 0.5f, fCoef = 56.0f;

	// 目標座標をコピー
	Vec3 AdjustPos = m_pMapSymbolPlayer->GetPosTarget();

	// プレイヤーの方角に合わせてシンボルの座標も回転させる
	AdjustPos += { cosf(fAngle)* fCoef, -sinf(fAngle) * fCoef - fMove, 0.0f };
	m_pMapSymbolPlayer->SetPos(AdjustPos);
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
	m_pMapBase{ nullptr },
	m_pMapRing{ nullptr },
	m_pMapSymbolPlayer{ nullptr },
	m_pMapSymbolItem{ nullptr },
	m_pPlayerGauge{ nullptr },
	m_pPlayerGaugeWindow{ nullptr },
	m_pBossGaugeBack{ nullptr },
	m_pBossGaugeBar{ nullptr },
	m_BossGaugeBarParam{},
	m_bSwitchBar{ false },
	m_pBossGaugeBase{ nullptr }
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
	{ // マップ枠を作成
		m_pMapBase = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\mapbase.json"));
		m_pMapBase->BindTex(CTexture_Manager::TYPE::MAPBASE);
	}

	{ // マップ輪を作成
		m_pMapRing = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\mapring.json"));
		m_pMapRing->BindTex(CTexture_Manager::TYPE::MAPRING);
	}

	{ // プレイヤーマップシンボルを作成
		m_pMapSymbolPlayer = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\mapsymbolplayer.json"));
		m_pMapSymbolPlayer->BindTex(CTexture_Manager::TYPE::MAPSYMBOLPLA);
	}

	{ // アイテムマップシンボルを作成
		m_pMapSymbolItem = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\mapsymbolitem.json"));
		m_pMapSymbolItem->BindTex(CTexture_Manager::TYPE::MAPSYMBOLITE);
	}

	{ // プレイヤーゲージバーを生成
		m_pPlayerGauge = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\playergauge.json"));
		m_pPlayerGauge->BindTex(CTexture_Manager::TYPE::PLAGAUBAR);
	}

	{ // プレイヤーゲージ枠を生成
		m_pPlayerGaugeWindow = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\playergaugewindow.json"));
		m_pPlayerGaugeWindow->BindTex(CTexture_Manager::TYPE::PLAGAUBASE);
	}

	// プレイヤーの体力表示を生成
	for (WORD wCntLife = 0; wCntLife < CPlayer::MAX_LIFE; ++wCntLife)
	{
		std::string FilePath = "Data\\JSON\\HUD\\playerlife\\" + to_string(wCntLife) + ".json";
		m_pPlayerLife[wCntLife] = CObject_HUD::Create(utility::OpenJsonFile(FilePath));
		m_pPlayerLife[wCntLife]->BindTex(CTexture_Manager::TYPE::PLALIFE);
	}

	{ // ボスゲージ背景を生成
		m_pBossGaugeBack = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\bossgaugeback.json"));
		m_pBossGaugeBack->BindTex(CTexture_Manager::TYPE::BOSSGAUBACK);
	}

	{ // ボスゲージバーを生成
		m_pBossGaugeBar = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\bossgaugebar.json"));
		m_pBossGaugeBar->BindTex(CTexture_Manager::TYPE::BOSSGAUBAR);

		// X方向目標サイズを0にしておく
		Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget();
		SizeTarget.x = 0.0f;
		m_pBossGaugeBar->SetSizeTarget(SizeTarget);

		// ボスゲージバー用のパラメータをコピーしておく
		m_BossGaugeBarParam = utility::OpenJsonFile("Data\\JSON\\HUD\\bossgaugebar.json");
	}

	{ // ボスゲージ枠を生成
		m_pBossGaugeBase = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\bossgaugebase.json"));
		m_pBossGaugeBase->BindTex(CTexture_Manager::TYPE::BOSSGAUBASE);
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CHUD_Manager::Uninit()
{
	/* 現在は無し */
}

//============================================================================
// 調整
//============================================================================
void CHUD_Manager::ParamControl()
{
#if 0 // 座標確認
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Pos Edit")) {
		Vec3 Pos = m_pMapSymbol->GetPos();
		ImGui::DragFloat("Pos:X", &Pos.x);
		ImGui::DragFloat("Pos:Y", &Pos.y);
		ImGui::DragFloat("Pos:Z", &Pos.z);
		m_pMapSymbol->SetPos(Pos);
		ImGui::End();
	}
#endif

#if 1 // カラー確認
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Color Edit")) {
		XCol ColTarget = m_pMapSymbolItem->GetColTarget();
		ImGui::ColorEdit4("ColTarget", &ColTarget.r);
		m_pMapSymbolItem->SetColTarget(ColTarget);
		ImGui::End();
	}
#endif

#if 0 // HUD挙動の確認
	if (CManager::GetKeyboard()->GetPress(DIK_Z))
		m_pPlayerGaugeWindow->SetVibration();
	else if (CManager::GetKeyboard()->GetTrigger(DIK_X))
		m_pPlayerGaugeWindow->SetWaving();
#endif
}

//============================================================================
// プレイヤーマップシンボルの更新
//============================================================================
void CHUD_Manager::UpdateMapSymbolPlayer()
{
	// プレイヤーの方角をコピー
	float fAngle = m_pSyncPlayer->GetDirection(), fCoef = 56.0f;

	// プレイヤーシンボルの目標座標をコピー
	Vec3 AdjustPos = m_pMapSymbolPlayer->GetPosTarget();

	// プレイヤーの方角に合わせてプレイヤーシンボルの座標も回転させる
	AdjustPos += { cosf(fAngle)* fCoef, -sinf(fAngle) * fCoef, 0.0f };
	m_pMapSymbolPlayer->SetPos(AdjustPos);
}

//============================================================================
// アイテムマップシンボルの更新
//============================================================================
void CHUD_Manager::UpdateMapSymbolItem()
{
	// アイテムタイプのオブジェクトを探す
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

	// 上記のオブジェクトの有無に応じて処理分岐
	if (pObj)
	{ // アイテムが存在している

		// オブジェクトをアイテムクラスにキャスト
		CItem* pItem = utility::DownCast<CItem, CObject>(pObj);

		// アイテムの方角をコピー
		float fAngle = pItem->GetDirection(), fCoef = 56.0f;

		// アイテムシンボルの目標座標をコピー
		Vec3 AdjustPos = m_pMapSymbolItem->GetPosTarget();

		// アイテムの方角に合わせてアイテムシンボルの座標も回転させる
		AdjustPos += { cosf(fAngle)* fCoef, -sinf(fAngle) * fCoef, 0.0f };
		m_pMapSymbolItem->SetPos(AdjustPos);

		// アイテムシンボルを表示
		XCol NewColTarget = m_pMapSymbolItem->GetColTarget();
		NewColTarget.a = 0.75f;
		m_pMapSymbolItem->SetColTarget(NewColTarget);
	}
	else
	{ // アイテムが存在しない
		
		// アイテムシンボルを透明に
		XCol NewColTarget = m_pMapSymbolItem->GetColTarget();
		NewColTarget.a = 0.0f;
		m_pMapSymbolItem->SetColTarget(NewColTarget);
	}
}

//============================================================================
// プレイヤーライフの更新
//============================================================================
void CHUD_Manager::UpdatePlayerLife()
{
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
			NewPosTarget.y = 100.0f;

		// 座標を反映
		m_pPlayerLife[wCntLife - 1]->SetPosTarget(NewPosTarget);
	}
}

//============================================================================
// ボスゲージ背景の更新
//============================================================================
void CHUD_Manager::UpdateBossGaugeBack()
{
	/* 現在は無し */
}

//============================================================================
// ボスゲージバーの更新
//============================================================================
void CHUD_Manager::UpdateBossGaugeBar()
{
	// ボスタイプのオブジェクトを取得
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::BOSS);

	// ボス登場でゲージの役割を変更
	if (!m_bSwitchBar && pObj)
	{
		m_bSwitchBar = true;

		// バーの色を赤く染める
		m_pBossGaugeBar->SetColTarget({ 0.75f, 0.0, 0.0f, 0.75f });
	}

	// 役割変更後のゲージバーの更新
	if (m_bSwitchBar)
	{
		// ボスのライフ数をコピー
		CBoss* pBoss = utility::DownCast<CBoss, CObject>(pObj);
		int nLife = pBoss->GetLife();

		{ // 目標サイズを設定

			// 基礎サイズを最大カウント数で割る→1カウント当たりのサイズを出す→現在のカウント数分のサイズに設定
			Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget();
			SizeTarget.x = nLife * (utility::JsonConvertToVec3(m_BossGaugeBarParam["SizeTarget"]).x / CBoss::MAX_LIFE);
			m_pBossGaugeBar->SetSizeTarget(SizeTarget);
		}

		{ // 目標座標を設定

			// ( 基礎座標 - 基礎サイズ )でポリゴンの左端となるの座標を出す
			float BasePosX = utility::JsonConvertToVec3(m_BossGaugeBarParam["PosTarget"]).x - utility::JsonConvertToVec3(m_BossGaugeBarParam["SizeTarget"]).x;

			// 横幅を足すことで、ゲージ延長→右にずらす→ゲージと枠の左端が揃う
			Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget(), PosTarget = m_pBossGaugeBar->GetPosTarget();
			PosTarget.x = BasePosX + SizeTarget.x;
			m_pBossGaugeBar->SetPosTarget(PosTarget);
		}

		{ // テクスチャサイズを設定

			// 実値を直接変更するので、目標値への補間はここで行う
			Vec2 TexSize = m_pBossGaugeBar->GetTexSize();
			TexSize.x += (1.0f - (nLife * (1.0f / CBoss::MAX_LIFE)) - TexSize.x) * static_cast<float>(m_BossGaugeBarParam["CorrectionCoef"]);
			m_pBossGaugeBar->SetTexSize(TexSize);
		}

		return;
	}

	// 破壊したブロック数をコピー
	float fCntDestroyBlock = static_cast<float>(CField_Manager::GetInstance()->GetFieldBuilder()->GetCntDestroyBlock());

	{ // 目標サイズを設定

		// 基礎サイズを最大カウント数で割る→1カウント当たりのサイズを出す→現在のカウント数分のサイズに設定
		Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget();
		SizeTarget.x = fCntDestroyBlock * (utility::JsonConvertToVec3(m_BossGaugeBarParam["SizeTarget"]).x / CField_Manager::MAX_DESTROY_BLOCK);
		m_pBossGaugeBar->SetSizeTarget(SizeTarget);
	}

	{ // 目標座標を設定

		// ( 基礎座標 - 基礎サイズ )でポリゴンの左端となるの座標を出す
		float BasePosX = utility::JsonConvertToVec3(m_BossGaugeBarParam["PosTarget"]).x - utility::JsonConvertToVec3(m_BossGaugeBarParam["SizeTarget"]).x;

		// 横幅を足すことで、ゲージ延長→右にずらす→ゲージと枠の左端が揃う
		Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget(), PosTarget = m_pBossGaugeBar->GetPosTarget();
		PosTarget.x = BasePosX + SizeTarget.x;
		m_pBossGaugeBar->SetPosTarget(PosTarget);
	}

	{ // テクスチャサイズを設定

		// 実値を直接変更するので、目標値への補間はここで行う
		Vec2 TexSize = m_pBossGaugeBar->GetTexSize();
		TexSize.x += (1.0f - (fCntDestroyBlock * (1.0f / CField_Manager::MAX_DESTROY_BLOCK)) - TexSize.x) * static_cast<float>(m_BossGaugeBarParam["CorrectionCoef"]);
		m_pBossGaugeBar->SetTexSize(TexSize);
	}
}

//============================================================================
// エラー検出
//============================================================================
bool CHUD_Manager::DetectError()
{
	// エラー検出用
	bool bError = 0;

	if (!m_pMapBase)
	{
		CRenderer::GetRenderer()->SetDebugString("マップ枠表示が出来ません");
		bError = 1;
	}

	if (!m_pMapRing)
	{
		CRenderer::GetRenderer()->SetDebugString("マップ輪表示が出来ません");
		bError = 1;
	}

	if (!m_pMapSymbolPlayer)
	{
		CRenderer::GetRenderer()->SetDebugString("プレイヤーマップシンボル表示が出来ません");
		bError = 1;
	}

	if (!m_pMapSymbolItem)
	{
		CRenderer::GetRenderer()->SetDebugString("アイテムマップシンボル表示が出来ません");
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

	if (m_pBossGaugeBack == nullptr)
	{
		CRenderer::GetRenderer()->SetDebugString("ボスゲージ背景表示が出来ません");
		bError = 1;
	}

	if (m_pBossGaugeBar == nullptr)
	{
		CRenderer::GetRenderer()->SetDebugString("ボスゲージバー表示が出来ません");
		bError = 1;
	}

	if (m_pBossGaugeBase == nullptr)
	{
		CRenderer::GetRenderer()->SetDebugString("ボスゲージ枠表示が出来ません");
		bError = 1;
	}

	// エラー検出状況を返す
	return bError;
}