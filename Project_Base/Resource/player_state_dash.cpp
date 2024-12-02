//============================================================================
//
// プレイヤーダッシュステート [player_state_dash.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state_dash.h"
#include "player_state_default.h"
#include "player_state_damage.h"
#include "constellation.h"

// フィールドサイズ取得用
#include "field_manager.h"

// インプット取得用
#include "manager.h"

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
CPlayer_State_Dash::CPlayer_State_Dash() :
	CPlayer_State{},
	m_bDirection{ false },
	m_nDashDuration{ 0 }
{
	// アクションデータのダッシュした回数をインクリメント
	CField_Manager::GetInstance()->IncrementCntDash();

	// 左右の入力に合わせて移動方向のフラグを変更
	if (CManager::GetKeyboard()->GetPress(DIK_A) || CManager::GetKeyboard()->GetRelease(DIK_A))
		m_bDirection = 0;
	else
		m_bDirection = 1;

	// Y軸方向の加速度を無くす
	m_pCharacter->SetVelY(0.0f);
}

//============================================================================
// デストラクタ
//============================================================================
CPlayer_State_Dash::~CPlayer_State_Dash()
{

}

//============================================================================
// 更新処理
//============================================================================
void CPlayer_State_Dash::Update()
{
	// 星座エフェクトを発生
	for (int nCnt = 0; nCnt < 2; ++nCnt)
		CConstellation::GenerateSpread(m_pCharacter->GetPos());
		CConstellation::GenerateSpread(m_pCharacter->GetPosTarget());

	// 目標座標をダッシュ方向に増加
	SetPosTarget_Unnamed();

	// 継続期間をカウントアップ
	m_nDashDuration++;

	// 継続期間が最大に到達で
	if (m_nDashDuration >= MAX_DASH_DURATION)
	{
		// 通常状態へ
		To_Default();
	}
}

//============================================================================
// 通常状態へ
//============================================================================
void CPlayer_State_Dash::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}

//============================================================================
// ダメージ状態へ
//============================================================================
void CPlayer_State_Dash::To_Damage()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Damage());
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 目標座標をダッシュ方向に増加
//============================================================================
void CPlayer_State_Dash::SetPosTarget_Unnamed()
{
	// 目標方角をコピー
	float fDirectionTarget = m_pCharacter->GetDirectionTarget();

	// 設定されている移動方向に方角を増加
	if (m_bDirection)
	{
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * 3.0f;
	}
	else
	{
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * -3.0f;
	}

	// 目標方角を反映
	m_pCharacter->SetDirectionTarget(fDirectionTarget);

	// 自動で目標座標を変動した方角に合わせる
	m_pCharacter->AutoSetRotTarget();
}