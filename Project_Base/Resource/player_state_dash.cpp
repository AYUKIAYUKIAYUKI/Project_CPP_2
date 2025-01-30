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
#include "object_X.h"
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
	m_bDirection{ false },
	m_nDuration{ 0 }
{
	// アクションデータのダッシュした回数をインクリメント
	CField_Manager::GetInstance()->GetFieldBuilder()->IncrementCntDash();

	// 左右の入力に合わせて移動方向のフラグを変更
	if (CManager::GetKeyboard()->GetPress(DIK_A) || CManager::GetKeyboard()->GetRelease(DIK_A) ||
		CManager::GetPad()->GetPress(CInputPad::JOYKEY::LEFT) || CManager::GetPad()->GetRelease(CInputPad::JOYKEY::LEFT) ||
		CManager::GetPad()->GetJoyStickL().X < 0)
		m_bDirection = 0;
	else
		m_bDirection = 1;

	// キャラクターのポインタをプレイヤークラスにダウンキャスト
	CPlayer* pPlayer = utility::DownCast<CPlayer, CCharacter>(m_pCharacter);

	// 疾走効果が有効中で
	if (pPlayer->IsEnabledBoots())
	{
		// 特殊走行モーションを再生
		m_pCharacter->SetNowMotion(11);
	}
	else
	{
		// ダッシュモーションを再生
		m_pCharacter->SetNowMotion(6);
	}

	// ダッシュを鳴らす
	CSound::GetInstance()->Play(CSound::LABEL::DASH);
}

//============================================================================
// デストラクタ
//============================================================================
CPlayer_State_Dash::~CPlayer_State_Dash()
{
	// 全てのエフェクトを
	for (auto it : m_vEffect)
	{
		// 破棄設定
		it->SetRelease();
	}
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer_State_Dash::Update()
{
	// プレイヤーステートクラスの更新処理
	CPlayer_State::Update();

	// 継続期間をインクリメント
	++m_nDuration;

	// 星座エフェクトを発生
	for (WORD wCnt = 0; wCnt < 2; ++wCnt)
	{
		CConstellation::GenerateSpread(m_pCharacter->GetPos());
		CConstellation::GenerateSpread(m_pCharacter->GetPosTarget());
	}

	// キャラクターのポインタをプレイヤークラスにダウンキャスト
	CPlayer* pPlayer = utility::DownCast<CPlayer, CCharacter>(m_pCharacter);

	// 疾走効果が有効中で
	if (pPlayer->IsEnabledBoots())
	{
		{ // 1つずつ螺旋状に並べ、増やしていく

			// ルーンエフェクトを1つ作成
			CObject_X* pEffect = CObject_X::Create(CX_Manager::TYPE::SUM0 + (m_nDuration % 12));

			// 座標・向きを調整
			pEffect->SetPos(pPlayer->GetPos());
			pEffect->SetRot(pPlayer->GetRot());

			// エフェクトを保持
			m_vEffect.push_back(pEffect);
		}

		// 経過時間に合わせたルーン用縮尺を作成
		float fScale = static_cast<float>(35 - m_nDuration) * 0.065f;

		// 保持している全てのルーンエフェクトを編集
		for (auto it : m_vEffect)
		{
			// じわじわと縮小
			it->SetScale({ fScale, fScale, fScale });

			// プレイヤーから遠ざかる
			Vec3 AddMove = (it->GetPos() - pPlayer->GetPos()) * 0.001f;
			it->SetPos(it->GetPos() + AddMove);
		}
	}

	// Y軸方向の加速度を無くす
	m_pCharacter->SetVelY(0.0f);

	// 目標座標をダッシュ方向に増加
	SetPosToFacing();

#if 0
	// 入力取得用
	CInputMouse* pMouse = CManager::GetManager()->GetMouse();
	CInputPad* pPad = CManager::GetManager()->GetPad();

	// ダッシュ入力をやめていたら
	if (m_nDuration > 10)
	{
		if (!pMouse->GetPress(1) ||
			!pPad->GetPress(CInputPad::JOYKEY::X))
		{
			// 通常状態へ
			To_Default();
		}
	}
#endif

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
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * 1.35f;
	}
	else
	{
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * -1.35f;
	}

	// 目標方角を反映
	m_pCharacter->SetDirectionTarget(fDirectionTarget);

	// 自動で目標座標を変動した方角に合わせる
	m_pCharacter->AutoSetRotTarget();
}