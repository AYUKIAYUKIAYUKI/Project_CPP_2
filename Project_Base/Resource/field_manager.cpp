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

// オブジェクト用
#include "player.h"
#include "block.h"

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
	// 仮のメソッド
	TestMethod();

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
CField_Manager::CField_Manager()
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
void CField_Manager::TestMethod()
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
		if (nCntBlock >= 20)
		{
			return;
		}

		pObj = pObj->GetNext();
	}

	// プレイヤータグを取得
	if (CObject::FindObject(CObject::TYPE::PLAYER))
	{
		// オブジェクトをプレイヤータグにダウンキャスト
		CPlayer* pPlayer = CUtility::DownCast<CPlayer, CObject>(CObject::FindObject(CObject::TYPE::PLAYER));

		// プレイヤーの座標から角度を計算
		D3DXVECTOR3 NewPos = pPlayer->GetPos();
		float f角度 = atan2f(NewPos.z, NewPos.x), f反映量 = 150.0f, f乱数 = CUtility::GetRandomValue<float>() * 0.01f;
		NewPos.x = cosf(f角度 + f乱数) * f反映量;
		NewPos.y = fabsf(CUtility::GetRandomValue<float>());
		NewPos.z = sinf(f角度 + f乱数) * f反映量;

		// 向きを決定
		D3DXVECTOR3 NewRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		NewRot.y = -(f角度 + f乱数);

		CBlock::Create(NewPos, NewRot);
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
				CBlock* pBlock = CUtility::DownCast<CBlock, CObject>(pObj);
				
				// オブジェクトをプレイヤータグにダウンキャスト
				CPlayer* pPlayer = CUtility::DownCast<CPlayer, CObject>(CObject::FindObject(CObject::TYPE::PLAYER));

				//if (pP)
				//{
				//	pBlock->SetRelease();
				//}
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