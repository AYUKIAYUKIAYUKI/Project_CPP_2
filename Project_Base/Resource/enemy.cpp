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

#include "field_manager.h"
#include "player.h"

#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;
using namespace field_manager;
using namespace player;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CEnemy::CEnemy() :
	CObject_X{ static_cast<int>(CObject::LAYER::MIDDLE) },
	m_fDirection{ 0.0f },
	m_PosTarget{ VEC3_INIT },
	m_fMoveSpeed{ DEFAULT_MOVE_SPEED },
	m_RotTarget{ VEC3_INIT },
	m_nLife{ MAX_LIFE }
{

}

//============================================================================
// デストラクタ
//============================================================================
CEnemy::~CEnemy()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CEnemy::Init()
{
	// 初期方角を設定
	m_fDirection = D3DX_PI * 0.5f;

	// Xオブジェクトの初期設定
	if (FAILED(CObject_X::Init()))
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
	// 基底クラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CEnemy::Update()
{
	// 方角の補正
	CUtility::AdjustDirection(m_fDirection);

	// 方角の変動
	m_fDirection += DEFAULT_MOVE_SPEED;

	// 向きの反映
	Vec3 NewRotTarget = GetRotTarget();					// 目標向きを取得
	const Vec3& MoveVec = GetPosTarget() - GetPos();	// 移動方向のベクトルを作成
	NewRotTarget.y = atan2f(-MoveVec.x, -MoveVec.z);	// 目標向きを移動方向に
	SetRotTarget(NewRotTarget);							// 目標向きを反映

	// 方角の反映
	Vec3 NewPosTarget = VEC3_INIT;										// 新規目標座標を作成
	NewPosTarget.x = cosf(m_fDirection) * CField_Manager::FIELD_RADIUS;	// X方向の座標を設定
	NewPosTarget.z = sinf(m_fDirection) * CField_Manager::FIELD_RADIUS;	// Z方向の座標を設定
	SetPosTarget(NewPosTarget);											// 目標座標を反映

	// 目標値への補正
	AdjustToTarget();

	// プレイヤーへの攻撃
	AttackOnPlayer();

	// 基底クラスの更新処理
	CObject_X::Update();

#ifdef _DEBUG

	CRenderer::GetInstance()->SetDebugString("エネミー体力　　 : " + to_string(m_nLife));
	CRenderer::GetInstance()->SetDebugString("エネミー方角　　 : " + to_string(m_fDirection * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("エネミー座標　　 : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::GetInstance()->SetDebugString("エネミー速度　　 : " + to_string(m_fMoveSpeed));
	CRenderer::GetInstance()->SetDebugString("エネミー向き　　 : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("エネミー目標向き : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));

#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CEnemy::Draw()
{
	// 基底クラスの描画処理
	CObject_X::Draw();
}

//============================================================================
// 体力を取得
//============================================================================
const int& CEnemy::GetLife() const
{
	return m_nLife;
}

//============================================================================
// 体力を設定
//============================================================================
void CEnemy::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================
// 目標座標を取得
//============================================================================
const Vec3& CEnemy::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// 目標座標を設定
//============================================================================
void CEnemy::SetPosTarget(Vec3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// 目標向きを取得
//============================================================================
const Vec3& CEnemy::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// 目標向きを設定
//============================================================================
void CEnemy::SetRotTarget(Vec3 RotTarget)
{
	m_RotTarget = RotTarget;
}


//============================================================================
// 移動速度を取得
//============================================================================
const float& CEnemy::GetMoveSpeed() const
{
	return m_fMoveSpeed;
}

//============================================================================
// 移動速度を設定
//============================================================================
void CEnemy::SetMoveSpeed(float fMoveSpeed)
{
	m_fMoveSpeed = fMoveSpeed;
}

//============================================================================
// 方角を取得
//============================================================================
const float& CEnemy::GetDirection() const
{
	return m_fDirection;
}

//============================================================================
// 方角を設定
//============================================================================
void CEnemy::SetDirection(float fDirection)
{
	m_fDirection = fDirection;
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
	pNewInstance->BindModel(CModel_X_Manager::TYPE::ENEMY);

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
	CPlayer* pPlayer = nullptr;

	// プレイヤーを検索
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		// プレイヤーへのポインタを保持
		pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// プレイヤーの持つ円柱範囲内に侵入していたら
		if (CUtility::CylinderAndPoint(pPlayer->GetPos(), 10.0f, 100.0f, GetPos()))
		{
			// 体力を減少
			int nLife = pPlayer->GetLife();
			nLife--;
			pPlayer->SetLife(nLife);
		}
	}
	else
	{
		assert(false && "プレイヤーの検索結果がありませんでした");
	}
}

//============================================================================
// 目標値への補正
//============================================================================
void CEnemy::AdjustToTarget()
{
	// 目標座標へ移動
	Vec3 NowPos = GetPos();
	NowPos += (m_PosTarget - NowPos) * COEF_ADJUST;
	SetPos(NowPos);

	// 目標向きへ補正
	Vec3 NowRot = GetRot();
	CUtility::AdjustAngle(NowRot.y, m_RotTarget.y);	// 角度の差を補正
	NowRot += (m_RotTarget - NowRot) * COEF_ADJUST;
	SetRot(NowRot);
}