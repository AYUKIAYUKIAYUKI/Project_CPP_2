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
	// 左方向を入力していたら
	if (CManager::GetKeyboard()->GetPress(DIK_A) || CManager::GetKeyboard()->GetRelease(DIK_A))
	{
		m_bDirection = 0;
	}
	else
	{
		m_bDirection = 1;
	}
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
	// 現在の方角を取得
	float fDirection = m_pCharacter->GetDirection();

	// 設定されている移動方向に方角を増加
	if (m_bDirection)
	{
		fDirection += m_pCharacter->GetMoveSpeed() * 5.0f;
	}
	else
	{
		fDirection += m_pCharacter->GetMoveSpeed() * -5.0f;
	}

	m_pCharacter->SetDirection(fDirection);

	Vec3 NewPosTarget = VEC3_INIT;										// 新規目標座標を作成
	NewPosTarget.x = cosf(fDirection) * CField_Manager::FIELD_RADIUS;	// X方向の座標を設定
	NewPosTarget.z = sinf(fDirection) * CField_Manager::FIELD_RADIUS;	// Z方向の座標を設定
	m_pCharacter->SetPosTarget(NewPosTarget);							// 目標座標を反映
}