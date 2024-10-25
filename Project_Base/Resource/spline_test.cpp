//============================================================================
//
// スプライン曲線？のテスト [spline_test.cpp]
// Author : 福田歩希
//
//============================================================================

#include "spline_test.h"
#include "renderer.h"

// 初期設定
void CSpline::Init() {
	// JSONファイルを読み取り展開
	std::ifstream ifs("Data\\JSON\\spline_test.json");

	// ファイルが展開出来ていたら
	if (ifs.good())
	{
		// シリアライズ化
		ifs >> m_json;

		// パラメータを読み込む
		m_nNumVtx = m_json["Vtx"];
		m_nNumIdx = m_json["Idx"];
		m_nNumPrim = m_json["Prim"];
	}
	else
	{
		assert(false && "spline_test.jsonの読み取りに失敗しました");
	}

	// 頂点を生成
	CreateVtxBuff();

	// インデックスを生成
	CreateIdxBuff();
}

// 終了処理
void CSpline::Uninit() {
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

// 更新処理
void CSpline::Update() {

}

// 描画処理
void CSpline::Draw() {
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

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

	//// ボックスの描画
	//pDev->DrawIndexedPrimitive(D3DPT_LINELIST,
	//	0,
	//	0,
	//	m_nNumVtx,			// 頂点数
	//	0,
	//	m_nNumPrimitive);	// 辺の数

	// スプラインの描画
	pDev->DrawPrimitive(D3DPT_LINELIST,	// プリミティブの種類
		0,								// 頂点情報の先頭アドレス
		m_nNumPrim);					// プリミティブ数

	// ライトをオン
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

// 頂点を生成
HRESULT CSpline::CreateVtxBuff() {
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

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
	VERTEX_3D* pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	auto Pos_List = m_json["Pos_List"];

	for (int i = 0; i < m_nNumVtx; i++)
	{
		// 座標の設定
		float X = Pos_List[i][0], Y = Pos_List[i][1], Z = Pos_List[i][2];
		pVtx[i].pos = D3DXVECTOR3(X, Y, Z);

		// 法線ベクトルの設定
		pVtx[i].nor = { 0.0f, 0.0f, 0.0f };

		// 色の設定
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャの設定
		pVtx[i].tex = { 0.0f, 0.0f };
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

// インデックスを生成
HRESULT CSpline::CreateIdxBuff(){
	return S_OK;
}

//============================================================================
// ワールド行列設定
//============================================================================
void CSpline::SetMtxWorld()
{
	// 計算用行列
	D3DXMATRIX mtxRot, mtxTrans;

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