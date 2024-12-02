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
#include "character.h"
#include "player_state_dash.h"
#include "player_state_jump.h"
#include "player_state_slash.h"
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
	// プレイヤーステートクラスの更新処理
	CPlayer_State::Update();

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
	CInputMouse* pMouse = CManager::GetMouse();				// マウス
	CInputPad* pPad = CManager::GetPad();					// パッド

	// プレイヤーのパラメータを取得
	float fDirectionTarget = m_pCharacter->GetDirectionTarget();	// 目標方角を取得
	const float& fMoveSpeed = m_pCharacter->GetMoveSpeed();			// 移動速度を取得

	// X軸の入力
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
	{ // カメラから見て左へ

		// 方角を変動
		fDirectionTarget += -fMoveSpeed;

		// 自動で目標座標を変動した方角に合わせる
		m_pCharacter->AutoSetRotTarget();

		// 移動入力時にのみ
		if (pMouse->GetTrigger(1))
		{
			// ダッシュ状態へ
			To_Dash();
		}
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{ // カメラから見て右へ
		
		 // 方角を変動
		fDirectionTarget += fMoveSpeed;

		// 自動で目標座標を変動した方角に合わせる
		m_pCharacter->AutoSetRotTarget();

		// 移動入力時にのみ
		if (pMouse->GetTrigger(1))
		{
			// ダッシュ状態へ
			To_Dash();
		}
	}

	if (pMouse->GetTrigger(0))
	{
		// ジャンプ状態へ
		To_Jump();
	}
	else if (pKeyboard->GetTrigger(DIK_SPACE))
	{
		// 斬撃状態へ
		To_Slash();
	}

	// 目標方角を反映
	m_pCharacter->SetDirectionTarget(fDirectionTarget);
}

//============================================================================
// ダッシュ状態へ
//============================================================================
void CPlayer_State_Default::To_Dash()
{
	// ダッシュのキャストが溜まっていれば処理を行わない
	if (m_nCntDashCast > 0)
		return;

	if (GetNextState() == nullptr)
	{
		// 再ダッシュまでのキャストカウントを設定
		m_nCntDashCast = CPlayer_State_Dash::MAX_DASH_CAST;

		SetNextState(DBG_NEW CPlayer_State_Dash());
	}
}

//============================================================================
// ジャンプ状態へ
//============================================================================
void CPlayer_State_Default::To_Jump()
{
	// Y軸方向に加速度のかかった状態(空中)ならジャンプに変更しない
	if (m_pCharacter->GetVelY() != 0.0f)
		return;

	if (GetNextState() == nullptr)
	{
		// Y軸の加速度を大幅に増加
		m_pCharacter->SetVelY(CPlayer_State_Jump::AMOUNT_JUMPACCEL);

		SetNextState(DBG_NEW CPlayer_State_Jump());
	}
}

//============================================================================
// 斬撃状態へ
//============================================================================
void CPlayer_State_Default::To_Slash()
{
	// 斬撃のキャストが溜まっていれば処理を行わない
	if (m_nCntSlashCast > 0)
		return;

	if (GetNextState() == nullptr)
	{
		// 再攻撃までのキャストカウントを設定
		m_nCntSlashCast = CPlayer_State_Slash::MAX_SLASH_CAST;

		SetNextState(DBG_NEW CPlayer_State_Slash());
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