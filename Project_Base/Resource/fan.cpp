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
	CRenderer::SetDebugString("扇形の方角 : " + to_string(m_fDirection));
	CRenderer::SetDebugString("方角のベク : " + to_string(sinf(m_fDirection)) + " : " + to_string(cosf(m_fDirection)));
	CRenderer::SetDebugString("扇形の長さ : " + to_string(m_fLength));
	CRenderer::SetDebugString("扇形の範囲 : " + to_string(m_fRange));
	CRenderer::SetDebugString("範囲のベク : " + to_string(cosf(m_fRange)));
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
		1);									// プリミティブ数

	// ライトをオン
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================================
// 扇形範囲内にあるか検出
//============================================================================
bool CFan::DetectInFanRange(D3DXVECTOR3 Pos)
{
	// 扇形の始点から特定座標へのベクトルを計算
	Vec3 VecPoint = Pos - m_Pos;

	// 範囲内に存在しているか、小数値を切り詰めて精密な判定を行う
	if (static_cast<int>(sqrtf(VecPoint.x * VecPoint.x + VecPoint.z * VecPoint.z)) <= m_fLength)
	{
		// 座標ベクトルを正規化
		D3DXVec3Normalize(&VecPoint, &VecPoint);

		// 方角から方向ベクトルを計算
		Vec3 VecDir = Vec3(sinf(m_fDirection), 0.0f, cosf(m_fDirection));

		// 方向ベクトルを正規化
		D3DXVec3Normalize(&VecDir, &VecDir);

		// 方向ベクトルと座標ベクトルの内積を行い、扇の範囲をcosで求める
		float fθ = D3DXVec3Dot(&VecDir, &VecPoint);

		// 扇の範囲内に存在していれば
		if (cosf(m_fRange) <= fθ ||
			cosf(m_fRange) >= fθ)
		{
			CRenderer::SetDebugString("内積のなす角 : " + to_string(fθ));

			return 1;
		}
	}

	return 0;
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
// 長さを取得
//============================================================================
const float& CFan::GetLength() const
{
	return m_fLength;
}

//============================================================================
// 長さを設定
//============================================================================
void CFan::SetLength(float fLength)
{
	m_fLength = fLength;
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
		pNewInstance->SetLength(Json["Length"]);
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
CFan* CFan::Create(D3DXVECTOR3 Pos, float fDirection, float fLength, float fRange)
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
	pNewInstance->SetLength(fLength);
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
	m_fDirection{ 0.0f },
	m_fLength{ 0.0f },
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

		// 頂点色の設定
		pVtx[i].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[i].tex = VEC2_INIT;
	}

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
	// 方角から方向ベクトルを計算
	Vec3 VecDir = Vec3(sinf(m_fDirection), 0.0f, cosf(m_fDirection));

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 頂点座標の設定
	pVtx[0].pos = m_Pos;
	pVtx[1].pos = VecDir * 350.0f;

	//for (WORD i = 0; i < NUM_VTX; ++i)
	//{
	//	CRenderer::SetDebugString("オオオオオ : " + to_string(pVtx[i].pos.x) + " : " + to_string(pVtx[i].pos.y) + " : " + to_string(pVtx[i].pos.z));
	//}

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