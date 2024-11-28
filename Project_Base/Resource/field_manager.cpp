//============================================================================
// 
// フィールドマネージャー [field_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "field_manager.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "object_HUD.h"

// オブジェクト用
#include "block.h"
#include "collision.h"
#include "sparks.h"

// デバッグ表示用
#include "fan.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define CHANGE_FIELRDCREATE_STYLE 0	// ブロックの生成方法切り替え

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CField_Manager* CField_Manager::m_pFieldManager = nullptr;	// フィールドマネージャーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CField_Manager::Update()
{
	// 火の粉を生成
	CSparks::AutoGenerate();

	// プレイヤーの現在の方角を扇形の方角にする
	m_pRenderFan->SetDirection(m_pPlayer->GetDirection());

	// 扇形表示の更新処理
	m_pRenderFan->Update();

#if !CHANGE_FIELRDCREATE_STYLE

	// ブロックの自動生成
	GenerateBlock();

	// ブロックの自動削除
	//DestroyBlock();

#endif	// CHANGE_FIELRDCREATE_STYLE

	// 仮の全破棄メソッド
	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
	{
		DestroyAllBlock();
	}

	// HUDの更新
	UpdateHUD();

	if (m_pPlayer->GetLife() <= 0)
	{
		
	}
}

//============================================================================
// 描画処理
//============================================================================
void CField_Manager::Draw()
{
	// 扇形の描画処理
	m_pRenderFan->Draw();
}

//============================================================================
// 生成
//============================================================================
HRESULT CField_Manager::Create()
{
	// 既に生成されていたら
	if (m_pFieldManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("フィールドマネージャーは既に生成されています", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// インスタンスを生成
	m_pFieldManager = DBG_NEW CField_Manager();

	// 生成失敗
	if (m_pFieldManager == nullptr)
	{
		return E_FAIL;
	}

	// フィールドマネージャーの初期設定
	if (FAILED(m_pFieldManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CField_Manager::Release()
{
	if (m_pFieldManager != nullptr)
	{
		// 終了処理
		m_pFieldManager->Uninit();

		// メモリを解放
		delete m_pFieldManager;

		// ポインタを初期化
		m_pFieldManager = nullptr;
	}
}

//============================================================================
// フィールドマネージャーを取得
//============================================================================
CField_Manager* CField_Manager::GetInstance()
{
	// 本体が存在しなければ
	if (m_pFieldManager == nullptr)
	{
		// 生成
		if (FAILED(Create()))
		{
			assert(false && "フィールドマネージャーの取得に失敗");
		}
	}

	return m_pFieldManager;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CField_Manager::CField_Manager() :
	m_pMap{ nullptr },
	m_pPlayer{ nullptr },
	m_pPlayerGauge{ nullptr },
	m_pPlayerGaugeWindow{ nullptr },
	m_pRenderFan{ nullptr }
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
CField_Manager::~CField_Manager()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CField_Manager::Init()
{
	// プレイヤーを検索
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{ // 発見

		// プレイヤーへのポインタを保持
		m_pPlayer = utility::DownCast(m_pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	}
	else
	{ // 見つからない

		// プレイヤーを生成
		m_pPlayer = CPlayer::Create();
	}

	// マップ表示を作成
	m_pMap = CObject_HUD::Create("Data\\JSON\\HUD\\map.json");
	m_pMap->BindTex(CTexture_Manager::TYPE::MAP);

	// プレイヤーの体力表示を生成
	for (WORD i = 0; i < CPlayer::MAX_LIFE; ++i)
	{
		std::string FilePath = "Data\\JSON\\HUD\\playerlife\\" + to_string(i) + ".json";
		m_pPlayerLife[i] = CObject_HUD::Create(FilePath);
		m_pPlayerLife[i]->BindTex(CTexture_Manager::TYPE::CIRCLE);
	}

	// プレイヤーのゲージを生成
	m_pPlayerGaugeWindow = CObject_HUD::Create("Data\\JSON\\HUD\\playergauge.json");
	m_pPlayerGaugeWindow->BindTex(CTexture_Manager::TYPE::PLAYERGAUGE);

	// プレイヤーのゲージウィンドウを生成
	m_pPlayerGaugeWindow = CObject_HUD::Create("Data\\JSON\\HUD\\playergaugewindow.json");
	m_pPlayerGaugeWindow->BindTex(CTexture_Manager::TYPE::PLAYERGAUGEWINDOW);

	// 扇形表示を生成
	m_pRenderFan = CFan::Create();

#if CHANGE_FIELRDCREATE_STYLE

	// 円の生成
	DEBUG_CIRCLE();

#endif	// CHANGE_FIELRDCREATE_STYLE

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CField_Manager::Uninit()
{
	// 扇形表示を破棄
	if (m_pRenderFan != nullptr)
	{
		m_pRenderFan->Release();	// 解放
		m_pRenderFan = nullptr;		// ポインタを初期化
	}
}

//============================================================================
// ブロックの自動生成
//============================================================================
void CField_Manager::GenerateBlock()
{
	// ブロック数をカウント
	int nCntBlock = CObject::CountSpecificObject(CObject::TYPE::BLOCK);

#ifdef _DEBUG	// ブロック数の表示
	CRenderer::SetDebugString("ブロック数:" + to_string(nCntBlock));
#endif	// _DEBUG

	// ブロック数が上限に満たなければ
	while (nCntBlock < MAX_BLOCK)
	{
		// 生成座標計算用
		const float&	fDirection = m_pPlayer->GetDirection();	// プレイヤーの現在の方角をコピー
		Vec3			NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// ブロック用の座標・向きを作成
		float			fRandomRange = 0.0f;					// ランダムな方角範囲

		// 破棄範囲にはみ出さず生成されるように調整
		/* 初期座標が原点の場合、生成範囲の半径がフィールドの半径を下回ると無限ループ */
		do
		{
			// ランダムに方角をずらす
			//fRandomRange = utility::GetRandomValue<float>();

			// 生成用の座標を決定
			NewPos.x = cosf(fDirection + fRandomRange) * FIELD_RADIUS;
			NewPos.y = fabsf(utility::GetRandomValue<float>());
			NewPos.z = sinf(fDirection + fRandomRange) * FIELD_RADIUS;

			// ブロック同士の幅を検出
			if (DetectAdjacentBlock(NewPos))
			{
				NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
			}

		} while (!m_pRenderFan->DetectInFanRange(NewPos));

		// 向きを決定
		NewRot.y = (fDirection + fRandomRange) + D3DX_PI * 0.5f;

		// ブロックを生成
		CBlock::Create(NewPos, NewRot);

		// ブロック数をカウントアップ
		nCntBlock++;
	}
}

//============================================================================
// 隣接し合うブロックを検出
//============================================================================
bool CField_Manager::DetectAdjacentBlock(const D3DXVECTOR3& Pos)
{
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロックタグにダウンキャスト
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			/* 今回は試験的にお互いの距離のみを考慮する */
			const Vec3& Vec = pBlock->GetPos() - Pos;

			/* ある程度接近してしまっているブロックが存在する場合 */
			if (sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= pBlock->GetSize().x)
			{
				// 座標の生成をやり直す
				return 1;
			}
		}

		pObj = pObj->GetNext();
	}

	return 0;
}

//============================================================================
// ブロックの自動削除
//============================================================================
void CField_Manager::DestroyBlock()
{
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::MIDDLE);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロックタグにダウンキャスト
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			// 扇形表示の範囲内にブロックが無ければ破棄
			if (!m_pRenderFan->DetectInFanRange(pBlock->GetPos()))
			{
				pBlock->SetRelease();
			}
		}

		pObj = pObj->GetNext();
	}
}

//============================================================================
// 全ブロックの削除
//============================================================================
void CField_Manager::DestroyAllBlock()
{
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::MIDDLE);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// 破棄予約
			pObj->SetRelease();
		}

		pObj = pObj->GetNext();
	}
}

//============================================================================
// HUDの更新処理
//============================================================================
void CField_Manager::UpdateHUD()
{
	// 体力量分のHUDが表示されるように調整
	for (WORD i = CPlayer::MAX_LIFE; i > 0; --i)
	{
		// 座標を取得
		Vec3 NewPosTarget = m_pPlayerLife[i - 1]->GetPosTarget();

		if (i > m_pPlayer->GetLife())
		{
			NewPosTarget.y = -100.0f;
		}
		else
		{
			NewPosTarget.y = 75.0f;
		}

		// 座標を反映
		m_pPlayerLife[i - 1]->SetPosTarget(NewPosTarget);
	}

#if 0	// HUD挙動の確認

	if (CManager::GetKeyboard()->GetPress(DIK_Z))
	{
		m_pPlayerGaugeWindow->SetVibration();
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_X))
	{
		m_pPlayerGaugeWindow->SetWaving();
	}

#endif
}

//============================================================================
// デバッグサークル
//============================================================================
void CField_Manager::DEBUG_CIRCLE()
{
	float fDirection = 0.0f;						// 方角
	float fAdder = (D3DX_PI * 2.0f) / MAX_BLOCK;	// 増加量

	for (WORD i = 0; i < MAX_BLOCK; ++i)
	{
		// 生成用の座標を決定
		Vec3 NewPos = VEC3_INIT;
		NewPos.x = cosf(fDirection) * FIELD_RADIUS;
		NewPos.y = 40.0f - utility::GetRandomValue<float>() * 0.15f;
		NewPos.z = -sinf(fDirection) * FIELD_RADIUS;

		// 生成用の向きを決定
		Vec3 NewRot = VEC3_INIT;
		//NewRot.y = utility::GetRandomValue<float>();
		NewRot.y = fDirection + D3DX_PI * 0.5f;

		// ブロックを生成
		CBlock::Create(NewPos, NewRot);

		// 方角を増加
		fDirection += fAdder;
	}
}