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
#include "fan.h"

// 入力取得用
#include "manager.h"

// デバッグ表示用
#include "renderer.h"

// オブジェクト用
#include "block.h"
#include "object_HUD.h"
#include "player.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace field_manager;
using namespace player;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CField_Manager* CField_Manager::m_pInstance = nullptr;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 初期設定
//============================================================================
HRESULT CField_Manager::Init()
{
	// プレイヤーを検索
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		// プレイヤーへのポインタを保持
		m_pPlayer = CUtility::DownCast(m_pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	}
	else
	{
		assert(false && "プレイヤーの検索結果がありませんでした");
	}

	// プレイヤーの体力を生成
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

	// 円柱の判定を生成
	m_pCylinderCollider = CObject_X::Create(static_cast<int>(CObject::LAYER::BACK));
	m_pCylinderCollider->Init();
	m_pCylinderCollider->BindModel(CModel_X_Manager::TYPE::CYLINDERCOLLIDER);
	m_pCylinderCollider->SetPos(m_pPlayer->GetPos());
	m_pCylinderCollider->SetRot(m_pPlayer->GetRot());
	m_pCylinderCollider->SetScale(GENERATE_RANGE_RADIUS);
	m_pCylinderCollider->SetAlpha(0.5f);

	// 扇形を生成
	m_pFan = CFan::Create();

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CField_Manager::Release()
{
	if (m_pInstance != nullptr)
	{
		// 終了処理
		m_pInstance->Uninit();

		// メモリを解放
		delete m_pInstance;

		// ポインタを初期化
		m_pInstance = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CField_Manager::Update()
{
	// 扇形の方角をプレイヤーの方角に
	m_pFan->SetDirection(m_pPlayer->GetDirection());

	// 扇形の更新処理
	m_pFan->Update();

	// 仮の生成メソッド
	TestCreate();

	// 仮の破棄メソッド
	TestDelete();

	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
	{
		// 仮の全破棄メソッド
		TestDeleteAll();
	}

	// HUDの更新
	UpdateHUD();
}

//============================================================================
// 描画処理
//============================================================================
void CField_Manager::Draw()
{
	// 扇形の描画処理
	m_pFan->Draw();
}

//============================================================================
// 自インスタンスを取得
//============================================================================
CField_Manager* CField_Manager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		// 生成
		m_pInstance->Create();
	}

	return m_pInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CField_Manager::CField_Manager() :
	m_pPlayer{ nullptr },
	m_pPlayerGauge{ nullptr },
	m_pPlayerGaugeWindow{ nullptr },
	m_pCylinderCollider{ nullptr },
	m_pFan{ nullptr }
{
	for (WORD i = 0; i < CPlayer::MAX_LIFE; ++i)
	{
		m_pPlayerLife[i] = nullptr;
	}
}

//============================================================================
// デストラクタ
//============================================================================
CField_Manager::~CField_Manager()
{

}

//============================================================================
// 生成
//============================================================================
void CField_Manager::Create()
{
	if (m_pInstance != nullptr)
	{ // 二重生成禁止
		assert(false);
	}

	// インスタンスを生成
	m_pInstance = DBG_NEW CField_Manager();
}

//============================================================================
// 終了処理
//============================================================================
void CField_Manager::Uninit()
{
	// 扇形を破棄
	if (m_pFan != nullptr)
	{
		m_pFan->Release();	// 解放
		m_pFan = nullptr;	// ポインタを初期化
	}
}

//============================================================================
// 仮の生成メソッド
//============================================================================
void CField_Manager::TestCreate()
{
	// ブロック数をカウント
	int nCntBlock = CObject::CountSpecificObject(CObject::TYPE::BLOCK);

#ifdef _DEBUG
	CRenderer::GetInstance()->SetDebugString("ブロック数:" + to_string(nCntBlock));
#endif

	// ブロック数が上限に満たなければ
	while (nCntBlock < MAX_BLOCK)
	{
		CInputKeyboard* pKeyboard = CManager::GetKeyboard();
		CInputPad* pPad = CManager::GetPad();

		/* スクリーン画面内で、どちらの方向を移動していたか */
		//float ScreenX = 0.0f;

		/* 直前の方角の変更を検出 */
		if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
		{ // カメラから見て左へ

		}
		else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
		{ // カメラから見て右へ

		}

		/* 移動していた方向に合わせて、範囲の端から新たな地形が登場してくる */

		// プレイヤータグを取得
		if (CObject::FindSpecificObject(CObject::TYPE::PLAYER))
		{
			// 生成座標計算用
			const float&	fDirection = m_pPlayer->GetDirection();	// プレイヤーの方角をコピー
			Vec3			NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// ブロック用の座標・向きを作成
			float			fRandomRange = 0.0f;					// ランダムな方角範囲

			// ①プレイヤーの進行方向に沿って生成されるように補正

			// 破棄範囲にはみ出さず生成されるように調整
			/* 初期座標が原点の場合、生成範囲の半径がフィールドの半径を下回ると無限ループ */
			//do
			//{
#if 0
				// ランダムに方角をずらす
				fRandomRange = CUtility::GetRandomValue<float>() * m_fCoeffRaondomRange;

				// 生成用の座標を決定
				NewPos.x = cosf(fDirection + fRandomRange) * FIELD_RADIUS;
				NewPos.y = fabsf(CUtility::GetRandomValue<float>());
				NewPos.z = sinf(fDirection + fRandomRange) * FIELD_RADIUS;

				// ブロック同士の幅を検出
				if (DetectAdjacentBlock(NewPos))
				{
					NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
				}
#else
				NewPos = { 0.0f,0.0f, FIELD_RADIUS };
#endif
			//} while (!CUtility::CylinderAndSphere(m_pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, NewPos, 10.0f));

			// 向きを決定
			NewRot.y = -(fDirection + fRandomRange);

			// ブロックを生成
			CBlock::Create(NewPos, NewRot);

			// ブロック数をカウントアップ
			nCntBlock++;
		}
	}
}

//============================================================================
// 隣接し合うブロックを検出
//============================================================================
bool CField_Manager::DetectAdjacentBlock(const Vec3& Pos)
{
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// ブロックタグの数をカウントする
	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロックタグにダウンキャスト
			CBlock* pBlock = nullptr;
			pBlock = CUtility::DownCast(pBlock, pObj);

			// 軸方向におけるブロックの隣接を検出する

			/* 今回は試験的にお互いの距離のみを考慮する */
			const Vec3& Vec = pBlock->GetPos() - Pos;

			/* ある程度接近してしまっているブロックが存在する場合 */
			if (sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= pBlock->GetSize().x * 5.0f)
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
// 仮の破棄メソッド
//============================================================================
void CField_Manager::TestDelete()
{
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// ブロックタグの数をカウントする
	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// プレイヤータグを取得
			if (CObject::FindSpecificObject(CObject::TYPE::PLAYER))
			{
				// オブジェクトをブロックタグにダウンキャスト
				CBlock* pBlock = nullptr;
				pBlock = CUtility::DownCast(pBlock, pObj);

				// 破棄方法の変更
#if 0
				m_pCylinderCollider->SetPos(m_pPlayer->GetPos());
				m_pCylinderCollider->SetRot(m_pPlayer->GetRot());

				// 逆に、円柱範囲外の場合消去
				if (!CUtility::CylinderAndSphere(m_pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, pBlock->GetPos(), 10.0f))
				{
					pBlock->SetRelease();
				}
#else
				m_pCylinderCollider->SetPos(Vec3(FLT_MAX, FLT_MAX, FLT_MAX));

				if (m_pFan->DetectInFanRange(pBlock->GetPos()))
				{
					pBlock->SetRelease();
				}
#endif
			}
		}

		pObj = pObj->GetNext();
	}
}

//============================================================================
// 仮の破棄メソッド
//============================================================================
void CField_Manager::TestDeleteAll()
{
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// ブロックタグの数をカウントする
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
			NewPosTarget.y = 100.0f;
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