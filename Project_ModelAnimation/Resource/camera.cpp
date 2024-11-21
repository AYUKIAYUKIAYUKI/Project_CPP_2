//============================================================================
// 
// カメラ [camera.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "camera.h"

// インプット取得用
#include "manager.h"

// デバイス取得用
#include "renderer.h"

// 追従対象取得用
#include "object.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CCamera::CCamera() :
	m_Pos{ VEC3_INIT },
	m_PosTarget{ VEC3_INIT },
	m_PosV{ VEC3_INIT },
	m_PosTargetV{ VEC3_INIT },
	m_PosR{ VEC3_INIT },
	m_PosTargetR{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_fDistance{ 0.0f },
	m_VecU{ 0.0f, 1.0f, 0.0f },
	m_fAdjust{ 0.0f }
{
	// 行列を初期化
	D3DXMatrixIdentity(&m_MtxProjection);	// プロジェクション行列
	D3DXMatrixIdentity(&m_MtxView);			// ビュー行列
}

//============================================================================
// デストラクタ
//============================================================================
CCamera::~CCamera()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CCamera::Init()
{
	// 間距離を設定
	m_fDistance = 40.0f;

	// 俯瞰度合いを設定
	m_fAdjust = 0.0f;

	return S_OK;
}

//============================================================================
// 更新処理
//============================================================================
void CCamera::Update()
{
	// ビューモード分岐
	BranchMode();

	// 回転
	Rotation();

	// 移動
	Translation();

	// 視点座標を計算
	CalcPosV();

	// 注視点座標を計算
	CalcPosR();

#if 0
#ifdef _DEBUG

	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("カメラ座標　　 : " + to_string(m_Pos.x) + " :  " + to_string(m_Pos.y) + " : " + to_string(m_Pos.z));
	CRenderer::SetDebugString("目標カメラ座標 : " + to_string(m_PosTarget.x) + " :  " + to_string(m_PosTarget.y) + " : " + to_string(m_PosTarget.z));
	CRenderer::SetDebugString("カメラ向き　　 : " + to_string(m_Rot.x * (180 / D3DX_PI)) + " :  " + to_string(m_Rot.y * (180 / D3DX_PI)) + " : " + to_string(m_Rot.z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("目標カメラ向き : " + to_string(m_RotTarget.x * (180 / D3DX_PI)) + " :  " + to_string(m_RotTarget.y * (180 / D3DX_PI)) + " : " + to_string(m_RotTarget.z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("カメラ間距離 　: " + to_string(m_fDistance));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");

#endif // _DEBUG
#endif
}

//============================================================================
// カメラをセット
//============================================================================
void CCamera::SetCamera()
{
#if 0	// バッファのクリアはレンダラーに委ねる

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 画面バッファクリア
	pDev->Clear(0,
		nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

#endif

	// プロジェクション行列を計算
	CalcMtxProjection();

	// ビュー行列を計算
	CalcMtxView();
}

//============================================================================
// 座標を取得
//============================================================================
const D3DXVECTOR3& CCamera::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標を設定
//============================================================================
void CCamera::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 目標座標を取得
//============================================================================
const D3DXVECTOR3& CCamera::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// 目標座標を設定
//============================================================================
void CCamera::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// 向きを取得
//============================================================================
const D3DXVECTOR3& CCamera::GetRot() const
{
	return m_Rot;
}

//============================================================================
// 向きを設定
//============================================================================
void CCamera::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// 目標向きを取得
//============================================================================
const D3DXVECTOR3& CCamera::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// 目標向きを設定
//============================================================================
void CCamera::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// 間距離を取得
//============================================================================
const float& CCamera::GetDistance() const
{
	return m_fDistance;
}

//============================================================================
// 間距離を設定
//============================================================================
void CCamera::SetDistance(float fDistance)
{
	m_fDistance = fDistance;
}

//============================================================================
// モード分岐
//============================================================================
void CCamera::BranchMode()
{
	// カメラ操作
	Control();
}

//============================================================================
// カメラ操作
//============================================================================
void CCamera::Control()
{
	if (CManager::GetMouse()->GetPress(0))
	{ 
		// 移動上下
		m_PosTarget.y += CManager::GetMouse()->GetMouseMove().y * 0.025f;
	}
	else if (CManager::GetMouse()->GetPress(1))
	{ 
		// 向き上下左右
		m_RotTarget.x += CManager::GetMouse()->GetMouseMove().y * -0.01f;
		m_RotTarget.y += CManager::GetMouse()->GetMouseMove().x * 0.01f;
	}

	// 距離感リセット
	if (CManager::GetMouse()->GetTrigger(2))
	{
		m_fDistance = 40.0f;
	}

	// ズームイン / アウト
	m_fDistance += -CManager::GetMouse()->GetWheelScroll() * 0.05f;

	// ズームインの下限を設定
	if (m_fDistance < 10.0f)
	{
		m_fDistance = 10.0f;
	}
}

//============================================================================
// 回転
//============================================================================
void CCamera::Rotation()
{
	// ヨー角の範囲を制限
	utility::AdjustDirection(m_RotTarget.y, m_Rot.y);

	// ピッチ角の範囲を制限
	RestrictPitch();

	// 目標向きへ補正
	m_Rot += (m_RotTarget - m_Rot) * COEF_ADJUST;
}

//============================================================================
// ピッチ角の範囲を制限
//============================================================================
void CCamera::RestrictPitch()
{
	// 0.0f ～ 0.5f
	float fCoeff = 0.48f;

	if (m_RotTarget.x > D3DX_PI * fCoeff)
	{
		m_RotTarget.x = D3DX_PI * fCoeff;
	}
	else if (m_RotTarget.x < -D3DX_PI * fCoeff)
	{
		m_RotTarget.x = -D3DX_PI * fCoeff;
	}
}

//============================================================================
// 移動
//============================================================================
void CCamera::Translation()
{
	// 目標座標へ補正
	m_Pos += (m_PosTarget - m_Pos) * COEF_ADJUST;
}

//============================================================================
// 視点座標を計算
//============================================================================
void CCamera::CalcPosV()
{
	// 視点は対象物の後方に
	m_PosV =
	{
		m_Pos.x - (sinf(m_Rot.y) * cosf(m_Rot.x) * (m_fDistance)),
		m_Pos.y - (sinf(m_Rot.x) * (m_fDistance)),
		m_Pos.z - (cosf(m_Rot.y) * cosf(m_Rot.x) * (m_fDistance))
	};
}

//============================================================================
// 注視点座標を計算
//============================================================================
void CCamera::CalcPosR()
{
	// 注視点を対象物の前方に
	m_PosR =
	{
		m_Pos.x + (sinf(m_Rot.y) * cosf(m_Rot.x) * m_fDistance),
		m_Pos.y + (sinf(m_Rot.x) * m_fDistance),
		m_Pos.z + (cosf(m_Rot.y) * cosf(m_Rot.x) * m_fDistance)
	};
}

//============================================================================
// プロジェクション行列計算
//============================================================================
void CCamera::CalcMtxProjection()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// プロジェクション行列の初期化
	D3DXMatrixIdentity(&m_MtxProjection);

	// プロジェクション行列を生成
#if 1
	// 透視投影
	D3DXMatrixPerspectiveFovLH(&m_MtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		5000.0f);
#else
	// 平行投影
	D3DXMatrixOrthoLH(&m_MtxProjection,
		(float)SCREEN_WIDTH * 0.25f,
		(float)SCREEN_HEIGHT * 0.25f,
		10.0f,
		5000.0f);
#endif

	// プロジェクション行列の設定
	pDev->SetTransform(D3DTS_PROJECTION,
		&m_MtxProjection);
}

//============================================================================
// ビュー行列計算
//============================================================================
void CCamera::CalcMtxView()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ビュー行列の初期化
	D3DXMatrixIdentity(&m_MtxView);

	// 視点座標の調整用
	Vec3 posV = m_PosV, posR = m_PosR;

	// 俯瞰度合いを反映
	posV.y += m_fAdjust;

	// ビュー行列の生成
	D3DXMatrixLookAtLH(&m_MtxView,
		&posV,
		&posR,
		&m_VecU);

	// ビュー行列の設定
	pDev->SetTransform(D3DTS_VIEW,
		&m_MtxView);
}