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
#include "render_collision.h"

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
CObject_X::CObject_X(LAYER Priority) :
	CObject{ Priority },
	m_pModel{ nullptr },
	m_pRender_Collision{ nullptr },
	m_Pos{ 0.0f, 0.0f, 0.0f },
	m_Rot{ 0.0f, 0.0f, 0.0f },
	m_fScale{ SCALE_INIT },
	m_fAlpha{ ALPHA_INIT }
{
	// 判定表示を生成
	m_pRender_Collision = CRender_Collision::Create(this);

	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);
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
	// 判定表示の破棄
	if (m_pRender_Collision != nullptr)
	{
		// 判定表示の破棄予約
		m_pRender_Collision->SetRelease();

		// ポインタを初期化
		m_pRender_Collision = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CObject_X::Update()
{
	// 向きを調整する
	AdjustRotAngle();

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
#ifdef _DEBUG	// 警告表示

		CRenderer::SetDebugString(static_cast<std::string>("【") + typeid(*this).name() + static_cast<std::string>("にモデル情報がありません！】"));

#endif	// _DEBUG

		return;
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

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
// モデル取得
//============================================================================
CModel_X_Manager::MODEL* CObject_X::GetModel()
{
	return m_pModel;
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
	// モデルを取得して設定
	m_pModel = CModel_X_Manager::GetInstance()->GetModel(Type);
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
// 半径を取得
//============================================================================
float CObject_X::GetRadius() const
{
	return 0.0f;
}

//============================================================================
// サイズを取得
//============================================================================
D3DXVECTOR3 CObject_X::GetSize() const
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create()
{
	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X();

	// 生成失敗
	if (pObjectX == nullptr)
	{
		assert(false && "Xオブジェクトの生成に失敗しました");
	}

	// Xオブジェクトの初期設定
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create(LAYER Priority)
{
	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X(Priority);

	// 生成失敗
	if (pObjectX == nullptr)
	{
		assert(false && "Xオブジェクトの生成に失敗しました");
	}

	// Xオブジェクトの初期設定
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 向きを調整する
//============================================================================
void CObject_X::AdjustRotAngle()
{
	while (fabsf(m_Rot.y) > D3DX_PI)
	{
		CUtility::AdjustDirection(m_Rot.y);
	}
}

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