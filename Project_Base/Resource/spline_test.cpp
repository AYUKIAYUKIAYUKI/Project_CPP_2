//============================================================================
//
// スプライン曲線テスト [spline_test.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "spline_test.h"
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
// コンストラクタ
//============================================================================
CSpline_Test::CSpline_Test() :
	m_pVtxBuff{ nullptr },
	m_nNumVtx{ 0 },
	m_nNumPrim{ 0 },
	m_Pos{ VEC3_INIT },
	m_pQuadratic_Bezier{ nullptr, nullptr }
{
	// JSONファイルを読み取り展開
	std::ifstream ifs("Data\\JSON\\spline_test.json");

	// ファイルが展開出来ていたら
	if (ifs.good())
	{
		// JSONデータをパース
		ifs >> m_Json;

		// 各種パラメータをデシリアライズ
		m_nNumVtx = m_Json["Vtx"];
		m_nNumPrim = m_Json["Prim"];
	}
	else
	{
		assert(false && "spline_test.jsonの読み取りに失敗しました");
	}

	// ワールド行列の初期化
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// デストラクタ
//============================================================================
CSpline_Test::~CSpline_Test()
{
	// 念のため終了処理
	Uninit();
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CSpline_Test::Init()
{
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// 座標情報をデシリアライズ
	const auto& Pos_List = m_Json["Pos_List"];

	for (WORD i = 0; i < NUM_QUADRATIC_BEZIER; ++i)
	{
		// 座標格格納用コンテナ
		std::array<Vec3, CQuadratic_Bezier::NUM_CONTROLPOINT> ControlPoint;

		for (WORD j = 0; j < CQuadratic_Bezier::NUM_CONTROLPOINT; ++j)
		{
			// 制御点の作成
			const auto& Pos = Pos_List[j + i];				// 要素を抜き出して
			ControlPoint[j] = Vec3(Pos[0], Pos[1], Pos[2]);	// 座標を作成し代入
		}

		// 二次ベジェ曲線用を生成
		m_pQuadratic_Bezier[i] = DBG_NEW CQuadratic_Bezier(ControlPoint);
		m_pQuadratic_Bezier[i]->Init();
	}

	// 三次ベジェ曲線の生成
	m_pCubic_Bezier = DBG_NEW CCubic_Bezier(m_pQuadratic_Bezier[0]->GetQuadratic_Trajectory(), m_pQuadratic_Bezier[1]->GetQuadratic_Trajectory());
	m_pCubic_Bezier->Init();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CSpline_Test::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 二次ベジェ曲線の破棄
	for (WORD i = 0; i < NUM_QUADRATIC_BEZIER; ++i)
	{
		if (m_pQuadratic_Bezier[i] != nullptr)
		{
			m_pQuadratic_Bezier[i]->Uninit();
			delete m_pQuadratic_Bezier[i];
			m_pQuadratic_Bezier[i] = nullptr;
		}
	}

	// 三次ベジェ曲線の破棄
	if (m_pCubic_Bezier != nullptr)
	{
		m_pCubic_Bezier->Uninit();
		delete m_pCubic_Bezier;
		m_pCubic_Bezier = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CSpline_Test::Update()
{
	// 二次ベジェ曲線の更新
	for (WORD i = 0; i < NUM_QUADRATIC_BEZIER; ++i)
	{
		m_pQuadratic_Bezier[i]->Update();
	}

	// 三次ベジェ曲線の更新
	m_pCubic_Bezier->Update();

	// ワールド行列設定
	SetMtxWorld();
}

//============================================================================
// 描画処理
//============================================================================
void CSpline_Test::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();

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
		m_nNumPrim);						// プリミティブ数

	// ライトをオン
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 二次ベジェ曲線の描画
	for (WORD i = 0; i < NUM_QUADRATIC_BEZIER; ++i)
	{
		m_pQuadratic_Bezier[i]->Draw();
	}

	// 三次ベジェ曲線の描画
	m_pCubic_Bezier->Draw();
}

//============================================================================
//
// privateメンバ
//
//============================================================================

//============================================================================
// 頂点バッファを生成
//============================================================================
HRESULT CSpline_Test::CreateVtxBuff()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();

	// 頂点バッファの生成
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
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

	// 座標情報をデシリアライズ
	const auto& Pos_List = m_Json["Pos_List"];

	for (WORD i = 0; i < m_nNumVtx; ++i)
	{
		// 頂点座標の設定
		const auto& Pos = Pos_List[i];				// 要素を抜き出して
		pVtx[i].pos = Vec3(Pos[0], Pos[1], Pos[2]);	// 座標を作成し代入

		// 法線ベクトルの設定
		pVtx[i].nor = VEC3_INIT;

		// 頂点色の設定
		pVtx[i].col = XCOL_INIT;

		// テクスチャ座標の設定
		pVtx[i].tex = VEC2_INIT;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// ワールド行列設定
//============================================================================
void CSpline_Test::SetMtxWorld()
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