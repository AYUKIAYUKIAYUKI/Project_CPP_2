//============================================================================
// 
// Xオブジェクト管理 [object_X.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

// デバイス取得用
#include "renderer.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************

/// <summary> Zバッファでの描画切り替え </summary>
#define CHANGE_DRAW_ZBUFFER 0

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CObject_X::CObject_X(int nPriority) :
	CObject{ nPriority },
	m_pModel{ nullptr },
	m_Pos{ 0.0f, 0.0f, 0.0f },
	m_Rot{ 0.0f, 0.0f, 0.0f },
	m_Size{ 0.0f, 0.0f, 0.0f },
	m_fScale{ SCALE_INIT },
	m_fAlpha{ ALPHA_INIT }
{
	D3DXMatrixIdentity(&m_MtxWorld);	// ワールド行列
}

//============================================================================
// デストラクタ
//============================================================================
CObject_X::~CObject_X()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_X::Init()
{
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject_X::Uninit()
{

}

//============================================================================
// 更新処理
//============================================================================
void CObject_X::Update()
{
	// ワールド行列の設定
	SetMtxWorld();
}

//============================================================================
// 描画処理
//============================================================================
void CObject_X::Draw()
{
	// モデルが設定されていない
	if (m_pModel == nullptr)
	{
#ifdef _DEBUG
		CRenderer::GetInstance()->SetDebugString(static_cast<std::string>("【") + typeid(*this).name() + static_cast<std::string>("にモデル情報がありません！】"));
#endif	// _DEBUG
		return;
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

	// 頂点法線の自動正規化を有効に
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

#if CHANGE_DRAW_ZBUFFER

		// 深度テストの比較方法の変更
		pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		// 深度バッファに描画しない
		pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

#endif	// CHANGE_DRAW_ZBUFFER

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアル情報へのポインタ
	D3DXMATERIAL* pMat = nullptr;

	// ワールドマトリックスの設定
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// 現在のマテリアルを取得
	pDev->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<int>(m_pModel->dwNumMat); nCntMat++)
	{
		// アルファ値の設定
		pMat[nCntMat].MatD3D.Diffuse.a = m_fAlpha;

		// マテリアルの設定
		pDev->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDev->SetTexture(0, m_pModel->apTex[nCntMat]);

		// オブジェクトパーツの描画
		m_pModel->pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDev->SetMaterial(&matDef);

	// 頂点法線の自動正規化を無効に
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

#if CHANGE_DRAW_ZBUFFER

		// 深度テストの比較方法の変更
		pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		// 深度バッファに書き込む
		pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

#endif	// CHANGE_DRAW_ZBUFFER
}

//============================================================================
// モデル割当
//============================================================================
void CObject_X::BindModel(CModel_X_Manager::MODEL* pModel)
{
	m_pModel = pModel;
}

//============================================================================
// もっとモデル割当
//============================================================================
void CObject_X::BindModel(CModel_X_Manager::TYPE Type)
{
	m_pModel = CModel_X_Manager::GetInstance()->GetModel(Type);
}

//============================================================================
// 座標取得
//============================================================================
const D3DXVECTOR3& CObject_X::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標設定
//============================================================================
void CObject_X::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 向き取得
//============================================================================
const D3DXVECTOR3& CObject_X::GetRot()const
{
	return m_Rot;
}

//============================================================================
// 向き設定
//============================================================================
void CObject_X::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// サイズ取得
//============================================================================
const D3DXVECTOR3& CObject_X::GetSize() const
{
	return m_Size;
}

//============================================================================
// サイズ設定
//============================================================================
void CObject_X::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// 縮尺を取得
//============================================================================
const float& CObject_X::GetScale() const
{
	return m_fScale;
}

//============================================================================
// 縮尺を設定
//============================================================================
void CObject_X::SetScale(float fScale)
{
	m_fScale = fScale;
}

//============================================================================
// アルファ値を取得
//============================================================================
const float& CObject_X::GetAlpha() const
{
	return m_fAlpha;
}

//============================================================================
// アルファ値を設定
//============================================================================
void CObject_X::SetAlpha(float fAlpha)
{
	m_fAlpha = fAlpha;
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create()
{
	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X();

	// 生成出来ていたら初期設定
	if (pObjectX != nullptr)
	{
		pObjectX->Init();
	}

	return pObjectX;
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create(int nPriority)
{
	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X(nPriority);

	// 生成出来ていたら初期設定
	if (pObjectX != nullptr)
	{
		pObjectX->Init();
	}

	return pObjectX;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// ワールド行列設定
//============================================================================
void CObject_X::SetMtxWorld()
{
	// 計算用行列
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 拡縮行列作成
	D3DXMatrixScaling(&mtxScale,
		m_fScale,
		m_fScale,
		m_fScale);

	// 拡縮行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxScale);

	// 回転行列作成
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y,
		m_Rot.x,
		m_Rot.z);

	// 回転行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxRot);

	// 平行移動行列作成
	D3DXMatrixTranslation(&mtxTrans,
		m_Pos.x,
		m_Pos.y,
		m_Pos.z);

	// 平行移動行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);
}