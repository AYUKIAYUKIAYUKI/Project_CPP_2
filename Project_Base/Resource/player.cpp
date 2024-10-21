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

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CPlayer::CPlayer() :
	CObject_X(static_cast<int>(CObject::LAYER::MIDDLE)),
	m_PosTarget(VEC3_INIT),
	m_fMoveSpeed(DEFAULT_MOVE_SPEED),
	m_fDirection(0.0f)
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
	// 初期方角を設定
	//m_fDirection = D3DX_PI * -0.5f;

	// 基底クラスの初期設定
	HRESULT hr = CObject_X::Init();

	if (FAILED(hr))
	{
		assert(false && "プレイヤークラスの初期設定に失敗");
	}

	return hr;
}

//============================================================================
// 終了処理
//============================================================================
void CPlayer::Uninit()
{
	// 基底クラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer::Update()
{
	// 方角の補正
	/* 必ず処理の先頭に */
	CUtility::AdjustDirection(m_fDirection);

	// 操作
	Control();

	// 基底クラスの更新処理
	CObject_X::Update();
	
#ifdef _DEBUG
	CRenderer::GetInstance()->SetDebugString("プレイヤー座標 : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::GetInstance()->SetDebugString("プレイヤー向き : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("プレイヤー方角" + to_string(m_fDirection * (180 / D3DX_PI)));
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
// 操作
//============================================================================
void CPlayer::Control()
{
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace field_manager;

	// インプット系取得
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();	// キーボード
	CInputPad* pPad = CManager::GetPad();					// パッド

	// X軸の入力
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
	{
		m_fDirection += -m_fMoveSpeed;
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{
		m_fDirection += m_fMoveSpeed;
	}

	// 座標を反映
	Vec3 NewPos = VEC3_INIT;
	NewPos.x = cosf(m_fDirection) * CField_Manager::FIELD_RADIUS;
	NewPos.z = sinf(m_fDirection) * CField_Manager::FIELD_RADIUS;
	SetPos(NewPos);

	// 向きを反映
	Vec3 NewRot = VEC3_INIT;
	//NewRot.y = atan2f(NewRot.x - NewPos.x, NewRot.z - NewPos.z);
	SetRot(NewRot);
}