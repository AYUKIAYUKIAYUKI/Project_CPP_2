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
#include "bright.h"
#include "player_state_default.h"
#include "player_state_damage.h"
#include "manager.h"
#include "sound.h"
#include "collision.h"
#include "field_manager.h"

#include "constellation.h"
#include "enemy.h"
#include "monster.h"
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
	m_SlashType{ SLASH_TYPE::LEFT },
	m_pBndSlash{ std::make_unique<CBounding_Sphere>() }
{
	// アクションデータのジャンプした回数をインクリメント
	CField_Manager::GetInstance()->IncrementCntSlash();

	// 斬撃のバウンディングのサイズを設定
	m_pBndSlash->SetRadius(5.0f);

	// 斬撃モーションを再生
	m_pCharacter->SetNowMotion(5);

	// 斬撃SEを鳴らす
	CSound::GetInstance()->Play(CSound::LABEL::SLASH);
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

	// プレイヤーの向いている方向のベクトルを作成
	Vec3 PlayerFacing =
	{
		m_pCharacter->GetPos().x + -sinf(m_pCharacter->GetRot().y) * 11.0f,
		m_pCharacter->GetPos().y + 4.0f,
		m_pCharacter->GetPos().z + -cosf(m_pCharacter->GetRot().y) * 11.0f
	};

	// 反動の衝撃量を作成
	float fImpact = -0.001f;

	// 向いている方向を左右で判別
	bool bFaceSide = m_pCharacter->CheckFacingSide();

	// お互いの位置関係に合わせて吹き飛ばす方角を変更
	if (!bFaceSide) fImpact *= -1.0f;

	// プレイヤーが反動で後退
	float fNewDirectionTarget = m_pCharacter->GetDirectionTarget();
	fNewDirectionTarget += fImpact;
	m_pCharacter->SetDirectionTarget(fNewDirectionTarget);

	// 斬撃バウンディングの中心点を設定
	m_pBndSlash->SetCenterPos(PlayerFacing);

	// 星座エフェクトを発生
	CConstellation::GenerateSpread(PlayerFacing);

	// 衝突検出
	SlashHitCheck();

	// 重力加速
	m_pCharacter->SetVelY(m_pCharacter->GetVelY() + CField_Manager::FIELD_GRAVITY * 0.25f);

	// 斬撃モーションが停止したら
	if (m_pCharacter->GetStopState())
	{
		// 通常モーションへ
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
	// デフォルトオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// オブジェクトリスト検索
	while (pObj != nullptr)
	{
		// オブジェクトのタイプに応じて処理分岐
		if (pObj->GetType() == CObject::TYPE::ENEMY)
		{ // エネミータイプなら

			// エネミークラスにキャスト
			CEnemy* pEnemy = CObject::DownCast<CEnemy>(pObj);

			// 斬撃バウンディングをコピー
			const CBounding_Sphere* pSlash = m_pBndSlash.get();

			// 攻撃とエネミーでの当たり判定
			if (collision::HitCylinderToSphere(pEnemy->CEnemy::GetBndCylinder(), pSlash))
			{
				// ダメージを与える
				pEnemy->SetDamage(-1);
			}
		}
		else if (pObj->GetType() == CObject::TYPE::BOSS)
		{ // ボスタイプなら

			// ボスクラスにキャスト
			CBoss* pBoss = CObject::DownCast<CBoss>(pObj);

			// 斬撃バウンディングをコピー
			const CBounding_Sphere* pSlash = m_pBndSlash.get();

			// 攻撃とボスでの当たり判定
			if (collision::HitCylinderToSphere(pBoss->GetBndCylinder(), pSlash))
			{
				// ダメージを与える
				pBoss->SetDamage(-1);
			}
		}

		// 次のオブジェクトへ
		pObj = pObj->GetNext();
	}

	return false;
}