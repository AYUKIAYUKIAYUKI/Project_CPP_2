//============================================================================
// 
// モーションセット [motion_set.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "motion_set.h"
#include "object_Parts.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CMotion_Set::CMotion_Set() :
	m_wNowMotion{ 0 },
	m_wNowKey{ 0 },
	m_wNowFrame{ 0 },
	m_vpModelParts{},
	m_wMaxParts{ 0 },
	m_wMaxMotion{ 0 },
	m_vpMotion{}
{
	m_vpModelParts.clear();
	m_vpMotion.clear();
}

//============================================================================
// デストラクタ
//============================================================================
CMotion_Set::~CMotion_Set()
{

}

//============================================================================
// 解放
//============================================================================
void CMotion_Set::Release()
{
	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		CMotion_Set::Motion* const pMotion = &m_vpMotion[wCntMotion];

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// キー情報のポインタを作成
			CMotion_Set::Key* const pKey = &pMotion->vpKey[wCntMotionKey];

			// 目標値情報のポインタ配列を破棄
			if (pKey->apDest != nullptr)
			{
				delete[] pKey->apDest;
				pKey->apDest = nullptr;
			}
		}
	}

	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// モーションごとにキー情報ベクターのクリア
		m_vpMotion[wCntMotion].vpKey.clear();
	}

	// モーション情報のベクターのクリア
	m_vpMotion.clear();

	// パーツ用オブジェクトの消去
	for (auto it : m_vpModelParts)
	{
		// 破棄予約
		it->SetRelease();
	}

	// モデルパーツベクターのクリア
	m_vpModelParts.clear();

	// 最後にこのモーションセットを破棄
	delete this;
}

//============================================================================
// 更新処理
//============================================================================
void CMotion_Set::Update()
{
	// 動作
	Animation();
}

//============================================================================
// 動作
//============================================================================
void CMotion_Set::Animation()
{
	// フレームカウント
	CountFrame();

	// 目標値への補正
	CorrectTarget();
}

//============================================================================
// フレームカウント
//============================================================================
void CMotion_Set::CountFrame()
{
	// 現在のフレーム数をインクリメント
	m_wNowFrame++;

	// フレーム数が、現在再生中のキーの総フレーム数に達したら
	if (m_wNowFrame >= GetNowKey()->nMaxFrame)
	{
		// 現在のフレーム数をリセット
		m_wNowFrame = 0;

		// 現在のキー数をインクリメント
		m_wNowKey++;

		// キー数が、現在再生中のモーションの総キー数に達したら
		if (m_wNowKey >= GetNowMotion()->wMaxKey)
		{
			// 現在のキー数をリセット
			m_wNowKey = 0;
		}
	}
}

//============================================================================
// 目標値への補正
//============================================================================
void CMotion_Set::CorrectTarget()
{
	// フレームの進行度合を作成 (総フレーム数 - 現在のフレーム)
	const WORD wFrameCoef = m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].nMaxFrame - m_wNowFrame;

	// 全てのパーツがそれぞれの目標値へ補正したパラメータを設定する
	for (WORD wCntModelParts = 0; wCntModelParts < m_wMaxParts; ++wCntModelParts)
	{
		// 目標縮尺
		Vec3 NewScale = m_vpModelParts[wCntModelParts]->GetScale();
		NewScale += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].ScaleTarget - NewScale) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetScale(NewScale);

		// 目標向き
		Vec3 NewRot = m_vpModelParts[wCntModelParts]->GetRot();
		NewRot += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].RotTarget - NewRot) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetRot(NewRot);

		// 目標座標
		Vec3 NewPos = m_vpModelParts[wCntModelParts]->GetPos();
		NewPos += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].PosTarget - NewPos) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetPos(NewPos);
	}
}

//============================================================================
// 生成
//============================================================================
CMotion_Set* CMotion_Set::Create(JSON Json)
{
	// モーションセットインスタンスを生成
	CMotion_Set* pNew = DBG_NEW CMotion_Set();

	// 生成失敗
	if (!pNew)
	{
		return nullptr;
	}

	// 総パーツ数を取得
	pNew->m_wMaxParts = static_cast<WORD>(Json["MaxParts"]);

	// パーツ数分のパーツオブジェクトを先行して生成
	for (WORD wCntParts = 0; wCntParts < pNew->m_wMaxParts; ++wCntParts)
	{
		pNew->m_vpModelParts.push_back(CObject_Parts::Create(static_cast<CX_Manager::TYPE>(Json["ModelType"][wCntParts]), nullptr));
	}

	// 生成されたパーツに対し、各種設定を行う
	for (WORD wCntParts = 0; wCntParts < pNew->m_wMaxParts; ++wCntParts)
	{
		// 親パーツのインデックス番号を取得
		const SHORT& shParentIdx = static_cast<SHORT>(Json["ParentIdx"][wCntParts]);

		// パーツのポインタをコピー
		CObject_Parts* pParts = pNew->m_vpModelParts[wCntParts];

		if (shParentIdx == -1)
		{
			// インデックスが-1のものは親を持たない
			pParts->SetParent(nullptr);
		}
		else
		{
			// 親パーツのポインタを渡す
			pParts->SetParent(pNew->m_vpModelParts[shParentIdx]);
		}

		// オフセット値を設定
		//pParts->SetScaleOffset(utility::JsonConvertToVec3(Json["ScaleOffset"][wCntParts]));
		//pParts->SetRotOffset(utility::JsonConvertToVec3(Json["RotOffset"][wCntParts]));
		pParts->SetPosOffset(utility::JsonConvertToVec3(Json["PosOffset"][wCntParts]));
	}

	// 総モーション数を取得
	pNew->m_wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// モーション数分のモーション情報を生成
	pNew->m_vpMotion.resize(pNew->m_wMaxMotion);

	// モーション情報の設定
	for (WORD wCntMotion = 0; wCntMotion < pNew->m_wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		Motion* const pMotion = &pNew->m_vpMotion[wCntMotion];

		// ループフラグを取得
		pMotion->bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// モーションの総キー数を取得
		pMotion->wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// キー情報の設定
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// キー情報オブジェクトを作成
			Key Key;

			// キーの総フレーム数を取得
			Key.nMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotion][wCntMotionKey]);

			// パーツ数分の目標値情報を生成
			Key.apDest = DBG_NEW CMotion_Set::KeyDest[pNew->m_wMaxParts];

			for (WORD wCntModelParts = 0; wCntModelParts < pNew->m_wMaxParts; ++wCntModelParts)
			{
				// 目標値情報のポインタを作成
				KeyDest* const pDest = &Key.apDest[wCntModelParts];

				// 各種パラメータを設定
				pDest->ScaleTarget = utility::JsonConvertToVec3(Json["ScaleTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);	// 目標縮尺
				pDest->RotTarget = utility::JsonConvertToVec3(Json["RotTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// 目標向き
				pDest->PosTarget = utility::JsonConvertToVec3(Json["PosTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// 目標座標
			}

			pMotion->vpKey.push_back(Key);
		}
	}

	return pNew;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 現在のモーションのポインタを取得
//============================================================================
CMotion_Set::Motion* const CMotion_Set::GetNowMotion()
{
	return &m_vpMotion[m_wNowMotion];
}

//============================================================================
// 現在のキーのポインタを取得
//============================================================================
CMotion_Set::Key* const CMotion_Set::GetNowKey()
{
	return &GetNowMotion()->vpKey[m_wNowKey];
}
