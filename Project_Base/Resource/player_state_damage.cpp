//============================================================================
//
// プレイヤーダメージステート [player_state_damage.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state_damage.h"
#include "player_state_default.h"
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
CPlayer_State_Damage::CPlayer_State_Damage() :
	CPlayer_State{},
	m_nDamageDuration{ 0 }
{

}

//============================================================================
// デストラクタ
//============================================================================
CPlayer_State_Damage::~CPlayer_State_Damage()
{

}

//============================================================================
// 更新処理
//============================================================================
void CPlayer_State_Damage::Update()
{
	// 継続期間をカウントアップ
	m_nDamageDuration++;

	// 継続期間の半分まで行くと歩ける
	if (m_nDamageDuration >= MAX_DASH_DURATION * 0.5f)
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
		}
		else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
		{ // カメラから見て右へ

			 // 方角を変動
			fDirectionTarget += fMoveSpeed;

			// 自動で目標座標を変動した方角に合わせる
			m_pCharacter->AutoSetRotTarget();
		}

		// 目標方角を反映
		m_pCharacter->SetDirectionTarget(fDirectionTarget);
	}

	// 点滅
	if (m_nDamageDuration % 2 == 0)
	{
		//m_pCharacter->SetUseCol(true);
		//m_pCharacter->SetCol({ 1.0f, 0.0f, 0.0f, 0.25f });
	}
	else
	{
		//m_pCharacter->SetUseCol(false);
	}

	// 継続期間が最大に到達で
	if (m_nDamageDuration >= MAX_DASH_DURATION)
	{
		// 通常カラーに設定
		//m_pCharacter->SetUseCol(false);

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
// 通常状態へ
//============================================================================
void CPlayer_State_Damage::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}