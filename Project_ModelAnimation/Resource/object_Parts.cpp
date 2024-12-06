//============================================================================
// 
// パーツオブジェクト [object_Parts.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object_parts.h"
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
CObject_Parts::CObject_Parts(LAYER Priority) :
	CObject{ Priority },
	m_pParent{ nullptr },
	m_pModel{ nullptr },
	m_ScaleOffset{ VEC3_INIT },
	m_Scale{ 1.0f, 1.0f, 1.0f },
	m_RotOffset{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_PosOffset{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_bUseCol{ false }
{
	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// デストラクタ
//============================================================================
CObject_Parts::~CObject_Parts()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_Parts::Init()
{
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject_Parts::Uninit()
{

}

//============================================================================
// 更新処理
//============================================================================
void CObject_Parts::Update()
{
	// 向きを調整する
	AdjustRotAngle();

	// ワールド行列の設定
	SetMtxWorld();
}

//============================================================================
// 描画処理
//============================================================================
void CObject_Parts::Draw()
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

	// アルファテストを有効にする
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// アルファブレンドの設定を変更
	//pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

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

	for (WORD wCntMat = 0; wCntMat < static_cast<WORD>(m_pModel->dwNumMat); wCntMat++)
	{
		// マテリアル色の設定
		m_bUseCol ? pMat[wCntMat].MatD3D.Diffuse = m_Col : pMat[wCntMat].MatD3D.Diffuse = m_pModel->apColMat[wCntMat];

		// マテリアルの設定
		pDev->SetMaterial(&pMat[wCntMat].MatD3D);

		// テクスチャの設定
		pDev->SetTexture(0, m_pModel->apTex[wCntMat]);

		// オブジェクトパーツの描画
		m_pModel->pMesh->DrawSubset(wCntMat);
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

	// アルファブレンドの設定を基本に戻す
	//pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを無効にする
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// 親パーツのポインタ取得
//============================================================================
const CObject_Parts* const CObject_Parts::GetParent()const
{
	return m_pParent;
}

//============================================================================
// 親パーツのポインタ設定
//============================================================================
void CObject_Parts::SetParent(CObject_Parts* pParent)
{
	m_pParent = pParent;
}

//============================================================================
// モデル取得
//============================================================================
CX_Manager::MODEL* CObject_Parts::GetModel()
{
	return m_pModel;
}

//============================================================================
// モデル割当
//============================================================================
void CObject_Parts::BindModel(CX_Manager::MODEL* pModel)
{
	m_pModel = pModel;
}

//============================================================================
// もっとモデル割当
//============================================================================
void CObject_Parts::BindModel(CX_Manager::TYPE Type)
{
	// モデルを取得して設定
	m_pModel = CX_Manager::GetInstance()->GetModel(Type);
}

//============================================================================
// 縮尺オフセットを取得
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetScaleOffset() const
{
	return m_ScaleOffset;
}

//============================================================================
// 縮尺オフセットを設定
//============================================================================
void CObject_Parts::SetScaleOffset(D3DXVECTOR3 Scale)
{
	m_ScaleOffset = Scale;
}

//============================================================================
// 縮尺を取得
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetScale() const
{
	return m_Scale;
}

//============================================================================
// 縮尺を設定
//============================================================================
void CObject_Parts::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//============================================================================
// 向きオフセット取得
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetRotOffset() const
{
	return m_RotOffset;
}

//============================================================================
// 向きオフセット設定
//============================================================================
void CObject_Parts::SetRotOffset(D3DXVECTOR3 Rot)
{
	m_RotOffset = Rot;
}

//============================================================================
// 向き取得
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetRot()const
{
	return m_Rot;
}

//============================================================================
// 向き設定
//============================================================================
void CObject_Parts::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// 座標オフセット取得
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetPosOffset() const
{
	return m_PosOffset;
}

//============================================================================
// 座標オフセット設定
//============================================================================
void CObject_Parts::SetPosOffset(D3DXVECTOR3 Pos)
{
	m_PosOffset = Pos;
}

//============================================================================
// 座標取得
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標設定
//============================================================================
void CObject_Parts::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 色を取得
//============================================================================
const D3DXCOLOR& CObject_Parts::GetCol() const
{
	return m_Col;
}

//============================================================================
// 色を設定
//============================================================================
void CObject_Parts::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// 色反映を取得
//============================================================================
const bool& CObject_Parts::GetUseCol() const
{
	return m_bUseCol;
}

//============================================================================
// 色反映を設定
//============================================================================
void CObject_Parts::SetUseCol(bool bUse)
{
	m_bUseCol = bUse;
}

//============================================================================
// ワールド行列を取得
//============================================================================
const D3DXMATRIX& CObject_Parts::GetMtxWorld() const
{
	return m_MtxWorld;
}

//============================================================================
// サイズを取得
//============================================================================
D3DXVECTOR3 CObject_Parts::GetSize() const
{
	return VEC3_INIT;
}

//============================================================================
// 半径を取得
//============================================================================
float CObject_Parts::GetRadius() const
{
	return 0.0f;
}

//============================================================================
// 高さを取得
//============================================================================
float CObject_Parts::GetHeight() const
{
	return 0.0f;
}

//============================================================================
// 生成
//============================================================================
CObject_Parts* CObject_Parts::Create(CX_Manager::TYPE Type, CObject_Parts* pParent)
{
	// インスタンスを生成
	CObject_Parts* pNewInstance = DBG_NEW CObject_Parts();

	// 生成失敗
	if (pNewInstance == nullptr)
	{
		assert(false && "パーツオブジェクトの生成に失敗しました");
	}

	// 親パーツのポインタを設定
	pNewInstance->SetParent(pParent);

	// モデルを設定
	pNewInstance->BindModel(Type);

	// パーツオブジェクトの初期設定
	pNewInstance->Init();

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 向きを調整する
//============================================================================
void CObject_Parts::AdjustRotAngle()
{
	while (fabsf(m_Rot.y) > D3DX_PI)
	{
		utility::AdjustDirection(m_Rot.y);
	}
}

//============================================================================
// ワールド行列設定
//============================================================================
void CObject_Parts::SetMtxWorld()
{
	// 計算用行列
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 拡縮行列作成
	D3DXMatrixScaling(&mtxScale,
		m_ScaleOffset.x + m_Scale.x,
		m_ScaleOffset.y + m_Scale.y,
		m_ScaleOffset.z + m_Scale.z);

	// 拡縮行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxScale);

	// 回転行列作成
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_RotOffset.x + m_Rot.x,
		m_RotOffset.y + m_Rot.y,
		m_RotOffset.z + m_Rot.z);

	// 回転行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxRot);

	// 平行移動行列作成
	D3DXMatrixTranslation(&mtxTrans,
		m_PosOffset.x + m_Pos.x,
		m_PosOffset.y + m_Pos.y,
		m_PosOffset.z + m_Pos.z);

	// 平行移動行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);

	// 親パーツが無ければここで終了
	if (m_pParent == nullptr)
	{
		return;
	}

	// 親パーツのワールド行列を取得
	D3DXMATRIX mtxParent = m_pParent->GetMtxWorld();

	// 親パーツのワールド行列とかけ合わせる
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxParent);
}