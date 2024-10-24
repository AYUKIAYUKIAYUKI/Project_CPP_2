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
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace field_manager;

	// プレイヤーパラメータ用
	float fDirection = m_pPlayer->GetDirection();			// 方角を取得
	const float& fMoveSpeed = m_pPlayer->GetMoveSpeed();	// 移動速度を取得

	// インプット系取得
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();	// キーボード
	CInputPad* pPad = CManager::GetPad();					// パッド

	// X軸の入力
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
	{
		fDirection += -fMoveSpeed;
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{
		fDirection += fMoveSpeed;
	}

	// 方角を反映
	m_pPlayer->SetDirection(fDirection);

	// 目標座標を反映
	Vec3 NewPosTarget = VEC3_INIT;
	NewPosTarget.x = cosf(fDirection) * CField_Manager::FIELD_RADIUS;
	NewPosTarget.z = sinf(fDirection) * CField_Manager::FIELD_RADIUS;
	m_pPlayer->SetPosTarget(NewPosTarget);

	// 目標向きを反映
	Vec3 NewRotTarget = m_pPlayer->GetRotTarget();
	const Vec3& NegVec = VEC3_INIT - m_pPlayer->GetPos();	// プレイヤーから原点への逆位置ベクトルを計算
	NewRotTarget.y = atan2f(NegVec.x, NegVec.z);			// プレイヤーの目標向きを逆位置ベクトル方向に
	m_pPlayer->SetRotTarget(NewRotTarget);

	if (CManager::GetKeyboard()->GetTrigger(DIK_RSHIFT))
	{
		// ダッシュをする
		To_Dash();
	}
}

//============================================================================
// ダッシュをする
//============================================================================
void CPlayer_State_Default::To_Dash()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Dash());
	}
}