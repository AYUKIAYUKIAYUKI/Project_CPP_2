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
#include "player_state_wall.h"
#include "player_state_slash.h"
#include "player_state_damage.h"
#include "constellation.h"

// フィールドパラメータ用
#include "field_manager.h"

// アクションデータ用
#include "field_builder.h"

// インプット取得用
#include "manager.h"

//****************************************************
// 静的メンバ変数の初期化
//****************************************************

// ジャンプ用パラメータ
JSON CPlayer_State_Jump::m_JumpParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\jump_param.json");

// ジャンプ一回当たりの増加加速度
float CPlayer_State_Jump::AMOUNT_JUMPACCEL = static_cast<float>(m_JumpParam["Power"]);

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
#ifdef _DEBUG
	// パラメータの再読み込み
	m_JumpParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\jump_param.json");
	AMOUNT_JUMPACCEL = static_cast<float>(m_JumpParam["Power"]);
#endif // _DEBUG

	// アクションデータのジャンプした回数をインクリメント
	CField_Manager::GetInstance()->GetFieldBuilder()->IncrementCntJump();

	// 飛び立ちモーションを再生
	m_pCharacter->SetNowMotion(7);
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
	// プレイヤーステートクラスの更新処理
	CPlayer_State::Update();

	// 星座エフェクトを発生
	CConstellation::GenerateSpread(m_pCharacter->GetPos());

	// 飛び立ちモーションが終了していたら
	if (m_pCharacter->GetNowMotion() == 7 &&
		m_pCharacter->GetStopState())
	{
		// 飛行モーションに変更
		m_pCharacter->SetNowMotion(8);
	}

	// Y軸方向の加速度が無く、Y方向の移動目標のノルムが小さい時
	if (m_pCharacter->GetVelY() == 0.0f &&
		fabsf(m_pCharacter->GetPosTarget().y - m_pCharacter->GetPos().y) <= 0.3f)
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
// 壁ずり状態へ
//============================================================================
void CPlayer_State_Jump::To_Wall()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Wall ());
	}
}

//============================================================================
// 斬撃状態へ
//============================================================================
void CPlayer_State_Jump::To_Slash()
{
	// 斬撃のキャストが溜まっていれば処理を行わない
	if (m_nCntSlashCast > 0)
		return;

	if (GetNextState() == nullptr)
	{
		// 再攻撃までのキャストカウントを設定
		m_nCntSlashCast = CPlayer_State_Slash::MAX_SLASH_CAST;

		// 軽くふわっと浮き上がるような加速度を強制発生
		m_pCharacter->SetVelY(1.0f);

		SetNextState(DBG_NEW CPlayer_State_Slash());
	}
}

//============================================================================
// ダメージ状態へ
//============================================================================
void CPlayer_State_Jump::To_Damage()
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

	if (pMouse->GetTrigger(0))
	{
		// 斬撃状態へ
		To_Slash();
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
	if (CManager::GetKeyboard()->GetRelease(DIK_SPACE) || CManager::GetKeyboard()->GetRelease(DIK_W) ||
		m_nJumpRemainDuration > static_cast<float>(m_JumpParam["Duration"]))
	{
		m_bEndRemain = true;
	}

	// 重力加速
	if (!m_bEndRemain)
	{
		// ジャンプ延長中はわずかに重力に逆らう
		m_pCharacter->SetVelY(m_pCharacter->GetVelY() + -CField_Manager::FIELD_GRAVITY * static_cast<float>(m_JumpParam["AntiPower"]));
	}
	else
	{
		// 延長期間が終了すると通常の重力加速を行う
		m_pCharacter->SetVelY(m_pCharacter->GetVelY() + CField_Manager::FIELD_GRAVITY);
	}
}