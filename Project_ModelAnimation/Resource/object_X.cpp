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
#include "renderer.h"

#include "manager.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************

/// <summary> Zバッファでの描画切り替え </summary>
#define CHANGE_DRAW_ZBUFFER 0

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
CObject_X::CObject_X(LAYER Priority) :
	CObject{ Priority },
	m_pModel{ nullptr },
	m_Pos{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Scale{ 1.0f, 1.0f, 1.0f },
	m_Col{ XCOL_INIT },
	m_bUseCol{ false }
{
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

}

//============================================================================
// 更新処理
//============================================================================
void CObject_X::Update()
{
	CManager::GetKeyboard()->GetPress(DIK_1) ? m_bUseCol = 1 : m_bUseCol = 0;

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
}

//============================================================================
// モデル取得
//============================================================================
CX_Manager::MODEL* CObject_X::GetModel()
{
	return m_pModel;
}

//============================================================================
// モデル割当
//============================================================================
void CObject_X::BindModel(CX_Manager::MODEL* pModel)
{
	m_pModel = pModel;
}

//============================================================================
// もっとモデル割当
//============================================================================
void CObject_X::BindModel(CX_Manager::TYPE Type)
{
	// モデルを取得して設定
	m_pModel = CX_Manager::GetInstance()->GetModel(Type);
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
const D3DXVECTOR3& CObject_X::GetScale() const
{
	return m_Scale;
}

//============================================================================
// 縮尺を設定
//============================================================================
void CObject_X::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//============================================================================
// 色を取得
//============================================================================
const D3DXCOLOR& CObject_X::GetCol() const
{
	return m_Col;
}

//============================================================================
// 色を設定
//============================================================================
void CObject_X::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// 色反映を取得
//============================================================================
const bool& CObject_X::GetUseCol() const
{
	return m_bUseCol;
}

//============================================================================
// 色反映を設定
//============================================================================
void CObject_X::SetUseCol(bool bUse)
{
	m_bUseCol = bUse;
}

//============================================================================
// サイズを取得
//============================================================================
D3DXVECTOR3 CObject_X::GetSize() const
{
	return VEC3_INIT;
}

//============================================================================
// 半径を取得
//============================================================================
float CObject_X::GetRadius() const
{
	return 0.0f;
}

//============================================================================
// 高さを取得
//============================================================================
float CObject_X::GetHeight() const
{
	return 0.0f;
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create(CX_Manager::TYPE Type)
{
	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X();

	// 生成失敗
	if (pObjectX == nullptr)
	{
		assert(false && "Xオブジェクトの生成に失敗しました");
	}

	// モデルを設定
	pObjectX->BindModel(Type);

	// Xオブジェクトの初期設定
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create(LAYER Priority, CX_Manager::TYPE Type)
{
	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X(Priority);

	// 生成失敗
	if (pObjectX == nullptr)
	{
		assert(false && "Xオブジェクトの生成に失敗しました");
	}

	// モデルを設定
	pObjectX->BindModel(Type);

	// Xオブジェクトの初期設定
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create(JSON Json)
{
	// パラメータをコピーしていく
	const auto& Priority = Json["Priority"];
	const auto& ModelType = Json["ModelType"];
	const auto& Rot = Json["Rot"];
	const auto& Pos = Json["Pos"];
	const auto& Scale = Json["Scale"];

	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X(static_cast<CObject::LAYER>(Priority));

	// 生成失敗
	if (pObjectX == nullptr)
	{
		assert(false && "Xオブジェクトの生成に失敗しました");
	}

	// モデルを設定
	pObjectX->BindModel(static_cast<CX_Manager::TYPE>(ModelType));

	// Xオブジェクトの初期設定
	pObjectX->Init();

	// 各種パラメータを設定
	pObjectX->SetRot(Vec3(Rot[0], Rot[1], Rot[2]));
	pObjectX->SetPos(Vec3(Pos[0], Pos[1], Pos[2]));
	pObjectX->SetScale(Vec3(Scale[0], Scale[1], Scale[2]));

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
		utility::AdjustDirection(m_Rot.y);
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
		m_Scale.x,
		m_Scale.y,
		m_Scale.z);

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