//============================================================================
//
// プレイヤー通常ステート [player_state_default.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state_default.h"
#include "player_state_dash.h"
#include "player_state_jump.h"
#include "player_state_damage.h"

// フィールド取得用
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
CPlayer_State_Default::CPlayer_State_Default() :
	CPlayer_State{}
{

}

//============================================================================
// デストラクタ
//============================================================================
CPlayer_State_Default::~CPlayer_State_Default()
{

}

//============================================================================
// 更新処理
//============================================================================
void CPlayer_State_Default::Update()
{
	// 操作
	Control();

	// 重力加速
	m_pCharacter->SetVelY(m_pCharacter->GetVelY() + CField_Manager::FIELD_GRAVITY);
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 操作
//============================================================================
void CPlayer_State_Default::Control()
{
	// インプット系取得
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();	// キーボード
	CInputPad* pPad = CManager::GetPad();					// パッド

	// プレイヤーのパラメータを取得
	float fDirection = m_pCharacter->GetDirection();		// 方角を取得
	const float& fMoveSpeed = m_pCharacter->GetMoveSpeed();	// 移動速度を取得

	// X軸の入力
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
	{ // カメラから見て左へ

		// 方角を変動
		fDirection += -fMoveSpeed;

	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{ // カメラから見て右へ
		
		 // 方角を変動
		fDirection += fMoveSpeed;
	}

	if (pKeyboard->GetTrigger(DIK_RSHIFT))
	{
		// ダッシュ状態へ
		To_Dash();
	}
	else if (pKeyboard->GetTrigger(DIK_SPACE))
	{
		// ジャンプ状態へ
		To_Jump();
	}

	// 方角を反映
	m_pCharacter->SetDirection(fDirection);
}

//============================================================================
// ダッシュ状態へ
//============================================================================
void CPlayer_State_Default::To_Dash()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Dash());
	}
}

//============================================================================
// ジャンプ状態へ
//============================================================================
void CPlayer_State_Default::To_Jump()
{
	if (GetNextState() == nullptr)
	{
		// Y軸の加速度を大幅に増加
		m_pCharacter->SetVelY(CPlayer_State_Jump::AMOUNT_JUMPACCEL);

		SetNextState(DBG_NEW CPlayer_State_Jump());
	}
}

//============================================================================
// ダメージ状態へ
//============================================================================
void CPlayer_State_Default::To_Damage()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Damage());
	}
}