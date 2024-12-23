//============================================================================
// 
// エネミー [enemy.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "enemy.h"
#include "bounding_cylinder.h"

#include "collision.h"
#include "object_parts.h"
#include "field_manager.h"
#include "player.h"
#include "player_state_damage.h"
#include "block.h"
#include "renderer.h"

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
CEnemy::CEnemy(LAYER Layer) :
	CCharacter{ Layer },
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() }
{

}

//============================================================================
// デストラクタ
//============================================================================
CEnemy::~CEnemy()
{
	// バウンディングシリンダーの破棄
	if (m_pBndCylinder != nullptr)
	{
		// メモリを解放
		delete m_pBndCylinder;

		// ポインタを初期化
		m_pBndCylinder = nullptr;
	}
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CEnemy::Init()
{
	// キャラクタークラスの初期設定
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "エネミークラスの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CEnemy::Uninit()
{
	// キャラクタークラスの終了処理
	CCharacter::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CEnemy::Update()
{
	// 高さの補正
	AdjustHeight();

	// 当たり判定の中心点を設定
	m_pBndCylinder->SetCenterPos(GetPos());

	// キャラクタークラスの更新処理
	CCharacter::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CEnemy::Draw()
{

}

//============================================================================
// 半径を取得
//============================================================================
float CEnemy::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// 高さを取得
//============================================================================
float CEnemy::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// 円柱バウンディングを取得
//============================================================================
const CBounding_Cylinder* const CEnemy::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// ダメージを受ける
//============================================================================
void CEnemy::SetDamage(int nDamage)
{
	// ダメージ量分、体力を変動
	int nNewLife = GetLife();
	nNewLife += nDamage;
	SetLife(nNewLife);
}

//============================================================================
// 初期方に対し情報を整える
//============================================================================
void CEnemy::AdjustInitDirection(float fHeight)
{
	// 方角をコピー
	float fDirection = GetDirection();

	// 目標方角を揃える
	SetDirectionTarget(fDirection);

	// 向きを方角に合わせる
	Vec3 NewRot = { 0.0f, fDirection, 0.0f };
	SetRot(NewRot);
	SetRotTarget(NewRot);

	// 座標を方角・フィールド範囲に合わせる
	Vec3 NewPos = utility::DirectionConvertVec3(fDirection, fHeight, CField_Manager::FIELD_RADIUS);
	SetPos(NewPos);
	SetPosTarget(NewPos);
}

//============================================================================
// 
// prtectedメンバ
// 
//============================================================================

//============================================================================
// プレイヤーを検索
//============================================================================
CPlayer* const CEnemy::FindPlayer() const
{
	// プレイヤータイプのオブジェクトを検索
	CObject* const pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// オブジェクトが見つからければ終了
	if (pObj == nullptr)
		return nullptr;

	// プレイヤークラスにダウンキャスト
	CPlayer* const pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	return pPlayer;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 高さを補正
//============================================================================
void CEnemy::AdjustHeight()
{
	// 目標座標をY軸の加速度分変動する
	Vec3 NewPosTarget = GetPosTarget();
	NewPosTarget.y += GetVelY();
	SetPosTarget(NewPosTarget);

	// 高さの目標に下限を設定
	if (GetPosTarget().y < m_pBndCylinder->GetHeight())
	{
		// 目標座標を下限に固定
		NewPosTarget.y = m_pBndCylinder->GetHeight();
		SetPosTarget(NewPosTarget);

		// Y軸方向の加速度を初期化
		SetVelY(0.0f);
	}
}

//============================================================================
// バウンディングサイズの設定
//============================================================================
void CEnemy::SetBndSize(float fRadius, float fHeight)
{
	// 半径の設定
	m_pBndCylinder->SetRadius(fRadius);

	// 高さの設定
	m_pBndCylinder->SetHeight(fHeight);
}

//============================================================================
// 衝突検出
//============================================================================
bool CEnemy::HitCheck()
{
	// プレイヤーを取得
	CPlayer* const pPlayer = FindPlayer();

	// プレイヤーの持つ円柱範囲内に侵入していたら衝突
	if (collision::HitCylinderToCylinder(m_pBndCylinder, pPlayer->GetBndCylinder()))
	{
		// 原点からキャラクターのいる場所への方向ベクトルを作成
		Vec3 CharacterPosVec = GetPos() - VEC3_INIT;
		D3DXVec3Normalize(&CharacterPosVec, &CharacterPosVec);

		// 原点からプレイヤーのいる場所へのベクトルを作成
		Vec3 PlayerVec = pPlayer->GetPos() - VEC3_INIT;
		D3DXVec3Normalize(&PlayerVec, &PlayerVec);

		// 2本の方向ベクトルの外積を作成
		float fCross = (CharacterPosVec.x * PlayerVec.z) - (CharacterPosVec.z * PlayerVec.x);
		
		// 半径に応じて衝撃量を作成
		float fImpact = GetRadius() * 0.01f;

		// お互いの位置関係に合わせて吹き飛ばす方角を変更
		if (fCross < 0.0f) fImpact *= -1.0f;

		// 1ダメージと衝撃力を与える
		pPlayer->SetDamage(-1, fImpact);

		// 衝突した
		return true;
	}

	// 衝突していない
	return false;
}