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

// 入力取得用
#include "manager.h"

// デバッグ表示用
#include "renderer.h"

// オブジェクト用
#include "player.h"
#include "block.h"

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
	// ランダム範囲の設定
	m_fCoeffRaondomRange = 0.01f;

	// 円柱の判定を生成
	m_pCylinderCollider = CObject_X::Create(static_cast<int>(CObject::LAYER::BACK));

	// 初期設定
	m_pCylinderCollider->Init();

	// プレイヤーを格納
	CPlayer* pPlayer = nullptr;

	// プレイヤーを検索
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	}
	else
	{
		assert(false && "プレイヤーの検索結果がありませんでした");
	}

	// 各種パラメータ設定
	m_pCylinderCollider->SetPos(pPlayer->GetPos());
	m_pCylinderCollider->SetRot(pPlayer->GetRot());
	m_pCylinderCollider->SetScale(GENERATE_RANGE_RADIUS);
	m_pCylinderCollider->SetAlpha(0.25f);

	// モデルを設定
	m_pCylinderCollider->BindModel(CModel_X_Manager::TYPE::CYLINDERCOLLIDER);

	// 透明度を設定
	m_pCylinderCollider->SetAlpha(0.5f);

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
#ifdef _DEBUG
	// ランダム範囲の強度を表示
	CRenderer::GetInstance()->SetDebugString("ランダム範囲の強度:" + to_string(m_fCoeffRaondomRange));
#endif

	// 仮の生成メソッド
	TestCreate();

	// 仮の破棄メソッド
	TestDelete();

	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
	{
		// 仮の全破棄メソッド
		TestDeleteAll();
	}
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
	m_fCoeffRaondomRange{ 0.0f },
	m_pCylinderCollider{ nullptr }
{

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
		// プレイヤータグを取得
		if (CObject::FindSpecificObject(CObject::TYPE::PLAYER))
		{
			// オブジェクトをプレイヤータグにダウンキャスト
			CPlayer* pPlayer = nullptr;
			pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

			// 生成座標計算用
			const float&	fDirection = pPlayer->GetDirection();	// プレイヤーの方角をコピー
			Vec3			NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// ブロック用の座標・向きを作成
			float			fRandomRange = 0.0f;					// ランダムな方角範囲

			// ①プレイヤーの進行方向に沿って生成されるように補正

			// 破棄範囲にはみ出さず生成されるように調整
			/* 初期座標が原点の場合、生成範囲の半径がフィールドの半径を下回ると無限ループ */
			do
			{
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

			} while (!CUtility::CylinderAndSphere(pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, NewPos, 10.0f));

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
				
				// オブジェクトをプレイヤータグにダウンキャスト
				CPlayer* pPlayer = nullptr;
				pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

				m_pCylinderCollider->SetPos(pPlayer->GetPos());
				m_pCylinderCollider->SetRot(pPlayer->GetRot());

				// 逆に、円柱範囲外の場合消去
				if (!CUtility::CylinderAndSphere(pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, pBlock->GetPos(), 10.0f))
				{
					pBlock->SetRelease();
				}
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