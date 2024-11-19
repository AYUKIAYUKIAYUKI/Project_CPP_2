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
		m_MotionSet.Animation();
	}

#if 1
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("総モーション数：" + to_string(m_MotionSet.m_wMaxMotion));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet.m_wMaxMotion; ++wCntMotion)
	{
		const CMotion_Set::Motion* const pMotion = &m_MotionSet.m_apMotion[wCntMotion];
		CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
		CRenderer::SetDebugString("<再生待機モーション情報>");
		CRenderer::SetDebugString("現在のフレーム数　：" + to_string(m_MotionSet.m_wNowFrame));
		CRenderer::SetDebugString("現在のキー数　　　：" + to_string(m_MotionSet.m_wNowKey));
		CRenderer::SetDebugString("ループフラグ　　　：" + to_string(pMotion->bLoop));
		CRenderer::SetDebugString("総キー数　　　　　：" + to_string(pMotion->wMaxKey));
		CRenderer::SetDebugString("ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー");
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			const CMotion_Set::Key* const pKey = &pMotion->apKey[wCntMotionKey];
			CRenderer::SetDebugString("キー[" + to_string(wCntMotionKey) + "]の総フレーム数：" + to_string(pKey->wMaxFrame));
		}
	}
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("現在の選択パーツ　　：" + to_string(m_wSelectParts));
	CRenderer::SetDebugString("座標編集の強度　　　：" + to_string(m_fPosEditCoef));
	CRenderer::SetDebugString("現在の選択モーション：" + to_string(m_wSelectMotion));
	CRenderer::SetDebugString("現在の選択キー　　　：" + to_string(m_wSelectKey));
	for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet.m_vpModelParts.size(); ++wCntModelParts)
	{
		const CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[wCntModelParts];
		CRenderer::SetDebugString("Scale：" + utility::ToPrecision(pDest->ScaleTarget.x) + "：" + utility::ToPrecision(pDest->ScaleTarget.y) + "：" + utility::ToPrecision(pDest->ScaleTarget.z) + "："
			"Rot：" + utility::ToPrecision(pDest->RotTarget.x) + "：" + utility::ToPrecision(pDest->RotTarget.y) + "：" + utility::ToPrecision(pDest->RotTarget.z) + "："
			"Pos：" + utility::ToPrecision(pDest->PosTarget.x) + "：" + utility::ToPrecision(pDest->PosTarget.y) + "：" + utility::ToPrecision(pDest->PosTarget.z));
	}
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
#endif

	// リセット
	Reset();
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
	m_MotionSet{/* 0, 0, 0, {}, 0, nullptr */},
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
	// JSONファイルからモーション情報を取得
	//m_Json = utility::OpenJsonFile("Data\\JSON\\player_motion.json");
	m_Json = utility::OpenJsonFile("Data\\JSON\\motion_export.json");

	// 総パーツ数を取得
	const WORD& MaxParts = static_cast<WORD>(m_Json["MaxParts"]);

	// パーツ数分のパーツオブジェクトを先行して生成
	for (WORD wCntParts = 0; wCntParts < MaxParts; ++wCntParts)
	{
		m_MotionSet.m_vpModelParts.push_back(CObject_Parts::Create(static_cast<CX_Manager::TYPE>(m_Json["ModelType"][wCntParts]), nullptr));
	}

	// 生成されたパーツに対し、各種設定を行う
	for (WORD wCntParts = 0; wCntParts < MaxParts; ++wCntParts)
	{
		// 親パーツのインデックス
		const SHORT& shParentIdx = static_cast<SHORT>(m_Json["ParentIdx"][wCntParts]);

		// パーツのポインタをコピー
		CObject_Parts* pParts = m_MotionSet.m_vpModelParts[wCntParts];

		if (shParentIdx == -1)
		{
			pParts->SetParent(nullptr);
		}
		else
		{
			pParts->SetParent(m_MotionSet.m_vpModelParts.at(shParentIdx));
		}

		// オフセット値を設定
		pParts->SetPosOffset(utility::JsonConvertToVec3(m_Json["PosOffset"][wCntParts]));
	}

	// 総モーション数を取得
	m_MotionSet.m_wMaxMotion = static_cast<WORD>(m_Json["MaxMotion"]);

	// モーション数分のモーション情報を生成
	m_MotionSet.m_apMotion = DBG_NEW CMotion_Set::Motion[m_MotionSet.m_wMaxMotion];

	// モーション情報の設定
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet.m_wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		CMotion_Set::Motion* const pMotion = &m_MotionSet.m_apMotion[wCntMotion];

		// ループフラグを取得
		pMotion->bLoop = static_cast<bool>(m_Json["Loop"][wCntMotion]);

		// モーションの総キー数を取得
		pMotion->wMaxKey = static_cast<WORD>(m_Json["MaxKey"][wCntMotion]);

		// キー数分のキー情報を生成
		pMotion->apKey = DBG_NEW CMotion_Set::Key[pMotion->wMaxKey];

		// キー情報の設定
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// キー情報のポインタを作成
			CMotion_Set::Key* const pKey = &pMotion->apKey[wCntMotionKey];

			// キーの総フレーム数を取得
			pKey->wMaxFrame = static_cast<WORD>(m_Json["MaxFrame"][wCntMotionKey]);

			// パーツ数分の目標値情報を生成
			pKey->apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet.m_vpModelParts.size()];

			for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet.m_vpModelParts.size(); ++wCntModelParts)
			{
				// 目標値情報のポインタを作成
				CMotion_Set::KeyDest* const pDest = &pKey->apDest[wCntModelParts];

				// 各種パラメータを設定
				pDest->ScaleTarget = utility::JsonConvertToVec3(m_Json["ScaleTarget"][wCntMotionKey][wCntModelParts]);	// 目標縮尺
				pDest->RotTarget = utility::JsonConvertToVec3(m_Json["RotTarget"][wCntMotionKey][wCntModelParts]);		// 目標向き
				pDest->PosTarget = utility::JsonConvertToVec3(m_Json["PosTarget"][wCntMotionKey][wCntModelParts]);		// 目標座標
			}
		}
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMotion_Manager::Uninit()
{
	// キー情報内の目標値情報を破棄
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet.m_wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		CMotion_Set::Motion* const pMotion = &m_MotionSet.m_apMotion[wCntMotion];

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// キー情報のポインタを作成
			CMotion_Set::Key* const pKey = &pMotion->apKey[wCntMotionKey];

			// 目標値情報のポインタ配列を破棄
			if (pKey->apDest != nullptr)
			{
				delete[] pKey->apDest;
				pKey->apDest = nullptr;
			}
		}
	}

	// モーション情報内のキー情報を破棄
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet.m_wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		CMotion_Set::Motion* const pMotion = &m_MotionSet.m_apMotion[wCntMotion];

		// キー情報のポインタ配列を破棄
		if (pMotion->apKey != nullptr)
		{
			delete[] pMotion->apKey;
			pMotion->apKey = nullptr;
		}
	}

	// モーション情報のポインタ配列を破棄
	if (m_MotionSet.m_apMotion != nullptr)
	{
		delete[] m_MotionSet.m_apMotion;
		m_MotionSet.m_apMotion = nullptr;
	}

	// パーツ用オブジェクトの消去
	for (auto it : m_MotionSet.m_vpModelParts)
	{
		// 破棄予約
		it->SetRelease();
	}
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
		m_wSelectParts > 0 ? m_wSelectParts-- : m_wSelectParts = static_cast<WORD>(m_MotionSet.m_vpModelParts.size()) - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
	{
		m_wSelectParts < m_MotionSet.m_vpModelParts.size() - 1 ? m_wSelectParts++ : m_wSelectParts = 0;
	}

	// 選択パーツを透過
	for (WORD wCntParts = 0; wCntParts < m_MotionSet.m_vpModelParts.size(); ++wCntParts)
	{
		if (m_wSelectParts == wCntParts)
		{
			m_MotionSet.m_vpModelParts[wCntParts]->SetCol({ 1.0f, 0.5f, 0.25f, 0.25f });
			m_MotionSet.m_vpModelParts[wCntParts]->SetUseCol(true);
		}
		else
		{
			m_MotionSet.m_vpModelParts[wCntParts]->SetUseCol(false);
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
		m_wSelectMotion > 0 ? m_wSelectMotion-- : m_wSelectMotion = m_MotionSet.m_wMaxMotion - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_4))
	{
		m_wSelectMotion < m_MotionSet.m_wMaxMotion - 1 ? m_wSelectMotion++ : m_wSelectMotion = 0;
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

	/*
	// 総キー数の切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_F2))
	{
		GetSelectMotion()->wMaxKey--;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F3))
	{
		GetSelectMotion()->wMaxKey++;
	}
	else if (CManager::GetKeyboard()->GetRelease(DIK_F2) || CManager::GetKeyboard()->GetRelease(DIK_F3))
	{
		// キー情報の再確保
		ResizeKey();
	}
	*/
}

//============================================================================
// キー情報の再確保
//============================================================================
void CMotion_Manager::ResizeKey()
{
	/* 横着しないできちんとコピーすること！ */
}

//============================================================================
// フレーム情報の編集
//============================================================================
void CMotion_Manager::EditFrame()
{
	// 総フレーム増減
	if (CManager::GetKeyboard()->GetTrigger(DIK_7) && m_MotionSet.GetNowKey()->wMaxFrame > 1)
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
		// 一旦解放
		Release();

		// 即再生成
		Create();
	}
}

//============================================================================
// 選択中のパーツのポインタを取得
//============================================================================
CObject_Parts* const CMotion_Manager::GetSelectParts() const
{
	return m_MotionSet.m_vpModelParts[m_wSelectParts];
}

//============================================================================
// 選択中のモーションのポインタを取得
//============================================================================
CMotion_Set::Motion* const CMotion_Manager::GetSelectMotion() const
{
	return &m_MotionSet.m_apMotion[m_wSelectMotion];
}

//============================================================================
// 選択中のキーのポインタを取得
//============================================================================
CMotion_Set::Key* const CMotion_Manager::GetSelectKey() const
{
	return &GetSelectMotion()->apKey[m_wSelectKey];
}