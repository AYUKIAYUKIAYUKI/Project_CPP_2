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
#include "object_X.h"
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
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("総モーション数：" + to_string(m_Test.wMaxMotion));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	for (WORD wCntMotion = 0; wCntMotion < m_Test.wMaxMotion; ++wCntMotion)
	{
		const Motion* const pMotion = &m_Test.apMotion[wCntMotion];
		CRenderer::SetDebugString("現在のフレーム数　：" + to_string(pMotion->wNowFrame));
		CRenderer::SetDebugString("現在のキー数　　　：" + to_string(pMotion->wNowKey));
		CRenderer::SetDebugString("ループフラグ　　　：" + to_string(pMotion->bLoop));
		CRenderer::SetDebugString("総キー数　　　　　：" + to_string(pMotion->wMaxKey));
		CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			const MotionKey* const pKey = &pMotion->apKey[wCntMotionKey];
			CRenderer::SetDebugString("キー[" + to_string(wCntMotionKey) + "]の総フレーム数：" + to_string(pKey->wMaxFrame));
		}
	}
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
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
CMotion_Manager::CMotion_Manager()
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
	JSON Json = utility::OpenJsonFile("Data\\JSON\\motion_test.json");

	// 総パーツ数を取得
	const WORD& MaxParts = static_cast<WORD>(Json["MaxParts"]);

	// パーツオブジェクトを生成
	for (WORD wCntParts = 0; wCntParts < MaxParts; ++wCntParts)
	{
		m_Test.vpModelParts.push_back(CObject_X::Create(static_cast<CX_Manager::TYPE>(Json["ModelType"][wCntParts])));
	}

	// 総モーション数を取得
	m_Test.wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// モーション情報を生成
	m_Test.apMotion = DBG_NEW Motion[m_Test.wMaxMotion];

	// モーション情報の設定
	for (WORD wCntMotion = 0; wCntMotion < m_Test.wMaxMotion; ++wCntMotion)
	{
		// 再生中のキーとフレームを初期化
		m_Test.apMotion[wCntMotion].wNowKey = 0;
		m_Test.apMotion[wCntMotion].wNowFrame = 0;

		// ループフラグを取得
		m_Test.apMotion[wCntMotion].bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// モーションの総キー数を取得
		m_Test.apMotion[wCntMotion].wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// キー情報を生成
		m_Test.apMotion[wCntMotion].apKey = DBG_NEW MotionKey[m_Test.apMotion[wCntMotion].wMaxKey];

		// キー情報の設定
		for (WORD wCntMotionKey = 0; wCntMotionKey < m_Test.apMotion[wCntMotion].wMaxKey; ++wCntMotionKey)
		{
			// キーの総フレーム数を取得
			m_Test.apMotion[wCntMotion].apKey[wCntMotionKey].wMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotionKey]);
		}
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMotion_Manager::Uninit()
{
	// キー情報のポインタ配列を破棄
	for (WORD wCntMotion = 0; wCntMotion < m_Test.wMaxMotion; ++wCntMotion)
	{
		if (m_Test.apMotion[wCntMotion].apKey != nullptr)
		{
			delete[] m_Test.apMotion[wCntMotion].apKey;
			m_Test.apMotion[wCntMotion].apKey = nullptr;
		}
	}

	// モーション情報のポインタ配列を破棄
	if (m_Test.apMotion != nullptr)
	{
		delete[] m_Test.apMotion;
		m_Test.apMotion = nullptr;
	}
}