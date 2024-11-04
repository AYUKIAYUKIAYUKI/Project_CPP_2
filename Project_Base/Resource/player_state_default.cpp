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

// 変更先ステート
#include "player_state_dash.h"

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
// デフォルトコンストラクタ
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

		// 目標向きを移動方向に設定
		SetRotTargetToMoveDirection();

		if (CManager::GetKeyboard()->GetTrigger(DIK_RSHIFT))
		{
			// ダッシュをする
			To_Dash(false);
		}
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{ // カメラから見て右へ
		
		 // 方角を変動
		fDirection += fMoveSpeed;

		// 目標向きを移動方向に設定
		SetRotTargetToMoveDirection();

		if (CManager::GetKeyboard()->GetTrigger(DIK_RSHIFT))
		{
			// ダッシュをする
			To_Dash(true);
		}
	}

	// 方角を反映
	m_pCharacter->SetDirection(fDirection);

	// 目標座標を方角に合わせて設定
	SetPosTargetByDirection();
}

//============================================================================
// 目標向きを移動方向に設定
//============================================================================
void CPlayer_State_Default::SetRotTargetToMoveDirection()
{
	/* キー入力の判定外でこの関数を呼ぶと、座標移動の終わりがけの慣性力で向きが狂う */

	Vec3 NewRotTarget = m_pCharacter->GetRotTarget();								// 目標向きを取得
	const Vec3& MoveVec = m_pCharacter->GetPosTarget() - m_pCharacter->GetPos();	// 移動方向のベクトルを作成
	NewRotTarget.y = atan2f(-MoveVec.x, -MoveVec.z);								// 目標向きを移動方向に
	m_pCharacter->SetRotTarget(NewRotTarget);										// 目標向きを反映
}

//============================================================================
// 方角から目標座標を設定
//============================================================================
void CPlayer_State_Default::SetPosTargetByDirection()
{
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace field_manager;

	// 方角を取得
	const float& fDirection = m_pCharacter->GetDirection();

	Vec3 NewPosTarget = VEC3_INIT;										// 新規目標座標を作成
	NewPosTarget.x = cosf(fDirection) * CField_Manager::FIELD_RADIUS;	// X方向の座標を設定
	NewPosTarget.z = sinf(fDirection) * CField_Manager::FIELD_RADIUS;	// Z方向の座標を設定
	m_pCharacter->SetPosTarget(NewPosTarget);							// 目標座標を反映
}

//============================================================================
// ダッシュをする
//============================================================================
void CPlayer_State_Default::To_Dash(bool bDirection)
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Dash(bDirection));
	}
}