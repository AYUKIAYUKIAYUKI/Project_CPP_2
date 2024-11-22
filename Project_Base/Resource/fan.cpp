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
#include "field_manager.h"

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
	// 頂点座標の設定
	SetVtx();

	// ワールド行列設定
	SetMtxWorld();

#ifdef _DEBUG

	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("扇形の方角：" + to_string(m_fDirection));
	CRenderer::SetDebugString("扇形の範囲：" + to_string(m_fRange));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");

#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CFan::Draw()
{
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

	float fDot[NUM_VEC] = { 0.0f, 0.0f };
	float fCross[NUM_VEC] = { 0.0f, 0.0f };

	// 対象物への方向ベクトルと扇形方向ベクトルから内積を作成
	for(int i = 0; i < NUM_VEC; ++i)
		fDot[i] = m_DirVec[i].x * OtherVec.x + m_DirVec[i].z * OtherVec.z;
	
	// 対象物への方向ベクトルと扇形方向のベクトルから外積を作成
	for (int i = 0; i < NUM_VEC; ++i)
		fCross[i] = m_DirVec[i].x * OtherVec.z - m_DirVec[i].z * OtherVec.x;

#if 1
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("Dot  ：青" + to_string(fDot[0]) + "：緑" + to_string(fDot[1]));
	CRenderer::SetDebugString("Cross：青" + to_string(fCross[0]) + "：緑" + to_string(fCross[1]));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
#endif

	// 1つ目の方向ベクトルとの外積結果が正の角度なら範囲外
	if (fCross[0] > 0.0f)
	{
		return 0;
	}

	// 2つ目の方向ベクトルとの外積結果が負の角度なら範囲外
	if (fCross[1] < 0.0f)
	{
		return 0;
	}

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

	if (!pNewInstance)
	{
		assert(false && "扇形インスタンスの生成に失敗");
	}

	// 初期設定
	pNewInstance->Init();
	
	// JSONファイルを読み取り展開
	std::ifstream ifs("Data\\JSON\\fan_parameter.json");

	// ファイルが展開出来ていたら
	if (ifs.good())
	{
		// JSONデータをパース
		JSON Json;
		ifs >> Json;

		// 各種パラメータをデシリアライズ
		pNewInstance->SetDirection(Json["Direction"]);
		pNewInstance->SetRange(Json["Range"]);
	}
	else
	{
		assert(false && "spline_test.jsonの読み取りに失敗しました");
	}

	return pNewInstance;
}

//============================================================================
// 生成
//============================================================================
CFan* CFan::Create(D3DXVECTOR3 Pos, float fDirection, float fRange)
{
	CFan* pNewInstance = DBG_NEW CFan();

	if (!pNewInstance)
	{
		assert(false && "扇形インスタンスの生成に失敗");
	}

	// 初期設定
	pNewInstance->Init();

	// パラメータを反映
	pNewInstance->SetPos(Pos);
	pNewInstance->SetDirection(fDirection);
	pNewInstance->SetRange(fRange);

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

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CFan::Init()
{
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// 扇形の範囲を設定
	m_fRange = D3DX_PI * 0.1f;

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
	pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
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
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//============================================================================
// 頂点座標の設定
//============================================================================
void CFan::SetVtx()
{
	// 方角に合わせて範囲分の方向ベクトルを2本作成
	m_DirVec[0] = { cosf(m_fDirection + m_fRange), 0, sin(m_fDirection + m_fRange) };
	m_DirVec[1] = { cosf(m_fDirection - m_fRange), 0, sin(m_fDirection - m_fRange) };

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);
	
	// 頂点座標の設定
	pVtx[0].pos = m_DirVec[0] * CField_Manager::FIELD_RADIUS;
	pVtx[1].pos = m_Pos;
	pVtx[2].pos = m_DirVec[1] * CField_Manager::FIELD_RADIUS;
	
	// すぐにけせ
#if 0
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	for (int i = 0; i < 2; ++i)
	CRenderer::SetDebugString("方向ベクトル" + to_string(i) + "：" + to_string(m_DirVec[i].x) + "：" + to_string(m_DirVec[i].z));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
#endif

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