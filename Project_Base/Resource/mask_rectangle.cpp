//============================================================================
// 
// 四角形マスク [mask_rectangle.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "mask_rectangle.h"

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
// テクスチャ指定
//============================================================================
CMask_Rectangle::CMask_Rectangle(CTexture_Manager::TYPE Type) :
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_Pos{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Size{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_fLength{ 0.0f },
	m_fAngle{ 0.0f }
{
	// テクスチャ割当
	BindTex(Type);
}

//============================================================================
// デストラクタ
//============================================================================
CMask_Rectangle::~CMask_Rectangle()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CMask_Rectangle::Init()
{
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// 座標の設定
	m_Pos = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };

	// サイズの設定
	m_Size = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMask_Rectangle::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();	// 頂点バッファの解放
		m_pVtxBuff = nullptr;	// ポインタの初期化
	}
}

//============================================================================
// 更新処理
//============================================================================
void CMask_Rectangle::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // 頂点バッファが消失
		assert(false && "2Dオブジェクトの頂点バッファが消失");
	}

	// 必要な数値を算出
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);
	m_fAngle = atan2f(m_Size.x, m_Size.y);

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 頂点座標の設定
	pVtx[0].pos = {
		m_Pos.x + sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		m_Pos.y + cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[1].pos = {
		m_Pos.x + sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		m_Pos.y + cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[2].pos = {
		m_Pos.x + sinf(m_Rot.z - m_fAngle) * m_fLength,
		m_Pos.y + cosf(m_Rot.z - m_fAngle) * m_fLength,
		0.0f
	};

	pVtx[3].pos = {
		m_Pos.x + sinf(m_Rot.z + m_fAngle) * m_fLength,
		m_Pos.y + cosf(m_Rot.z + m_fAngle) * m_fLength,
		0.0f
	};

#if 0
#ifdef _DEBUG

	for (int i = 0; i < 4; ++i)
	{
		CRenderer::SetDebugString("頂点座標 : " + std::to_string(pVtx[i].pos.x) + " :  " + std::to_string(pVtx[i].pos.y) + " : " + std::to_string(pVtx[i].pos.z));
	}

#endif	// _DEBUG
#endif

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================================
// 描画処理
//============================================================================
void CMask_Rectangle::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 頂点バッファをデータストリームに設定
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDev->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDev->SetTexture(0, m_pTex);

	// ポリゴンの描画
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,										// 頂点情報の先頭アドレス
		NUM_PRIM);								// プリミティブ数
}

//============================================================================
// テクスチャ割当
//============================================================================
void CMask_Rectangle::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// もっとテクスチャ割当
//============================================================================
void CMask_Rectangle::BindTex(CTexture_Manager::TYPE Type)
{
	m_pTex = CTexture_Manager::GetInstance()->GetTexture(Type);
}

//============================================================================
// 生成
//============================================================================
CMask_Rectangle* CMask_Rectangle::Create(CTexture_Manager::TYPE Type)
{
	// インスタンスを生成
	CMask_Rectangle* pNewInstance = DBG_NEW CMask_Rectangle(Type);

	if (pNewInstance == nullptr)
	{ 
		assert(false && "四角形マスクの生成に失敗");
	}

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
// 頂点バッファの生成
//============================================================================
HRESULT CMask_Rectangle::CreateVtxBuff()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 頂点バッファの生成
	pDev->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (m_pVtxBuff == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// 頂点座標の設定
		pVtx[wNumVtx].pos = VEC3_INIT;

		// 除算数の設定
		pVtx[wNumVtx].rhw = 1.0f;

		// 頂点色の設定
		pVtx[wNumVtx].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[wNumVtx].tex = VEC2_INIT;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}