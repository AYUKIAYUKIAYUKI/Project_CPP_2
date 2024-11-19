//============================================================================
// 
// モーションマネージャー [motion_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "motion_manager.h"
#include "object_Parts.h"
#include "manager.h"
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバの初期化
//****************************************************
CMotion_Manager* CMotion_Manager::m_pMotionManager = nullptr;	// モーションマネージャーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CMotion_Manager::Update()
{
	// 編集
	Edit();

	if (m_bPlay)
	{
		// 動作
		m_MotionSet->Animation();
	}

	// リセット
	Reset();

	// デバッグ表示
	PrintDebug();
}

//============================================================================
// 生成
//============================================================================
HRESULT CMotion_Manager::Create()
{
	// 既に生成されていたら
	if (m_pMotionManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("モーションマネージャーは既に生成されています", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// インスタンスを生成
	m_pMotionManager = DBG_NEW CMotion_Manager();

	// 生成失敗
	if (m_pMotionManager == nullptr)
	{
		return E_FAIL;
	}

	// モーションマネージャーの初期設定
	if (FAILED(m_pMotionManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CMotion_Manager::Release()
{
	if (m_pMotionManager != nullptr)
	{
		// モーションマネージャーの終了処理
		m_pMotionManager->Uninit();

		// メモリの解放
		delete m_pMotionManager;

		// ポインタの初期化
		m_pMotionManager = nullptr;
	}
}

//============================================================================
// モーションマネージャーを取得
//============================================================================
CMotion_Manager* CMotion_Manager::GetInstance()
{
	// 本体が存在しなければ
	if (m_pMotionManager == nullptr)
	{
		// 生成
		if (FAILED(Create()))
		{
			assert(false && "モーションマネージャーの取得に失敗");
		}
	}

	return m_pMotionManager;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CMotion_Manager::CMotion_Manager() :
	m_Json{},
	m_MotionSet{ nullptr },
	m_wSelectParts{ 0 },
	m_fPosEditCoef{ 0.0f },
	m_wSelectMotion{ 0 },
	m_wSelectKey{ 0 },
	m_bPlay{ false }
{
	
}

//============================================================================
// デストラクタ
//============================================================================
CMotion_Manager::~CMotion_Manager()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CMotion_Manager::Init()
{
	// ジェイソンデータを取得
	m_Json = utility::OpenJsonFile("Data\\JSON\\motion_export.json");

	// モーションセットを生成
	m_MotionSet = CMotion_Set::Create(m_Json);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMotion_Manager::Uninit()
{
	if (m_MotionSet != nullptr)
	{
		// モーションセットの解放処理
		m_MotionSet->Release();

		// ポインタを初期化
		m_MotionSet = nullptr;
	}
}

//============================================================================
// デバッグ表示
//============================================================================
void CMotion_Manager::PrintDebug()
{
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("総パーツ数：" + to_string(m_MotionSet->m_wMaxParts));
	CRenderer::SetDebugString("総モーション数：" + to_string(m_MotionSet->m_wMaxMotion));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet->m_wMaxMotion; ++wCntMotion)
	{
		const CMotion_Set::Motion* const pMotion = &m_MotionSet->m_apMotion[wCntMotion];
		CRenderer::SetDebugString("<再生待機モーション情報>");
		CRenderer::SetDebugString("現在のフレーム数　：" + to_string(m_MotionSet->m_wNowFrame));
		CRenderer::SetDebugString("現在のキー数　　　：" + to_string(m_MotionSet->m_wNowKey));
		CRenderer::SetDebugString("ループフラグ　　　：" + to_string(pMotion->bLoop));
		CRenderer::SetDebugString("総キー数　　　　　：" + to_string(pMotion->wMaxKey));
		CRenderer::SetDebugString("ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー");
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			const CMotion_Set::Key* const pKey = &pMotion->vpKey[wCntMotionKey];
			CRenderer::SetDebugString("キー[" + to_string(wCntMotionKey) + "]の総フレーム数：" + to_string(pKey->wMaxFrame));
		}
	}
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("<編集情報>");
	CRenderer::SetDebugString("現在の選択パーツ　　：" + to_string(m_wSelectParts));
	CRenderer::SetDebugString("座標編集の強度　　　：" + to_string(m_fPosEditCoef));
	CRenderer::SetDebugString("現在の選択モーション：" + to_string(m_wSelectMotion));
	CRenderer::SetDebugString("現在の選択キー　　　：" + to_string(m_wSelectKey));
	for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_vpModelParts.size(); ++wCntModelParts)
	{
		const CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[wCntModelParts];
		CRenderer::SetDebugString("Scale：" + utility::ToPrecision(pDest->ScaleTarget.x) + "：" + utility::ToPrecision(pDest->ScaleTarget.y) + "：" + utility::ToPrecision(pDest->ScaleTarget.z) + "："
			"Rot：" + utility::ToPrecision(pDest->RotTarget.x) + "：" + utility::ToPrecision(pDest->RotTarget.y) + "：" + utility::ToPrecision(pDest->RotTarget.z) + "："
			"Pos：" + utility::ToPrecision(pDest->PosTarget.x) + "：" + utility::ToPrecision(pDest->PosTarget.y) + "：" + utility::ToPrecision(pDest->PosTarget.z));
	}
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
}

//============================================================================
// 編集
//============================================================================
void CMotion_Manager::Edit()
{
	// 再生切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_F5))
	{
		m_bPlay = !m_bPlay;
	}

	// パーツ情報の編集
	EditParts();

	// モーション情報の編集
	EditMotion();

	// キー情報の編集
	EditKey();

	// フレーム情報の編集
	EditFrame();

	// エクスポート
	if (CManager::GetKeyboard()->GetTrigger(DIK_F1))
	{
		Export();
	}
}

//============================================================================
// パーツ情報の編集
//============================================================================
void CMotion_Manager::EditParts()
{
	// 選択パーツ切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_1))
	{
		m_wSelectParts > 0 ? m_wSelectParts-- : m_wSelectParts = static_cast<WORD>(m_MotionSet->m_vpModelParts.size()) - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
	{
		m_wSelectParts < m_MotionSet->m_vpModelParts.size() - 1 ? m_wSelectParts++ : m_wSelectParts = 0;
	}

	// 選択パーツを透過
	for (WORD wCntParts = 0; wCntParts < m_MotionSet->m_vpModelParts.size(); ++wCntParts)
	{
		if (m_wSelectParts == wCntParts)
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetCol({ 1.0f, 0.5f, 0.25f, 0.25f });
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(true);
		}
		else
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(false);
		}
	}

	// 目標値情報の編集
	EditDest();
}

//============================================================================
// 目標値情報の編集
//============================================================================
void CMotion_Manager::EditDest()
{
	// 選択中のパーツの目標値情報のポインタを作成
	CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[m_wSelectParts];

	if (CManager::GetKeyboard()->GetTrigger(DIK_U))
	{
		pDest->RotTarget.x += D3DX_PI * 0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_O))
	{
		pDest->RotTarget.x += D3DX_PI * -0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_I))
	{
		pDest->RotTarget.y += D3DX_PI * 0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_K))
	{
		pDest->RotTarget.y += D3DX_PI * -0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_J))
	{
		pDest->RotTarget.z += D3DX_PI * 0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_L))
	{
		pDest->RotTarget.z += D3DX_PI * -0.05f;
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_R))
	{
		m_fPosEditCoef += -0.1f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_Y))
	{
		m_fPosEditCoef += 0.1f;
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_F))
	{
		pDest->PosTarget.x += -m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_H))
	{
		pDest->PosTarget.x += m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_T))
	{
		pDest->PosTarget.y += m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_G))
	{
		pDest->PosTarget.y += -m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_V))
	{
		pDest->PosTarget.z += -m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_N))
	{
		pDest->PosTarget.z += m_fPosEditCoef;
	}

	// 目標値を反映
	m_Json["ScaleTarget"][m_wSelectKey][m_wSelectParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
	m_Json["RotTarget"][m_wSelectKey][m_wSelectParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
	m_Json["PosTarget"][m_wSelectKey][m_wSelectParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
}

//============================================================================
// モーション情報の編集
//============================================================================
void CMotion_Manager::EditMotion()
{
	// 選択モーション切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_3))
	{
		m_wSelectMotion > 0 ? m_wSelectMotion-- : m_wSelectMotion = m_MotionSet->m_wMaxMotion - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_4))
	{
		m_wSelectMotion < m_MotionSet->m_wMaxMotion - 1 ? m_wSelectMotion++ : m_wSelectMotion = 0;
	}
}

//============================================================================
// キー情報の編集
//============================================================================
void CMotion_Manager::EditKey()
{
	// 選択キー切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_5))
	{
		m_wSelectKey > 0 ? m_wSelectKey-- : m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_6))
	{
		m_wSelectKey < GetSelectMotion()->wMaxKey - 1 ? m_wSelectKey++ : m_wSelectKey = 0;
	}

	// 総キー数の切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_F2) && GetSelectMotion()->wMaxKey > 1)
	{
		// 総キー数をデクリメント
		GetSelectMotion()->wMaxKey--;
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;

		// キー情報のポインタを作成
		CMotion_Set::Key* const pKey = &GetSelectMotion()->vpKey[GetSelectMotion()->wMaxKey];

		// 目標値情報のポインタ配列を破棄
		if (pKey->apDest != nullptr)
		{
			delete[] pKey->apDest;
			pKey->apDest = nullptr;
		}

		// 末尾のキー情報を削除
		GetSelectMotion()->vpKey.pop_back();

		// 選択キー番号を調整
		if (m_wSelectKey >= GetSelectMotion()->wMaxKey)
		{
			m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
		}
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F3))
	{
		// キー情報オブジェクトを新規作成
		CMotion_Set::Key Key;

		// キーの総フレーム数を設定
		Key.wMaxFrame = 1;
		m_Json["MaxFrame"][GetSelectMotion()->wMaxKey] = Key.wMaxFrame;

		// パーツ数分の目標値情報を生成
		Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet->m_wMaxParts];

		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// 目標値情報のポインタを作成
			CMotion_Set::KeyDest* const pDest = &Key.apDest[wCntModelParts];

			// 各種パラメータを設定
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };	// 目標縮尺
			m_Json["ScaleTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
			pDest->RotTarget = { VEC3_INIT };	// 目標向き
			m_Json["RotTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
			pDest->PosTarget = { VEC3_INIT };	// 目標座標
			m_Json["PosTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
		}

		GetSelectMotion()->vpKey.push_back(Key);

		// 総キー数をインクリメント
		GetSelectMotion()->wMaxKey++;
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;
	}
}

//============================================================================
// キー情報の再確保
//============================================================================
void CMotion_Manager::ResizeKey()
{
	
}

//============================================================================
// フレーム情報の編集
//============================================================================
void CMotion_Manager::EditFrame()
{
	// 総フレーム増減
	if (CManager::GetKeyboard()->GetTrigger(DIK_7) && m_MotionSet->GetNowKey()->wMaxFrame > 1)
	{
		GetSelectKey()->wMaxFrame--;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_8))
	{
		GetSelectKey()->wMaxFrame++;
	}
	else if (CManager::GetKeyboard()->GetRelease(DIK_7) || CManager::GetKeyboard()->GetRelease(DIK_8))
	{
		m_Json["MaxFrame"][m_wSelectKey] = GetSelectKey()->wMaxFrame;
	}
}

//============================================================================
// エクスポート
//============================================================================
void CMotion_Manager::Export()
{
	// ファイル名を作成
	std::string FileName = "Data\\JSON\\motion_export.json";

	// ファイルを書き出し展開
	std::ofstream Ofs(FileName, std::ios::out);

	if (Ofs.good())
	{
		// ジェイソンデータをシリアライズ
		Ofs << m_Json.dump(1, '	');	// 第一引数 -> インデント数, 第二引数 -> インデント形式

		CRenderer::SetTimeString("モーションデータ[" + FileName + "]を書き出しました", 120);
	}
	else
	{
		assert(false && "ファイルの書き込みに失敗");
	}
}

//============================================================================
// リセット
//============================================================================
void CMotion_Manager::Reset()
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_F7))
	{
		// 選択番号情報を初期化
		m_wSelectParts = 0;
		m_wSelectMotion = 0;
		m_wSelectKey = 0;

		// 一旦解放
		m_MotionSet->Release();

		// 即再生成
		m_MotionSet = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\motion_export.json"));
	}
}

//============================================================================
// 選択中のパーツのポインタを取得
//============================================================================
CObject_Parts* const CMotion_Manager::GetSelectParts() const
{
	return m_MotionSet->m_vpModelParts[m_wSelectParts];
}

//============================================================================
// 選択中のモーションのポインタを取得
//============================================================================
CMotion_Set::Motion* const CMotion_Manager::GetSelectMotion() const
{
	return &m_MotionSet->m_apMotion[m_wSelectMotion];
}

//============================================================================
// 選択中のキーのポインタを取得
//============================================================================
CMotion_Set::Key* const CMotion_Manager::GetSelectKey() const
{
	return &GetSelectMotion()->vpKey[m_wSelectKey];
}