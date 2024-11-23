//============================================================================
//
// 三次ベジェ曲線 [cubic_bezier.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "cubic_bezier.h"
#include "quadratic_bezier.h"
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
// 二次軌跡渡しコンストラクタ
//============================================================================
CCubic_Bezier::CCubic_Bezier(CObject_X* p1, CObject_X* p2) :
	m_pVtxBuff{ nullptr },
	m_fParameter{ 0.0f },
	m_pQuadratic_Trajectory{ p1, p2 },
	m_pCubic_Trajectory{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CCubic_Bezier::~CCubic_Bezier()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CCubic_Bezier::Init()
{
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// 三次軌跡の生成
	m_pCubic_Trajectory = CObject_X::Create();
	m_pCubic_Trajectory->BindModel(CX_Manager::TYPE::SPHERE);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CCubic_Bezier::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CCubic_Bezier::Update()
{
	// 二次軌跡同士を結ぶ線上に、三次軌跡を表示
	m_pCubic_Trajectory->SetPos(m_pQuadratic_Trajectory[0]->GetPos() + (m_pQuadratic_Trajectory[1]->GetPos() - m_pQuadratic_Trajectory[0]->GetPos()) * m_fParameter);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 頂点座標を制御点間の軌跡上に設定
	for (WORD i = 0; i < NUM_CONTROLPOINT - 1; ++i)
	{
		pVtx[i].pos = m_pQuadratic_Trajectory[i]->GetPos();
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 進行度の変動
	m_fParameter < 1.0f ? m_fParameter += MOVE_SPEED : m_fParameter = 0.0f;
}

//============================================================================
// 描画処理
//============================================================================
void CCubic_Bezier::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ライトをオフ
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点バッファをデータストリームに設定
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDev->SetFVF(FVF_VERTEX_3D);

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
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 頂点バッファを生成
//============================================================================
HRESULT CCubic_Bezier::CreateVtxBuff()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 頂点バッファの生成
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,
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

	for (WORD i = 0; i < 2; ++i)
	{
		// 頂点座標を設定
		pVtx[i].pos = VEC3_INIT;

		// 法線ベクトルの設定
		pVtx[i].nor = VEC3_INIT;

		// 頂点色の設定
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);;

		// テクスチャ座標の設定
		pVtx[i].tex = VEC2_INIT;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}