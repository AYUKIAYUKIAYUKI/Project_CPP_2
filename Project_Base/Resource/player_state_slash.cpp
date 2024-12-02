//============================================================================
//
// プレイヤー斬撃ステート [player_state_slash.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state_slash.h"
#include "player_state_default.h"
#include "player_state_damage.h"
#include "collision.h"

// フィールドサイズ取得用
#include "field_manager.h"

// インプット取得用
#include "manager.h"

#include "boss.h"

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
CPlayer_State_Slash::CPlayer_State_Slash() :
	CPlayer_State{},
	m_nSlashDuration{ 0 },
	m_SlashType{ SLASH_TYPE::LEFT },
	m_pBndSlash{ std::make_unique<CBounding_Sphere>() }
{
	// 斬撃のバウンディングのパラメータを設定
	Vec3 Pos = m_pCharacter->GetPos();
	m_pBndSlash->SetCenterPos(Pos);
	m_pBndSlash->SetRadius(20.0f);
}

//============================================================================
// デストラクタ
//============================================================================
CPlayer_State_Slash::~CPlayer_State_Slash()
{
	// 斬撃のバウンディングを破棄
	if (m_pBndSlash != nullptr)
	{
		m_pBndSlash.reset();
	}
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer_State_Slash::Update()
{
	// プレイヤーステートクラスの更新処理
	CPlayer_State::Update();

	// 衝突検出
	SlashHitCheck();

	// 継続期間をカウントアップ
	++m_nSlashDuration;

	// 継続期間が最大に到達で
	if (m_nSlashDuration >= MAX_SLASH_DURATION)
	{
		// 通常状態へ
		To_Default();
	}
}

//============================================================================
// 通常状態へ
//============================================================================
void CPlayer_State_Slash::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}

//============================================================================
// ダメージ状態へ
//============================================================================
void CPlayer_State_Slash::To_Damage()
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
// 重力の補正
//============================================================================
void CPlayer_State_Slash::AdjustGravity()
{

}

//============================================================================
// 斬撃当たり判定
//============================================================================
bool CPlayer_State_Slash::SlashHitCheck()
{
#if 0
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	while (pObj != nullptr)
	{
		// キャラクタークラスにダウンキャスト
		CCharacter* pCharacter = CObject::DownCast<CCharacter>(pObj);

		// キャスト失敗で
		if (!pCharacter)
		{
			pObj = pObj->GetNext();
			continue;
		}

		// 攻撃と敵キャラクターの当たり判定

		// ダメージを与える
		pCharacter->SetDamage(-1);

		pObj = pObj->GetNext();
	}
#else // 現段階のみ

	// ボスが存在していなければこのメソッドを無視
	if (!CObject::FindSpecificObject(CObject::TYPE::BOSS))
		return false;

	// ボスの円柱バウンディングを取得
	CBoss* pBoss = utility::DownCast<CBoss, CObject>(CObject::FindSpecificObject(CObject::TYPE::BOSS));

	// 斬撃バウンディングをコピー
	const CBounding_Sphere* pOther = m_pBndSlash.get();

	// 斬撃バウンディングとボスの円柱バウンディングで当たり判定
	if (collision::HitCylinderToSphere(pBoss->GetBndCylinder(), pOther))
		pBoss->SetDamage(-1);

#endif

	return false;
}