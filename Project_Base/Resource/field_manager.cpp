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
	m_fCoeffRaondomRange = 0.1f;

	// 円柱の判定を生成
	m_pCylinderCollider = CObject_X::Create();

	// 初期設定
	m_pCylinderCollider->Init();

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
	CRenderer::GetInstance()->SetDebugString(to_string(m_fCoeffRaondomRange));
#endif

	// 仮の生成メソッド
	TestCreate();

	// 仮の破棄メソッド
	TestDelete();

	if (CManager::GetKeyboard()->GetTrigger(DIK_BACK))
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
	m_fCoeffRaondomRange(0.0f),
	m_pCylinderCollider(nullptr)
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
	int nCntBlock = 0;

	// ミドルオブジェクトをを取得
	CObject* pObj = CObject::GetObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// ブロックタグの数をカウントする
	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			nCntBlock++;
		}

		// 一定カウントで以降の処理を行わない
		if (nCntBlock >= MAX_BLOCK)
		{
			return;
		}

		pObj = pObj->GetNext();
	}

	while (nCntBlock < MAX_BLOCK)
	{
		// プレイヤータグを取得
		if (CObject::FindObject(CObject::TYPE::PLAYER))
		{
			// オブジェクトをプレイヤータグにダウンキャスト
			CPlayer* pPlayer = nullptr;
			pPlayer = CUtility::DownCast(pPlayer, CObject::FindObject(CObject::TYPE::PLAYER));

			// プレイヤーの方角をコピー
			const float& fDirection = pPlayer->GetDirection();

			// ブロック用の座標・向きを作成
			Vec3 NewPos = VEC3_INIT, NewRot = VEC3_INIT;

			// ランダムな方角範囲
			float fRandomRange = 0.0f;

			// 破棄範囲にはみ出さず生成されるように調整
			do
			{
				// ランダムに方角をずらす
				fRandomRange = CUtility::GetRandomValue<float>() * m_fCoeffRaondomRange;

				// 生成用の座標を決定
				NewPos.x = cosf(fDirection + fRandomRange) * FIELD_RADIUS;
				NewPos.y = fabsf(CUtility::GetRandomValue<float>());
				NewPos.z = sinf(fDirection + fRandomRange) * FIELD_RADIUS;

			} while (!CUtility::CylinderAndPoint(pPlayer->GetPos(), 250.0f, 250.0f, NewPos));

			// 向きを決定
			NewRot.y = -(fDirection + fRandomRange);

			CBlock::Create(NewPos, NewRot);

			nCntBlock++;
		}
	}
}

//============================================================================
// 仮の破棄メソッド
//============================================================================
void CField_Manager::TestDelete()
{
	// ミドルオブジェクトをを取得
	CObject* pObj = CObject::GetObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// ブロックタグの数をカウントする
	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// プレイヤータグを取得
			if (CObject::FindObject(CObject::TYPE::PLAYER))
			{
				// オブジェクトをブロックタグにダウンキャスト
				CBlock* pBlock = nullptr;
				pBlock = CUtility::DownCast(pBlock, pObj);
				
				// オブジェクトをプレイヤータグにダウンキャスト
				CPlayer* pPlayer = nullptr;
				pPlayer = CUtility::DownCast(pPlayer, CObject::FindObject(CObject::TYPE::PLAYER));

				m_pCylinderCollider->SetPos(pPlayer->GetPos());
				m_pCylinderCollider->SetRot(pPlayer->GetRot());
				m_pCylinderCollider->SetScale(250.0f);

				// 逆に、円柱範囲外の場合消去
				if (!CUtility::CylinderAndPoint(pPlayer->GetPos(), 250.0f, 250.0f, pBlock->GetPos()))
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
	// ミドルオブジェクトをを取得
	CObject* pObj = CObject::GetObject(static_cast<int>(CObject::LAYER::MIDDLE));

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