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
#include "stone.h"

// アクションデータ用
#include "field_builder.h"

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
	// アクションデータの斬撃した回数をインクリメント
	CField_Manager::GetInstance()->GetFieldBuilder()->IncrementCntSlash();

	// 斬撃のバウンディングのサイズを設定
	m_pBndSlash->SetRadius(6.0f);

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

	// 入力取得用
	CInputKeyboard* pKeyBoard = CManager::GetManager()->GetKeyboard();
	CInputPad* pPad = CManager::GetManager()->GetPad();

	// ①反動の衝撃量を作成
	float fImpact = -0.001f;

	// ②左右どちらを向いているか判別
	bool bFaceSide = m_pCharacter->CheckFacingSide();

	// ③反動を付ける方向を決定
	if (!bFaceSide)
	{
		fImpact *= -1.0f;
	}

	// ④入力方向に応じて反動量を少し変化
	if (bFaceSide && pKeyBoard->GetPress(DIK_A))
	{ // 右向きで左入力中

		// 反動増大
		fImpact *= 1.5f;
	}
	else if (bFaceSide && pKeyBoard->GetPress(DIK_D))
	{ // 右向きで右入力中

		// 反動低減
		fImpact *= 0.25f;
	}
	else if (!bFaceSide && pKeyBoard->GetPress(DIK_A))
	{ // 左向きで左入力中

		// 反動低減
		fImpact *= 0.25f;
	}
	else if (!bFaceSide && pKeyBoard->GetPress(DIK_D))
	{ // 左向きで右入力中

		// 反動増大
		fImpact *= 1.5f;
	}

	// ⑤反動で後退
	float fNewDirectionTarget = m_pCharacter->GetDirectionTarget();
	fNewDirectionTarget += fImpact;
	m_pCharacter->SetDirectionTarget(fNewDirectionTarget);

	// 向いている方向のベクトルを作成
	Vec3 PlayerFacing =
	{
		m_pCharacter->GetPos().x + -sinf(m_pCharacter->GetRot().y) * 12.0f,
		m_pCharacter->GetPos().y + 3.5f,
		m_pCharacter->GetPos().z + -cosf(m_pCharacter->GetRot().y) * 12.0f
	};

	// 斬撃バウンディングの中心点を設定
	m_pBndSlash->SetCenterPos(PlayerFacing);

	// キャラクターのポインタをプレイヤークラスにダウンキャスト
	CPlayer* pPlayer = utility::DownCast<CPlayer, CCharacter>(m_pCharacter);

	// 弾岩が有効化されていたら
	if (pPlayer->IsEnabledPowerStone())
	{
		// 初期位置をずらす
		Vec3 InitPos = m_pCharacter->GetPos();
		InitPos += {
			utility::GetRandomValue<float>() * 0.075f,
			utility::GetRandomValue<float>() * 0.05f,
			utility::GetRandomValue<float>() * 0.075f
		};

		// 飛ばす方向を攻撃方向にする
		Vec3 Way = PlayerFacing - m_pCharacter->GetPos();
		Way.y = 0.0f;

		CStone::Create(InitPos, Way * 0.15f);
	}

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