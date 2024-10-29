//============================================================================
// 
// プレイヤー [player.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player.h"

// インプット取得
#include "manager.h"

// フィールドマネージャー
#include "field_manager.h"

// デバッグ表示用
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace player;
using namespace player_state_manager;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CPlayer::CPlayer() :
	CObject_X{ static_cast<int>(CObject::LAYER::MIDDLE) },
	m_pPlayerStateManager{ nullptr },
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
CPlayer::~CPlayer()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CPlayer::Init()
{
	// プレイヤーステートマネージャーの生成
	if (!m_pPlayerStateManager)
	{
		m_pPlayerStateManager = CPlayer_State_Manager::Create();
	}

	// 初期方角を設定
	m_fDirection = D3DX_PI * -0.5f;

	// Xオブジェクトの初期設定
	if (FAILED(CObject_X::Init()))
	{
		assert(false && "プレイヤークラスの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CPlayer::Uninit()
{
	// プレイヤーステートマネージャーの破棄
	if (m_pPlayerStateManager)
	{
		// 解放
		m_pPlayerStateManager->Release();
		
		// ポインタを初期化
		m_pPlayerStateManager = nullptr;
	}

	// 基底クラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer::Update()
{	
	// 方角の補正
	CUtility::AdjustDirection(m_fDirection);
	
	// プレイヤーステートマネージャーの更新
	m_pPlayerStateManager->Update();

	// 目標値への補正
	AdjustToTarget();

	// 基底クラスの更新処理
	CObject_X::Update();
	
#ifdef _DEBUG

	if (CManager::GetKeyboard()->GetTrigger(DIK_1))
	{
		m_nLife > 0 ? m_nLife += -1 : m_nLife = 0;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
	{
		m_nLife < MAX_LIFE ? m_nLife += 1 : m_nLife = MAX_LIFE;
	}

	CRenderer::GetInstance()->SetDebugString("プレイヤー体力" + to_string(m_nLife));
	CRenderer::GetInstance()->SetDebugString("プレイヤー方角" + to_string(m_fDirection * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("プレイヤー座標 : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::GetInstance()->SetDebugString("プレイヤー移動速度" + to_string(m_fMoveSpeed));
	CRenderer::GetInstance()->SetDebugString("プレイヤー向き　　 : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("目標プレイヤー向き : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));

#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CPlayer::Draw()
{
	// 基底クラスの描画処理
	CObject_X::Draw();
}

//============================================================================
// 体力を取得
//============================================================================
const int& CPlayer::GetLife() const
{
	return m_nLife;
}

//============================================================================
// 体力を設定
//============================================================================
void CPlayer::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================
// 目標座標を取得
//============================================================================
const Vec3& CPlayer::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// 目標座標を設定
//============================================================================
void CPlayer::SetPosTarget(Vec3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// 目標向きを取得
//============================================================================
const Vec3& CPlayer::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// 目標向きを設定
//============================================================================
void CPlayer::SetRotTarget(Vec3 RotTarget)
{
	m_RotTarget = RotTarget;
}


//============================================================================
// 移動速度を取得
//============================================================================
const float& CPlayer::GetMoveSpeed() const
{
	return m_fMoveSpeed;
}

//============================================================================
// 移動速度を設定
//============================================================================
void CPlayer::SetMoveSpeed(float fMoveSpeed)
{
	m_fMoveSpeed = fMoveSpeed;
}

//============================================================================
// 方角を取得
//============================================================================
const float& CPlayer::GetDirection() const
{
	return m_fDirection;
}

//============================================================================
// 方角を設定
//============================================================================
void CPlayer::SetDirection(float fDirection)
{
	m_fDirection = fDirection;
}

//============================================================================
// 生成
//============================================================================
CPlayer* CPlayer::Create()
{
	// インスタンスを生成
	CPlayer* pNewInstance = DBG_NEW CPlayer();

	// タイプを設定
	pNewInstance->SetType(TYPE::PLAYER);
	
	// 初期設定
	pNewInstance->Init();

	// モデルを設定
	pNewInstance->BindModel(CModel_X_Manager::TYPE::SAMUS);

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 目標値への補正
//============================================================================
void CPlayer::AdjustToTarget()
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