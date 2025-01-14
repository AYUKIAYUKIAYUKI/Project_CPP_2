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
#include "sound.h"
#include "constellation.h"

// アクションデータ用
#include "field_manager.h"
#include "field_builder.h"

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
	m_bDirection{ false }
{
	// アクションデータのダッシュした回数をインクリメント
	CField_Manager::GetInstance()->GetFieldBuilder()->IncrementCntDash();

	// 左右の入力に合わせて移動方向のフラグを変更
	if (CManager::GetKeyboard()->GetPress(DIK_A) || CManager::GetKeyboard()->GetRelease(DIK_A))
		m_bDirection = 0;
	else
		m_bDirection = 1;

	// ダッシュモーションを再生
	m_pCharacter->SetNowMotion(6);

	// ダッシュを鳴らす
	CSound::GetInstance()->Play(CSound::LABEL::DASH);
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
	// プレイヤーステートクラスの更新処理
	CPlayer_State::Update();

	// 星座エフェクトを発生
	for (WORD wCnt = 0; wCnt < 2; ++wCnt)
	{
		CConstellation::GenerateSpread(m_pCharacter->GetPos());
		CConstellation::GenerateSpread(m_pCharacter->GetPosTarget());
	}

	// Y軸方向の加速度を無くす
	m_pCharacter->SetVelY(0.0f);

	// 目標座標をダッシュ方向に増加
	SetPosToFacing();

	// 継続期間が最大に到達で
	if (m_pCharacter->GetStopState())
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

		// 空中のダッシュ解除後に着地をしてしまわないため
		m_pCharacter->SetVelY(-0.001f);

		// 落下を考慮して、飛行モーションをセットしながら通常状態へ
		m_pCharacter->SetNowMotion(8);
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
void CPlayer_State_Dash::SetPosToFacing()
{
	// 目標方角をコピー
	float fDirectionTarget = m_pCharacter->GetDirectionTarget();

	// 設定されている移動方向に方角を増加
	if (m_bDirection)
	{
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * 1.5f;
	}
	else
	{
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * -1.5f;
	}

	// 目標方角を反映
	m_pCharacter->SetDirectionTarget(fDirectionTarget);

	// 自動で目標座標を変動した方角に合わせる
	m_pCharacter->AutoSetRotTarget();
}