//============================================================================
// 
// ストーン [stone.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "stone.h"
#include "bounding_sphere.h"

#include "renderer.h"
#include "sound.h"
#include "field_manager.h"
#include "collision.h"
#include "enemy.h"

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
CStone::CStone() :
	CObject_X{ LAYER::DEFAULT },
	m_pBndSphere{ DBG_NEW CBounding_Sphere() },
	m_Accel{ VEC3_INIT },
	m_nDuration{ 0 }
{

}

//============================================================================
// デストラクタ
//============================================================================
CStone::~CStone()
{
	// 球バウンディングの破棄
	if (m_pBndSphere != nullptr)
	{
		// メモリを解放
		delete m_pBndSphere;

		// ポインタを初期化
		m_pBndSphere = nullptr;
	}
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CStone::Init()
{
	// Xオブジェクトクラスの初期設定
	if (FAILED(CObject_X::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CStone::Uninit()
{
	// Xオブジェクトクラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CStone::Update()
{
	++m_nDuration;

	// 移動
	if (m_nDuration > WAIT_DURATION)
	{
		// 座標を取得
		Vec3 Pos = GetPos();
		Pos += m_Accel;
		SetPos(Pos);
	}

	// 球バウンディングに中心点セット
	m_pBndSphere->SetCenterPos(GetPos());

	// 敵との当たり判定
	HitCheckEnemy();

	if (m_nDuration > WAIT_DURATION)
	{
		{ // 縮小

			// スケールを取得
			Vec3 Scale = GetScale();

			// 減少量
			float fAdjust = 0.0075f;

			// スケール減少
			Scale = {
				Scale.x - fAdjust,
				Scale.y - fAdjust,
				Scale.z - fAdjust
			};

			// 半径を更新
			SetRadius(Scale.x * 7.5f);

			// 下限を設定
			if (Scale.x < 0.15f)
			{
				Scale = VEC3_INIT;

				SetRelease();
			}

			SetScale(Scale);
		}

		{ // 回転

			float fCoef = fabsf(utility::GetRandomValue<float>() * 0.005f);

			// 向きを取得
			Vec3 Rot = GetRot();
			Rot.x += fCoef;
			Rot.y += fCoef;
			SetRot(Rot);
		}
	}

	// Xオブジェクトクラスの更新処理
	CObject_X::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CStone::Draw()
{
	// Xオブジェクトクラスの描画処理
	CObject_X::Draw();
}

//============================================================================
// 半径を取得
//============================================================================
float CStone::GetRadius() const
{
	return m_pBndSphere->GetRadius();
}

//============================================================================
// 半径を設定
//============================================================================
void CStone::SetRadius(float fRadius)
{
	m_pBndSphere->SetRadius(fRadius);
}

//============================================================================
// 生成
//============================================================================
CStone* CStone::Create(D3DXVECTOR3 InitPos, D3DXVECTOR3 Accel)
{
	// インスタンスを生成
	CStone* pNewInstance = DBG_NEW CStone();

	// タイプを設定
	pNewInstance->SetType(TYPE::NONE);

	// 初期設定
	pNewInstance->Init();

	// モデルをセット
	pNewInstance->BindModel(CX_Manager::TYPE::POWERSTONE);

	// 初期スケール
	pNewInstance->SetScale({ 0.4f, 0.4f, 0.4f });

	// 初期向きを設定
	float fCoef = fabsf(utility::GetRandomValue<float>());
	Vec3 Rot = pNewInstance->GetRot();
	Rot.x += fCoef;
	Rot.y += fCoef;
	pNewInstance->SetRot(Rot);

	// 初期位置を設定
	pNewInstance->SetPos(InitPos);

	// 推進力をセット
	pNewInstance->m_Accel = Accel;

	return pNewInstance;
}

//============================================================================
//
// privateメンバ
// 
//============================================================================

//============================================================================
// エネミーとの接触を検出
//============================================================================
void CStone::HitCheckEnemy()
{
	// エネミータイプのオブジェクトを探す
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ENEMY);

	// 斬撃バウンディングをコピー
	const CBounding_Sphere* pSlash = m_pBndSphere;

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::ENEMY)
		{ // エネミータイプなら

			// エネミークラスにキャスト
			CEnemy* pEnemy = CObject::DownCast<CEnemy>(pObj);

			// 攻撃とエネミーでの当たり判定
			if (collision::HitCylinderToSphere(pEnemy->CEnemy::GetBndCylinder(), pSlash))
			{
				// ダメージを与える
				pEnemy->SetDamage(-1);

				// 自身を破棄予約
				SetRelease();
			}
		}

		// 次のオブジェクトへ
		pObj = pObj->GetNext();
	}
}