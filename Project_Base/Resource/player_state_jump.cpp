//============================================================================
//
// プレイヤージャンプステート [player_state_jump.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state_jump.h"
#include "player_state_default.h"
#include "player_state_dash.h"

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
CPlayer_State_Jump::CPlayer_State_Jump() :
	CPlayer_State{},
	m_nJumpRemainDuration{ 0 },
	m_bEndRemain{ false }
{

}

//============================================================================
// デストラクタ
//============================================================================
CPlayer_State_Jump::~CPlayer_State_Jump()
{

}

//============================================================================
// 更新処理
//============================================================================
void CPlayer_State_Jump::Update()
{
	// 間違いなくプレイヤーが着地している場合
	if (m_pCharacter->GetVelY() == 0.0f && fabsf(m_pCharacter->GetPos().y - m_pCharacter->GetPosTarget().y) <= 1.0f)
	{
		// 通常状態へ
		To_Default();
	}

	// 重力の補正
	AdjustGravity();

	// 操作
	Control();
}

//============================================================================
// 通常状態へ
//============================================================================
void CPlayer_State_Jump::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}

//============================================================================
// ダッシュ状態へ
//============================================================================
void CPlayer_State_Jump::To_Dash()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Dash());
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 操作
//============================================================================
void CPlayer_State_Jump::Control()
{
	// インプット系取得
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();	// キーボード
	CInputMouse* pMouse = CManager::GetMouse();				// マウス
	CInputPad* pPad = CManager::GetPad();					// パッド

	// プレイヤーのパラメータを取得
	float fDirectionTarget = m_pCharacter->GetDirectionTarget();	// 目標方角を取得
	const float& fMoveSpeed = m_pCharacter->GetMoveSpeed();			// 移動速度を取得

	// X軸の入力
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
	{ // カメラから見て左へ

		// 方角を変動
		fDirectionTarget += -fMoveSpeed * 1.25f;

		// 自動で目標座標を変動した方角に合わせる
		m_pCharacter->AutoSetRotTarget();

		// 移動入力時のみ
		if (pMouse->GetTrigger(1))
		{
			// ダッシュ状態へ
			To_Dash();
		}
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{ // カメラから見て右へ

		 // 方角を変動
		fDirectionTarget += fMoveSpeed * 1.25f;

		// 自動で目標座標を変動した方角に合わせる
		m_pCharacter->AutoSetRotTarget();

		// 移動入力時のみ
		if (pMouse->GetTrigger(1))
		{
			// ダッシュ状態へ
			To_Dash();
		}
	}

	// 方角を反映
	m_pCharacter->SetDirectionTarget(fDirectionTarget);
}

//============================================================================
// 重力の補正
//============================================================================
void CPlayer_State_Jump::AdjustGravity()
{
	// 入力延長期間
	m_nJumpRemainDuration++;

	// ジャンプの入力をやめるか、入力の継続期間が最大延長猶予に達すると延長を終了する
	if (CManager::GetKeyboard()->GetRelease(DIK_SPACE) || m_nJumpRemainDuration > MAX_JUMPREMAIN_DURATION)
	{
		m_bEndRemain = true;
	}

	// 重力加速
	if (!m_bEndRemain)
	{
		JSON Json = utility::OpenJsonFile("Data\\debug.json");

		// ジャンプ延長中はわずかに重力に逆らう
		m_pCharacter->SetVelY(m_pCharacter->GetVelY() + -CField_Manager::FIELD_GRAVITY * static_cast<float>(Json["Anti"]));
	}
	else
	{
		// 延長期間が終了すると通常の重力加速を行う
		m_pCharacter->SetVelY(m_pCharacter->GetVelY() + CField_Manager::FIELD_GRAVITY);
	}
}