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

#include "collision.h"
#include "player.h"
#include "player_state_damage.h"
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
CEnemy::CEnemy() :
	CCharacter{},
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder(this) }
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
	// 補正強度を設定
	SetCorrectCoef(0.5f);

	// キャラクターの初期設定
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
	// 当たり判定の中心点を設定
	m_pBndCylinder->SetCenterPos(GetPos());

	// 方角の取得
	float fNewDirection = GetDirection();
	utility::AdjustDirection(fNewDirection);
	fNewDirection += -DEFAULT_MOVE_SPEED * 3.0f;
	SetDirection(fNewDirection);

	// 目標向きを移動方向に
	AutoSetRotTarget();

	// 方角から自動で座標を設定
	AutoSetPosTarget();

	// プレイヤーへの攻撃
	AttackOnPlayer();

	// キャラクタークラスの更新処理
	CCharacter::Update();

#if 1
#ifdef _DEBUG
	
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("エネミー体力　　 : " + to_string(GetLife()));
	CRenderer::SetDebugString("エネミー方角　　 : " + to_string(GetDirection() * (180 / D3DX_PI)));
	CRenderer::SetDebugString("エネミー座標　　 : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::SetDebugString("エネミー速度　　 : " + to_string(GetMoveSpeed()));
	CRenderer::SetDebugString("エネミー向き　　 : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("エネミー目標向き : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
#endif // _DEBUG
#endif
}

//============================================================================
// 描画処理
//============================================================================
void CEnemy::Draw()
{
	// キャラクターの描画処理
	CCharacter::Draw();
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
// 生成
//============================================================================
CEnemy* CEnemy::Create()
{
	// インスタンスを生成
	CEnemy* pNewInstance = DBG_NEW CEnemy();

	// タイプを設定
	pNewInstance->SetType(TYPE::ENEMY);

	// 初期設定
	pNewInstance->Init();

	// モデルを設定
	pNewInstance->BindModel(CX_Manager::TYPE::ENEMY);

	// 半径を設定
	float fRad = 0.0f;
	pNewInstance->GetModel()->Size.x > pNewInstance->GetModel()->Size.z ? fRad = pNewInstance->GetModel()->Size.x : fRad = pNewInstance->GetModel()->Size.z;
	pNewInstance->m_pBndCylinder->SetRadius(fRad);

	// 高さを設定
	pNewInstance->m_pBndCylinder->SetHeight(pNewInstance->GetModel()->Size.y);

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// プレイヤーへ攻撃
//============================================================================
void CEnemy::AttackOnPlayer()
{		
	// プレイヤーのポインタを作成
	CPlayer* pPlayer = nullptr;

	// プレイヤーを検索
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		// プレイヤーオブジェクトを保持
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// プレイヤーの持つ円柱範囲内に侵入していたら
		if (collision::HitCylinderToCylinder(m_pBndCylinder, pPlayer->GetBndCylinder()))
		{
			// 既にダメージ状態の場合は処理を終了
			/* この条件式は判定表示の色を戻すためにここにありますが、本来は当たり判定前にこちらを判定します */
			if (typeid(*pPlayer->GetNowState()) == typeid(CPlayer_State_Damage))
			{
				return;
			}

			/* 判定表示を赤色に */
			m_pBndCylinder->SetCol({ 1.0f, 0.0f, 0.0f, 0.5f });

			// ダメージを与える
			pPlayer->SetDamage(-1);
		}
		else
		{
			/* 判定表示を通常色に戻す */
			m_pBndCylinder->SetCol({ 1.0f, 1.0f, 1.0f, 0.5f });
		}
	}
	else
	{
		assert(false && "プレイヤーの検索結果がありませんでした");
	}
}