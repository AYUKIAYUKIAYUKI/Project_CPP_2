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

//****************************************************
// usingディレクティブ
//****************************************************
using namespace camera;

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CCamera::CCamera() :
	m_Pos{ 0.0f, 0.0f, 0.0f },
	m_PosTarget{ 0.0f, 0.0f, 0.0f },
	m_PosV{ 0.0f, 0.0f, 0.0f },
	m_PosTargetV{ 0.0f, 0.0f, 0.0f },
	m_PosR{ 0.0f, 0.0f, 0.0f },
	m_PosTargetR{ 0.0f, 0.0f, 0.0f },
	m_Rot{ 0.0f, 0.0f, 0.0f },
	m_RotTarget{ 0.0f, 0.0f, 0.0f },
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
	m_Pos = { 0.0f, 0.0f, 0.0f };
	m_PosTarget = { 0.0f, 0.0f, 0.0f };
	m_PosV = { 0.0f, 0.0f, 0.0f };
	m_PosTargetV = { 0.0f, 0.0f, 0.0f };
	m_PosR = { 0.0f, 0.0f, 0.0f };
	m_PosTargetR = { 0.0f, 0.0f, 0.0f };
	m_Rot = { 0.0f, 0.0f, 0.0f };
	m_RotTarget = { 0.0f, 0.0f, 0.0f };
	m_fDistance = { 0.0f };
	m_VecU = { 0.0f, 1.0f, 0.0f };
	m_fAdjust = 0.0f;
	m_fDistance = 300.0f;
	D3DXMatrixIdentity(&m_MtxProjection);
	D3DXMatrixIdentity(&m_MtxView);

	return S_OK;
}

//============================================================================
// 更新処理
//============================================================================
void CCamera::Update()
{
	// カメラ操作
	Control();

	// 回転
	Rotation();

	// 移動
	Translation();

	// 視点座標を計算
	CalcPosV();

	// 注視点座標を計算
	CalcPosR();

#ifdef _DEBUG
	CRenderer::GetInstance()->SetDebugString("カメラ座標 : " + std::to_string(GetPos().x) + " :  " + std::to_string(GetPos().y) + " : " + std::to_string(m_Pos.z));
	CRenderer::GetInstance()->SetDebugString("カメラ向き : " + std::to_string(m_Rot.x) + " :  " + std::to_string(m_Rot.y) + " : " + std::to_string(m_Rot.z));
	CRenderer::GetInstance()->SetDebugString("カメラ間距離 : " + std::to_string(m_fDistance));
#endif // _DEBUG
}

//============================================================================
// カメラをセット
//============================================================================
void CCamera::SetCamera()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

	// 画面バッファクリア
	pDev->Clear(0,
		nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

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
D3DXVECTOR3 CCamera::GetPosTarget() const
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
D3DXVECTOR3 CCamera::GetRot() const
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
D3DXVECTOR3 CCamera::GetRotTarget() const
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
float CCamera::GetDistance() const
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
// カメラ操作
//============================================================================
void CCamera::Control()
{
	// ズームイン / アウト
	if (CManager::GetKeyboard()->GetPress(DIK_AT) && m_fDistance > 10.0f)
	{
		m_fDistance -= 10.0f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_COLON))
	{
		m_fDistance += 10.0f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_BACKSLASH))
	{
		// 距離間リセット
		m_fDistance = 200.0f;
	}

	// 左右
	if (CManager::GetKeyboard()->GetPress(DIK_RIGHT))
	{
		m_RotTarget.y += 0.02f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_LEFT))
	{
		m_RotTarget.y -= 0.02f;
	}

	// 上下
	if (CManager::GetKeyboard()->GetPress(DIK_UP))
	{
		m_RotTarget.x += 0.02f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_DOWN))
	{
		m_RotTarget.x -= 0.02f;
	}
}

//============================================================================
// 回転
//============================================================================
void CCamera::Rotation()
{
	// 回転量減衰
	m_RotTarget *= 0.8f;

	// 回転量反映
	m_Rot += m_RotTarget * 0.5f;

	// ヨー角の範囲を制限
	RestrictYaw();

	// ピッチ角の範囲を制限
	RestrictPitch();
}

//============================================================================
// 移動
//============================================================================
void CCamera::Translation()
{
	// 追従度合
	float fTracking = 0.25f;

	// 上下
	if (CManager::GetKeyboard()->GetPress(DIK_W))
	{
		m_PosTarget.y += 1.0f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_S))
	{
		m_PosTarget.y -= 1.0f;
	}

	// 移動量反映
	// posTargetに値を設定していない場合は数値がおかしくなります
	m_Pos += (m_PosTarget - m_Pos) * fTracking;
}

//============================================================================
// ヨー角の範囲を制限
//============================================================================
void CCamera::RestrictYaw()
{
	if (m_Rot.y > D3DX_PI)
	{
		m_Rot.y += -D3DX_PI * 2.0f;
	}
	else if (m_Rot.y < -D3DX_PI)
	{
		m_Rot.y += D3DX_PI * 2.0f;
	}
}

//============================================================================
// ピッチ角の範囲を制限
//============================================================================
void CCamera::RestrictPitch()
{
	// 0 ～ 0.5
	float fCoeff{ 0.48f };

	if (m_Rot.x + m_RotTarget.x > D3DX_PI * fCoeff)
	{
		m_Rot.x = D3DX_PI * fCoeff;
	}
	else if (m_Rot.x + m_RotTarget.x < -D3DX_PI * fCoeff)
	{
		m_Rot.x = -D3DX_PI * fCoeff;
	}
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
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

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
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

	// ビュー行列の初期化
	D3DXMatrixIdentity(&m_MtxView);

	// 視点座標の調整用
	D3DXVECTOR3 posV = m_PosV;
	D3DXVECTOR3 posR = m_PosR;

	// ビュー行列の生成
	D3DXMatrixLookAtLH(&m_MtxView,
		&posV,
		&posR,
		&m_VecU);

	// ビュー行列の設定
	pDev->SetTransform(D3DTS_VIEW,
		&m_MtxView);
}