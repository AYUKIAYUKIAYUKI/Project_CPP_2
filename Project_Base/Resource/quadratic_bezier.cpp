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
	// 制御点の設定
	m_ControlPoint[0] = Pos1;
	m_ControlPoint[1] = Pos2;
	m_ControlPoint[2] = Pos3;

	// 軌跡の初期化
	for (WORD i = 0; i < NUM_TRAJECTORY; ++i)
	{
		m_pTrajectory[i] = nullptr;
	}
}

//============================================================================
// 座標渡しコンストラクタ
//============================================================================
CQuadratic_Bezier::CQuadratic_Bezier(const std::array<D3DXVECTOR3, NUM_CONTROLPOINT>& ControlPoint) :
	m_pVtxBuff{ nullptr },
	m_fParameter{ 0.0f }
{
	// 制御点の設定
	for (WORD i = 0; i < NUM_CONTROLPOINT; ++i)
	{
		m_ControlPoint[i] = ControlPoint[i];
	}

	// 軌跡の初期化
	for (WORD i = 0; i < NUM_TRAJECTORY; ++i)
	{
		m_pTrajectory[i] = nullptr;
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
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// 軌跡の生成
	for (WORD i = 0; i < NUM_TRAJECTORY; ++i)
	{
		m_pTrajectory[i] = CObject_X::Create();
		m_pTrajectory[i]->BindModel(CModel_X_Manager::TYPE::SPHERE);
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
	for (WORD i = 0; i < NUM_CONTROLPOINT - 1; ++i)
	{
		// 進行度に合わせて、制御点間上の軌跡の座標を更新
		m_pTrajectory[i]->SetPos(m_ControlPoint[i] + (m_ControlPoint[i + 1] - m_ControlPoint[i]) * m_fParameter);
	}

	// 軌跡同士を結ぶ線上に、さらなる軌跡を表示
	m_pTrajectory[NUM_CONTROLPOINT - 1]->SetPos(m_pTrajectory[0]->GetPos() + (m_pTrajectory[1]->GetPos() - m_pTrajectory[0]->GetPos()) * m_fParameter);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 頂点座標を制御点間の軌跡上に設定
	for (WORD i = 0; i < NUM_CONTROLPOINT - 1; ++i)
	{
		pVtx[i].pos = m_pTrajectory[i]->GetPos();
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 進行度の変動
	m_fParameter < 1.0f ? m_fParameter += MOVE_SPEED : m_fParameter = 0.0f;

#if 0
	for (WORD i = 0; i < NUM_CONTROLPOINT; ++i)
	{
		CRenderer::GetInstance()->SetDebugString(to_string(m_pTrajectory[i]->GetPos().x) + " : " + to_string(m_pTrajectory[i]->GetPos().y) + " : " + to_string(m_pTrajectory[i]->GetPos().z));
	}

	for (WORD i = 0; i < NUM_CONTROLPOINT; ++i)
	{
		CRenderer::GetInstance()->SetDebugString(to_string(m_ControlPoint[i].x) + " : " + to_string(m_ControlPoint[i].y) + " : " + to_string(m_ControlPoint[i].z));
	}
#endif
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

//============================================================================
// 二次軌跡を取得
//============================================================================
CObject_X* CQuadratic_Bezier::GetQuadratic_Trajectory() const
{
	return m_pTrajectory[2];
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 頂点バッファを生成
//============================================================================
HRESULT CQuadratic_Bezier::CreateVtxBuff()
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

	for (WORD i = 0; i < 2; ++i)
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

	return S_OK;
}