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

// 変更先ステート
#include "player_state_default.h"

// フィールドサイズ取得用
#include "field_manager.h"

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
// デフォルトコンストラクタ
//============================================================================
CPlayer_State_Dash::CPlayer_State_Dash() :
	CPlayer_State{},
	m_bDirection{ false },
	m_nDashDuration{ 0 }
{

}

//============================================================================
// 方向指定コンストラクタ
//============================================================================
CPlayer_State_Dash::CPlayer_State_Dash(bool bDirection) :
	CPlayer_State{},
	m_bDirection{ bDirection },
	m_nDashDuration{ 0 }
{

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
	// 目標向きを移動方向に設定
	SetRotTargetToMoveDirection();

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
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 目標向きを移動方向に設定
//============================================================================
void CPlayer_State_Dash::SetRotTargetToMoveDirection()
{
	Vec3 NewRotTarget = m_pCharacter->GetRotTarget();								// 目標向きを取得
	const Vec3& MoveVec = m_pCharacter->GetPosTarget() - m_pCharacter->GetPos();	// 移動方向のベクトルを作成
	NewRotTarget.y = atan2f(-MoveVec.x, -MoveVec.z);								// 目標向きを移動方向に
	m_pCharacter->SetRotTarget(NewRotTarget);										// 目標向きを反映
}

//============================================================================
// 目標座標をダッシュ方向に増加
//============================================================================
void CPlayer_State_Dash::SetPosTarget_Unnamed()
{
	// 方角を設定
	float fDirection = m_pCharacter->GetDirection();

	// 設定されている方角に合わせて移動
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

//============================================================================
// ステート - 通常状態へ
//============================================================================
void CPlayer_State_Dash::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}