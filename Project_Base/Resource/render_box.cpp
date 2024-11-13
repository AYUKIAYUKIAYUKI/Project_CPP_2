//============================================================================
// 
// ボックス表示 [render_box.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "render_box.h"
#include "object_X.h"

// デバイス取得用
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
// 描画優先度指定コンストラクタ
//============================================================================
CRender_Box::CRender_Box(LAYER Priority) :
	CRender_Collision{ Priority },
	m_pVtxBuff{ nullptr },
	m_pIdxBuff{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CRender_Box::~CRender_Box()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CRender_Box::Init()
{
	// 判定表示の初期設定
	if (FAILED(CRender_Collision::Init()))
	{
		return E_FAIL;
	}

	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// インデックスバッファの生成
	if (FAILED(CreateIdxBuff()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CRender_Box::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	// 判定表示の終了処理
	CRender_Collision::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CRender_Box::Update()
{
	// 頂点の設定
	SetVtx();

	// 判定表示の更新処理
	CRender_Collision::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CRender_Box::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ライトをオフ
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点バッファをデータストリームに設定
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDev->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDev->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの設定
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// テクスチャの設定
	pDev->SetTexture(0, nullptr);

	// 線の描画
	pDev->DrawIndexedPrimitive(D3DPT_LINELIST,
		0,
		0,
		NUM_VTX,	// 頂点数
		0,
		NUM_PRIM);	// 辺の数

	// ライトをオン
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================================
// 生成
//============================================================================
CRender_Box* CRender_Box::Create(CObject_X* pRef)
{
	// 判定表示を生成
	CRender_Box* pRender_Collision = DBG_NEW CRender_Box(LAYER::FRONT);

	// 生成失敗
	if (pRender_Collision == nullptr)
	{
		assert(false && "ボックス表示の生成に失敗しました");
	}

	// 判定表示の初期設定
	pRender_Collision->Init();

	// 対象オブジェクトの設定
	pRender_Collision->SetRefObj(pRef);

	return pRender_Collision;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 頂点バッファの生成
//============================================================================
HRESULT CRender_Box::CreateVtxBuff()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 頂点バッファの生成
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (m_pVtxBuff == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// 頂点座標の設定
		pVtx[wNumVtx].pos = VEC3_INIT;

		// 法線ベクトル
		pVtx[wNumVtx].nor = VEC3_INIT;

		// 頂点色の設定
		pVtx[wNumVtx].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[wNumVtx].tex = VEC2_INIT;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// インデックス情報の生成
//============================================================================
HRESULT CRender_Box::CreateIdxBuff()
{
	// インデックスバッファの生成
	CRenderer::GetRenderer()->GetDeviece()->CreateIndexBuffer(sizeof(WORD) * NUM_IDX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	if (m_pIdxBuff == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// インデックス情報へのポインタ
	WORD* pIdx = nullptr;

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, reinterpret_cast<void**>(&pIdx), 0);

	// X軸に平行な線を引く
	for (WORD i = 0; i < 8; ++i, pIdx++)
	{
		pIdx[0] = i;
	}

	// Y軸に平行な線を引く
	for (WORD i = 0, j = 0; i < 4; ++i, pIdx += 2)
	{
		if (i == 2) j += 2;	// 0, 1 ,4 ,5を辺の始点にするため3, 4のインデックスを飛ばす

		pIdx[0] = i + j;
		pIdx[1] = i + 2 + j;
	}

	// Z軸に平行な線を引く
	for (WORD i = 0; i < 4; ++i, pIdx += 2)
	{
		pIdx[0] = i;
		pIdx[1] = i + 4;
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//============================================================================
// 頂点の設定
//============================================================================
void CRender_Box::SetVtx()
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし頂点情報時へのポインタを取得
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// サイズをコピー
	const Vec3& Size = m_pRef->GetSize();

	// 向きをコピー
	const Vec3& Rot = m_pRef->GetRot();

	// 回転行列を用いて頂点座標をセット
	for (WORD i = 0; i < NUM_VTX; ++i)
	{
		pVtx[i].pos = CUtility::RotateBoxAroundY(i, Rot.y, Size);
		pVtx[i].pos = CUtility::InverseRotateBoxAroundY(Rot.y, pVtx[i].pos);	/* すぐに消せ */
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}