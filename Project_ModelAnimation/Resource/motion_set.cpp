//============================================================================
// 
// モーションマネージャー [motion_manager.cpp]
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
	m_wMaxMotion{ 0 },
	m_apMotion{ nullptr }
{
	m_vpModelParts.clear();
}

//============================================================================
// デストラクタ
//============================================================================
CMotion_Set::~CMotion_Set()
{

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
	if (m_wNowFrame >= GetNowKey()->wMaxFrame)
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
	const WORD wFrameCoef = m_apMotion[m_wNowMotion].apKey[m_wNowKey].wMaxFrame - m_wNowFrame;

	// パーツ数を取得
	const WORD wSize = static_cast<WORD>(m_vpModelParts.size());

	// 全てのパーツがそれぞれの目標値へ補正したパラメータを設定する
	for (WORD wCntModelParts = 0; wCntModelParts < wSize; ++wCntModelParts)
	{
		// 目標縮尺
		Vec3 NewScale = m_vpModelParts[wCntModelParts]->GetScale();
		NewScale += (m_apMotion->apKey[m_wNowKey].apDest[wCntModelParts].ScaleTarget - NewScale) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetScale(NewScale);

		// 目標向き
		Vec3 NewRot = m_vpModelParts[wCntModelParts]->GetRot();
		NewRot += (m_apMotion->apKey[m_wNowKey].apDest[wCntModelParts].RotTarget - NewRot) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetRot(NewRot);

		// 目標座標
		Vec3 NewPos = m_vpModelParts[wCntModelParts]->GetPos();
		NewPos += (m_apMotion->apKey[m_wNowKey].apDest[wCntModelParts].PosTarget - NewPos) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetPos(NewPos);
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 現在のモーションのポインタを取得
//============================================================================
CMotion_Set::Motion* const CMotion_Set::GetNowMotion() const
{
	return &m_apMotion[m_wNowMotion];
}

//============================================================================
// 現在のキーのポインタを取得
//============================================================================
CMotion_Set::Key* const CMotion_Set::GetNowKey() const
{
	return &GetNowMotion()->apKey[m_wNowKey];
}
