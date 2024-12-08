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
#include "object_X.h"
#include "player_state_default.h"
#include "player_state_damage.h"
#include "collision.h"
#include "constellation.h"

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
	m_SlashType{ SLASH_TYPE::LEFT },
	m_pSlashModel{ CObject_X::Create(CX_Manager::TYPE::PLAHEAD) },
	m_pBndSlash{ std::make_unique<CBounding_Sphere>() }
{
	// 斬撃モデルの座標を設定 (生成直後の座標で1f描画されてしまうため)
	m_pSlashModel->SetPos({ 0.0f, -1000.0f, 0.0f });

	// 斬撃のバウンディングのサイズを設定
	m_pBndSlash->SetRadius(8.0f);

	// 斬撃モーションを再生
	m_pCharacter->SetNowMotion(5);
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

	// 斬撃モデルを破棄
	m_pSlashModel->SetRelease();
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
		m_pCharacter->GetPos().x + -sinf(m_pCharacter->GetRot().y) * 15.0f,
		m_pCharacter->GetPos().y,
		m_pCharacter->GetPos().z + -cosf(m_pCharacter->GetRot().y) * 15.0f
	};

	// 斬撃モデルを回転
	Vec3 NewRot = m_pSlashModel->GetRot();
	NewRot.y += 1.0f;
	m_pSlashModel->SetRot(NewRot);

	// 斬撃モデルの座標を設定
	m_pSlashModel->SetPos(PlayerFacing);

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