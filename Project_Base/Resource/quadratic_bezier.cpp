//============================================================================
//
// 二次ベジェ曲線 [quadratic_bezier.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
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
// 座標渡しコンストラクタ
//============================================================================
CQuadratic_Bezier::CQuadratic_Bezier(const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Pos3) :
	m_pVtxBuff{ nullptr },
	m_fParameter{ 0.0f }
{
	// 制御点の初期化
	m_ControlPoint[0] = Pos1;
	m_ControlPoint[1] = Pos2;
	m_ControlPoint[2] = Pos3;

	// 見た目の初期化
	for (int i = 0; i < NUM_VISUAL; ++i)
	{
		m_pParameterVisual[i] = nullptr;
	}
}

//============================================================================
// デストラクタ
//============================================================================
CQuadratic_Bezier::~CQuadratic_Bezier()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CQuadratic_Bezier::Init()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

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

	for (int i = 0; i < 2; i++)
	{
		// 頂点座標を設定
		pVtx[i].pos = VEC3_INIT;

		// 法線ベクトルの設定
		pVtx[i].nor = VEC3_INIT;

		// 頂点色の設定
		pVtx[i].col = XCOl_INIT;

		// テクスチャ座標の設定
		pVtx[i].tex = VEC2_INIT;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	for (int i = 0; i < NUM_VISUAL; ++i)
	{
		m_pParameterVisual[i] = CObject_X::Create();
		m_pParameterVisual[i]->BindModel(CModel_X_Manager::TYPE::SPHERE);
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CQuadratic_Bezier::Uninit()
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
void CQuadratic_Bezier::Update()
{
	Vec3 MovePos1 = m_ControlPoint[0] + (m_ControlPoint[1] - m_ControlPoint[0]) * m_fParameter;
	m_pParameterVisual[0]->SetPos(MovePos1);

	Vec3 MovePos2 = m_ControlPoint[1] + (m_ControlPoint[2] - m_ControlPoint[1]) * m_fParameter;
	m_pParameterVisual[1]->SetPos(MovePos2);

	Vec3 MovePos3 = MovePos1 + (MovePos2 - MovePos1) * m_fParameter;
	m_pParameterVisual[2]->SetPos(MovePos3);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 頂点座標を設定
	pVtx[0].pos = MovePos1;
	pVtx[1].pos = MovePos2;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 進行度の変動
	m_fParameter < 1.0f ? m_fParameter += 0.01f : m_fParameter = 0.0f;
}

//============================================================================
// 描画処理
//============================================================================
void CQuadratic_Bezier::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

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