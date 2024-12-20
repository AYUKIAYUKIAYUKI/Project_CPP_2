//============================================================================
//
// 扇形 [fan.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "fan.h"
#include "motion_set.h"
#include "field_manager.h"
#include "renderer.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define DRAW_DEBUG_LINE 1	// デバッグ用のラインを描画する

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************

// 基礎パラメータの展開
const JSON CFan::m_InitParam = utility::OpenJsonFile("Data\\JSON\\fan_parameter.json");

//============================================================================
//
// publicメンバ
//
//============================================================================

//============================================================================
// 破棄
//============================================================================
void CFan::Release()
{
	// 自身の破棄
	if (this != nullptr)
	{
		this->Uninit();	// 終了処理
		delete this;	// メモリを解放
	}
}

//============================================================================
// 更新処理
//============================================================================
void CFan::Update()
{
#if DRAW_DEBUG_LINE	// デバッグ用のラインを描画する
	// 頂点座標の設定
	SetVtx();

	// ワールド行列設定
	SetMtxWorld();
#endif // DRAW_DEBUG_LINE

	// 方角に合わせて範囲分の方向ベクトルを2本作成
	m_DirVec[0] = { cosf(m_fDirection + m_fRange), 0, sinf(m_fDirection + m_fRange) };
	m_DirVec[1] = { cosf(m_fDirection - m_fRange), 0, sinf(m_fDirection - m_fRange) };
}

//============================================================================
// 描画処理
//============================================================================
void CFan::Draw()
{
#if DRAW_DEBUG_LINE	// デバッグ用のラインを描画する
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ライトをオフ
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点バッファをデータストリームに設定
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDev->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの設定
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// テクスチャの設定
	pDev->SetTexture(0, nullptr);

	// 線の描画
	pDev->DrawPrimitive(D3DPT_LINESTRIP,	// プリミティブの種類
		0,									// 頂点情報の先頭アドレス
		NUM_VEC);							// プリミティブ数

	// ライトをオン
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
#endif // デバッグ用のラインを描画する
}

//============================================================================
// 扇形範囲内にあるか検出
//============================================================================
bool CFan::DetectInFanRange(D3DXVECTOR3 Pos)
{
	// 対象物へのベクトルを出す
	Vec3 OtherVec = Pos - m_Pos;

	// 対象物へのベクトルを正規化
	D3DXVec3Normalize(&OtherVec, &OtherVec);
	
	// 片方の扇形方向のベクトルと対象物への方向ベクトルの外積を作成
	float fCross1 = m_DirVec[0].x * OtherVec.z - m_DirVec[0].z * OtherVec.x;

	// この時点で1つ目の方向ベクトルとの外積結果が正の角度なら範囲外
	if (fCross1 > 0.0f)
	{
		return 0;
	}

	// もう片方の外積も作成
	float fCross2 = m_DirVec[1].x * OtherVec.z - m_DirVec[1].z * OtherVec.x;

	// 2つ目の方向ベクトルとの外積結果が負の角度なら範囲外
	if (fCross2 < 0.0f)
	{
		return 0;
	}

	// 条件を満たしていれば扇形の範囲内
	return 1;
}

//============================================================================
// 座標を取得
//============================================================================
const D3DXVECTOR3& CFan::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標を設定
//============================================================================
void CFan::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 方角を取得
//============================================================================
const float& CFan::GetDirection() const
{
	return m_fDirection;
}

//============================================================================
// 方角を設定
//============================================================================
void CFan::SetDirection(float fDirection)
{
	m_fDirection = fDirection;
}

//============================================================================
// 範囲を取得
//============================================================================
const float& CFan::GetRange() const
{
	return m_fRange;
}

//============================================================================
// 範囲を設定
//============================================================================
void CFan::SetRange(float fRange)
{
	m_fRange = fRange;
}

//============================================================================
// 生成
//============================================================================
CFan* CFan::Create()
{
	CFan* pNewInstance = DBG_NEW CFan();

	// 生成失敗
	if (!pNewInstance) throw std::bad_alloc();

	// 初期設定
	pNewInstance->Init();
	
	return pNewInstance;
}

//============================================================================
//
// privateメンバ
//
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CFan::CFan() :
	m_pVtxBuff{ nullptr },
	m_Pos{ VEC3_INIT },
	m_DirVec{ VEC3_INIT, VEC3_INIT },
	m_fDirection{ 0.0f },
	m_fRange{ 0.0f }
{
	// ワールド行列の初期化
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// デストラクタ
//============================================================================
CFan::~CFan()
{
	// 念のため終了処理
	Uninit();
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CFan::Init()
{
#if DRAW_DEBUG_LINE	// デバッグ用のラインを描画
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}
#endif // DRAW_DEBUG_LINE

	// データをキャスト
	float
		fDirection = static_cast<float>(m_InitParam["Direction"]),
		fRange = static_cast<float>(m_InitParam["Range"]);
		
	// 扇形のパラメータをセット
	m_fDirection = fDirection;	// 方角
	m_fRange = fRange;			// 範囲

	return S_OK;
}

//============================================================================
// 頂点バッファの生成
//============================================================================
HRESULT CFan::CreateVtxBuff()
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

	for (WORD i = 0; i < NUM_VTX; ++i)
	{
		// 頂点座標を設定
		pVtx[i].pos = VEC3_INIT;

		// 法線ベクトルの設定
		pVtx[i].nor = VEC3_INIT;

		// テクスチャ座標の設定
		pVtx[i].tex = VEC2_INIT;
	}

	// 頂点色の設定
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CFan::Uninit()
{
#if DRAW_DEBUG_LINE
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
#endif // DRAW_DEBUG_LINE
}

//============================================================================
// 頂点座標の設定
//============================================================================
void CFan::SetVtx()
{
	// 方角に合わせて範囲分の方向ベクトルを2本作成
	m_DirVec[0] = { cosf(m_fDirection + m_fRange), 0, sinf(m_fDirection + m_fRange) };
	m_DirVec[1] = { cosf(m_fDirection - m_fRange), 0, sinf(m_fDirection - m_fRange) };

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);
	
	// 頂点座標の設定
	pVtx[0].pos = m_DirVec[0] * 500.0f;
	pVtx[1].pos = m_Pos;
	pVtx[2].pos = m_DirVec[1] * 500.0f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================================
// ワールド行列設定
//============================================================================
void CFan::SetMtxWorld()
{
	// 計算用行列
	Mtx mtxRot, mtxTrans;

	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);

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