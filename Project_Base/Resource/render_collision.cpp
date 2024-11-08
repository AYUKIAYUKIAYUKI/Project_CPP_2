//============================================================================
// 
// 判定表示 [render_collision.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "render_collision.h"
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
CRender_Collision::CRender_Collision(LAYER Priority) :
	CObject{ Priority },
	m_pRef{ nullptr },
	m_pVtxBuff{ nullptr },
	m_pIdxBuff{ nullptr },
	m_Col{ XCOL_INIT }
{
	// ワールド行列の初期化
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// デストラクタ
//============================================================================
CRender_Collision::~CRender_Collision()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CRender_Collision::Init()
{
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
void CRender_Collision::Uninit()
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
}

//============================================================================
// 更新処理
//============================================================================
void CRender_Collision::Update()
{
	// 頂点の設定
	SetVtx();

	// ワールド行列の設定
	SetMtxWorld();
}

//============================================================================
// 描画処理
//============================================================================
void CRender_Collision::Draw()
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
// 対象オブジェクトの設定
//============================================================================
void CRender_Collision::SetRefObj(CObject_X* pRef)
{
	m_pRef = pRef;
}

//============================================================================
// 生成
//============================================================================
CRender_Collision* CRender_Collision::Create(CObject_X* pRef)
{
	// 判定表示を生成
	CRender_Collision* pRender_Collision = DBG_NEW CRender_Collision(LAYER::FRONT);

	// 生成失敗
	if (pRender_Collision == nullptr)
	{
		assert(false && "判定表示の生成に失敗しました");
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
HRESULT CRender_Collision::CreateVtxBuff()
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
HRESULT CRender_Collision::CreateIdxBuff()
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
void CRender_Collision::SetVtx()
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし頂点情報時へのポインタを取得
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// サイズをコピー
	const Vec3& Size = m_pRef->GetSize();

	// 向きをコピー
	const Vec3& Rot = m_pRef->GetRot();

	// 回転行列を定義
	struct Mtx_Rot
	{
		float _11, _13, _31, _33;
	};

	// 回転行列を作成
	Mtx_Rot MtxRot = 
	{
		0.0f, 0.0f, 0.0f, 0.0f
	};

	// 向きから回転行列を作成
	MtxRot._11 = cosf(Rot.y);
	MtxRot._13 = sinf(Rot.y);
	MtxRot._31 = -sinf(Rot.y);
	MtxRot._33 = cosf(Rot.y);

	// 回転後のX・Z頂点座標の作成
	float VtxPosX = MtxRot._11 + MtxRot._31;
	float VtxPosZ = MtxRot._31 + MtxRot._33;

	VtxPosX *= Size.x;
	VtxPosZ *= Size.z;

	// 頂点座標を設定
#if 0
	pVtx[0].pos = { VtxPosX, +Size.y, -Size.z };
	pVtx[1].pos = { VtxPosX, +Size.y, -Size.z };
	pVtx[2].pos = { VtxPosX, -Size.y, -Size.z };
	pVtx[3].pos = { VtxPosX, -Size.y, -Size.z };
	pVtx[4].pos = { VtxPosX, +Size.y, +Size.z };
	pVtx[5].pos = { VtxPosX, +Size.y, +Size.z };
	pVtx[6].pos = { VtxPosX, -Size.y, +Size.z };
	pVtx[7].pos = { VtxPosX, -Size.y, +Size.z };
#elif 0
	pVtx[0].pos = { -VtxPosX, +Size.y, -VtxPosZ };
	pVtx[1].pos = { +VtxPosX, +Size.y, -VtxPosZ };
	pVtx[2].pos = { -VtxPosX, -Size.y, -VtxPosZ };
	pVtx[3].pos = { +VtxPosX, -Size.y, -VtxPosZ };
	pVtx[4].pos = { -VtxPosX, +Size.y, +VtxPosZ };
	pVtx[5].pos = { +VtxPosX, +Size.y, +VtxPosZ };
	pVtx[6].pos = { -VtxPosX, -Size.y, +VtxPosZ };
	pVtx[7].pos = { +VtxPosX, -Size.y, +VtxPosZ };
#elif 1
	// 回転角度に基づき cos と sin を計算
	float cosY = cosf(-Rot.y);
	float sinY = sinf(-Rot.y);

	// 各頂点の位置を回転させた後にサイズを掛けて配置
	pVtx[0].pos = { (-cosY * Size.x - sinY * Size.z), +Size.y, (-sinY * Size.x + cosY * Size.z) };
	pVtx[1].pos = { (cosY * Size.x - sinY * Size.z), +Size.y, (sinY * Size.x + cosY * Size.z) };
	pVtx[2].pos = { (-cosY * Size.x - sinY * Size.z), -Size.y, (-sinY * Size.x + cosY * Size.z) };
	pVtx[3].pos = { (cosY * Size.x - sinY * Size.z), -Size.y, (sinY * Size.x + cosY * Size.z) };

	pVtx[4].pos = { (-cosY * Size.x + sinY * Size.z), +Size.y, (-sinY * Size.x - cosY * Size.z) };
	pVtx[5].pos = { (cosY * Size.x + sinY * Size.z), +Size.y, (sinY * Size.x - cosY * Size.z) };
	pVtx[6].pos = { (-cosY * Size.x + sinY * Size.z), -Size.y, (-sinY * Size.x - cosY * Size.z) };
	pVtx[7].pos = { (cosY * Size.x + sinY * Size.z), -Size.y, (sinY * Size.x - cosY * Size.z) };
#endif

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//============================================================================
// ワールド行列設定
//============================================================================
void CRender_Collision::SetMtxWorld()
{
	// 計算用行列
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 平行移動行列作成
	D3DXMatrixTranslation(&mtxTrans,
		m_pRef->GetPos().x,
		m_pRef->GetPos().y,
		m_pRef->GetPos().z);

	// 平行移動行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);
}