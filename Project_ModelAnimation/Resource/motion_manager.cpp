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
	// 動作
	Animation();

	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("総モーション数：" + to_string(m_Actor.wMaxMotion));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	for (WORD wCntMotion = 0; wCntMotion < m_Actor.wMaxMotion; ++wCntMotion)
	{
		const ActorMotion* const pMotion = &m_Actor.apMotion[wCntMotion];
		CRenderer::SetDebugString("現在のフレーム数　：" + to_string(m_Actor.wNowFrame));
		CRenderer::SetDebugString("現在のキー数　　　：" + to_string(m_Actor.wNowKey));
		CRenderer::SetDebugString("ループフラグ　　　：" + to_string(pMotion->bLoop));
		CRenderer::SetDebugString("総キー数　　　　　：" + to_string(pMotion->wMaxKey));
		CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			const MotionKey* const pKey = &pMotion->apKey[wCntMotionKey];
			CRenderer::SetDebugString("キー[" + to_string(wCntMotionKey) + "]の総フレーム数：" + to_string(pKey->wMaxFrame));

			for (WORD wCntModelParts = 0; wCntModelParts < m_Actor.vpModelParts.size(); ++wCntModelParts)
			{
				const KeyDest* const pDest = &pKey->apDest[wCntModelParts];
				CRenderer::SetDebugString("ScaleTarget：" + to_string(pDest->ScaleTarget.x) + "：" + to_string(pDest->ScaleTarget.y) + "：" + to_string(pDest->ScaleTarget.z));
				CRenderer::SetDebugString("RotTarget：" + to_string(pDest->RotTarget.x) + "：" + to_string(pDest->RotTarget.y) + "：" + to_string(pDest->RotTarget.z));
				CRenderer::SetDebugString("PosTarget：" + to_string(pDest->PosTarget.x) + "：" + to_string(pDest->PosTarget.y) + "：" + to_string(pDest->PosTarget.z));
			}
		}
	}
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");

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
	m_Actor{ 0, 0, 0, {}, 0, nullptr }
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

	// パーツ数分のパーツオブジェクトを生成
	for (WORD wCntParts = 0; wCntParts < MaxParts; ++wCntParts)
	{
		m_Actor.vpModelParts.push_back(CObject_X::Create(static_cast<CX_Manager::TYPE>(Json["ModelType"][wCntParts])));
	}

	// 総モーション数を取得
	m_Actor.wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// モーション数分のモーション情報を生成
	m_Actor.apMotion = DBG_NEW ActorMotion[m_Actor.wMaxMotion];

	// モーション情報の設定
	for (WORD wCntMotion = 0; wCntMotion < m_Actor.wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		ActorMotion* const pMotion = &m_Actor.apMotion[wCntMotion];

		// ループフラグを取得
		pMotion->bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// モーションの総キー数を取得
		pMotion->wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// キー数分のキー情報を生成
		pMotion->apKey = DBG_NEW MotionKey[pMotion->wMaxKey];

		// キー情報の設定
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// キー情報のポインタを作成
			MotionKey* const pKey = &pMotion->apKey[wCntMotionKey];

			// キーの総フレーム数を取得
			pKey->wMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotionKey]);

			// パーツ数分の目標値情報を生成
			pKey->apDest = DBG_NEW KeyDest[m_Actor.vpModelParts.size()];

			for (WORD wCntModelParts = 0; wCntModelParts < m_Actor.vpModelParts.size(); ++wCntModelParts)
			{
				// 目標値情報のポインタを作成
				KeyDest* const pDest = &pKey->apDest[wCntModelParts];

				// 各種パラメータを設定
				pDest->ScaleTarget = utility::JsonConvertToVec3(Json["ScaleTarget"][wCntMotionKey][wCntModelParts]);	// 目標縮尺
				pDest->RotTarget = utility::JsonConvertToVec3(Json["RotTarget"][wCntMotionKey][wCntModelParts]);		// 目標向き
				pDest->PosTarget = utility::JsonConvertToVec3(Json["PosTarget"][wCntMotionKey][wCntModelParts]);		// 目標座標
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
	for (WORD wCntMotion = 0; wCntMotion < m_Actor.wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		ActorMotion* const pMotion = &m_Actor.apMotion[wCntMotion];

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// キー情報のポインタを作成
			MotionKey* const pKey = &pMotion->apKey[wCntMotionKey];

			// 目標値情報のポインタ配列を破棄
			if (pKey->apDest != nullptr)
			{
				delete[] pKey->apDest;
				pKey->apDest = nullptr;
			}
		}
	}

	// モーション情報内のキー情報を破棄
	for (WORD wCntMotion = 0; wCntMotion < m_Actor.wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		ActorMotion* const pMotion = &m_Actor.apMotion[wCntMotion];

		// キー情報のポインタ配列を破棄
		if (pMotion->apKey != nullptr)
		{
			delete[] pMotion->apKey;
			pMotion->apKey = nullptr;
		}
	}

	// モーション情報のポインタ配列を破棄
	if (m_Actor.apMotion != nullptr)
	{
		delete[] m_Actor.apMotion;
		m_Actor.apMotion = nullptr;
	}

	// パーツ用オブジェクトの消去
	for (auto it : m_Actor.vpModelParts)
	{
		// 破棄予約
		it->SetRelease();
	}
}

//============================================================================
// リセット
//============================================================================
void CMotion_Manager::Reset()
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_F5))
	{
		// 一旦解放
		Release();

		// 即再生成
		Create();
	}
}

//============================================================================
// 動作
//============================================================================
void CMotion_Manager::Animation()
{
	// フレームカウント
	CountFrame();

	// 目標値への補正
	CorrectTarget();
}

//============================================================================
// フレームカウント
//============================================================================
void CMotion_Manager::CountFrame()
{
	// 現在のフレーム数をインクリメント
	m_Actor.wNowFrame++;

	// フレーム数が、現在再生中のキーの総フレーム数に達したら
	if (m_Actor.wNowFrame >= m_Actor.apMotion[m_Actor.wNowMotion].apKey[m_Actor.wNowKey].wMaxFrame)
	{
		// 現在のフレーム数をリセット
		m_Actor.wNowFrame = 0;

		// 現在のキー数をインクリメント
		m_Actor.wNowKey++;

		// キー数が、現在再生中のモーションの総キー数に達したら
		if (m_Actor.wNowKey >= m_Actor.apMotion[m_Actor.wNowMotion].wMaxKey)
		{
			// 現在のキー数をリセット
			m_Actor.wNowKey = 0;
		}
	}
}

//============================================================================
// 目標値への補正
//============================================================================
void CMotion_Manager::CorrectTarget()
{
	// フレームの進行度合を作成 (総フレーム数 - 現在のフレーム)
	const WORD wFrameCoef = m_Actor.apMotion[m_Actor.wNowMotion].apKey[m_Actor.wNowKey].wMaxFrame - m_Actor.wNowFrame;

	// 全てのパーツがそれぞれの目標値へ補正したパラメータを設定する
	for (WORD wCntModelParts = 0; wCntModelParts < m_Actor.vpModelParts.size(); ++wCntModelParts)
	{
		// 目標向き
		Vec3 NewRot = m_Actor.vpModelParts[wCntModelParts]->GetRot();
		NewRot += (m_Actor.apMotion->apKey[m_Actor.wNowKey].apDest[wCntModelParts].RotTarget - NewRot) / wFrameCoef;
		m_Actor.vpModelParts[wCntModelParts]->SetRot(NewRot);

		// 目標座標
		Vec3 NewPos = m_Actor.vpModelParts[wCntModelParts]->GetPos();
		NewPos += (m_Actor.apMotion->apKey[m_Actor.wNowKey].apDest[wCntModelParts].PosTarget - NewPos) / wFrameCoef;
		m_Actor.vpModelParts[wCntModelParts]->SetPos(NewPos);
	}
}