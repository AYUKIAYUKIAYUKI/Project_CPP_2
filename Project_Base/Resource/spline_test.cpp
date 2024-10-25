//============================================================================
//
// スプライン曲線のテスト [spline_test.cpp]
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
// デフォルトコンストラクタ
//============================================================================
CSpline_Test::CSpline_Test() :
	m_pVtxBuff{ nullptr },
	m_nNumVtx{ 0 },
	m_nNumPrim{ 0 },
	m_pIdxBuff{ nullptr },
	m_nNumIdx{ 0 },
	m_Pos{ VEC3_INIT },
	m_pMoving{ nullptr },
	m_nMoveIdx{ 0 },
	m_nMoveCnt{ 0 }
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
		m_nNumIdx = m_Json["Idx"];
		m_nNumPrim = m_Json["Prim"];
	}
	else
	{
		assert(false && "spline_test.jsonの読み取りに失敗しました");
	}

	// ワールド行列の初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 動物を生成
	m_pMoving = CObject_X::Create();
	const auto& Pos_List = m_Json["Pos_List"];
	const float& X = Pos_List[m_nMoveIdx][0], Y = Pos_List[m_nMoveIdx][1], Z = Pos_List[m_nMoveIdx][2];
	m_pMoving->SetPos(D3DXVECTOR3(X, Y, Z));
	m_pMoving->BindModel(CModel_X_Manager::TYPE::SAMUS);
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
void CSpline_Test::Uninit()
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
void CSpline_Test::Update()
{
	// 座標情報をデシリアライズ
	const auto& Pos_List = m_Json["Pos_List"];

	// 過去のインデックス、目標のインデックスから地点を抜き出す
	const auto& OldPos_Param = Pos_List[m_nMoveIdx],
		Dest_Param = Pos_List[m_nMoveIdx + 1];
	Vec3 DestPos  = D3DXVECTOR3(Dest_Param[0], Dest_Param[1], Dest_Param[2]),
		OldPos = D3DXVECTOR3(OldPos_Param[0], OldPos_Param[1], OldPos_Param[2]),
		NewPos = m_pMoving->GetPos();

	// 目標地点に移動
	NewPos += (DestPos - OldPos) / MAX_MOVE_COUNT;
	m_pMoving->SetPos(NewPos);

	// 一定期間経過で目標を更新
	m_nMoveCnt++;
	if (m_nMoveCnt > MAX_MOVE_COUNT) {
		m_nMoveIdx < (m_nNumVtx - 1) - 1 ? m_nMoveIdx++ : m_nMoveIdx = 0;
		m_nMoveCnt = 0;
	}

#ifdef _DEBUG
	CRenderer::GetInstance()->SetDebugString("移動方向 : " + to_string(m_nMoveIdx) + " -> " + to_string(m_nMoveIdx + 1));
	CRenderer::GetInstance()->SetDebugString("現在地 　: " + to_string(NewPos.x) + to_string(NewPos.y) + to_string(NewPos.z));
	CRenderer::GetInstance()->SetDebugString("目的地 　: " + to_string(DestPos.x) + to_string(DestPos.y) + to_string(DestPos.z));
#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CSpline_Test::Draw()
{
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

#if 0

	// インデックスから線の描画
	pDev->DrawIndexedPrimitive(D3DPT_LINESTRIP,	// プリミティブの種類
		0,
		0,
		m_nNumVtx,								// 頂点数
		0,
		m_nNumPrim);							// プリミティブ数
#else

	// 線の描画
	pDev->DrawPrimitive(D3DPT_LINESTRIP,	// プリミティブの種類
		0,									// 頂点情報の先頭アドレス
		m_nNumPrim);						// プリミティブ数

#endif

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
HRESULT CSpline_Test::CreateVtxBuff()
{
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
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 座標情報をデシリアライズ
	const auto& Pos_List = m_Json["Pos_List"];

	for (int i = 0; i < m_nNumVtx; i++)
	{
#if 0
		// 頂点座標の設定
		const float& X = Pos_List[i][0], Y = Pos_List[i][1], Z = Pos_List[i][2];	// 要素を抜き出して
		pVtx[i].pos = D3DXVECTOR3(X, Y, Z);											// 座標を作成し代入
#elif 1
		const auto& Pos = Pos_List[i];				// 要素を抜き出して
		pVtx[i].pos = Vec3(Pos[0], Pos[1], Pos[2]);	// 座標を作成し代入
#endif

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

//============================================================================
// インデックスバッファを生成
//============================================================================
HRESULT CSpline_Test::CreateIdxBuff()
{
	// インデックスバッファの生成
	CRenderer::GetInstance()->GetDeviece()->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
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

	// インデックスを設定
	for (int i = 0; i < m_nNumIdx; i++)
	{
		pIdx[i] = static_cast<WORD>(i);
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

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