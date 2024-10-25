//============================================================================
//
// スプライン曲線のテスト、ヘッダファイル [spline_test.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _SPLINE_TEST_
#define _SPLINE_TEST_	// 二重インクルード防止

//****************************************************
// スプラインテストクラス
//****************************************************
class CSpline_Test
{
public:

	CSpline_Test() :m_pVtxBuff{ nullptr }, m_pIdxBuff{ nullptr }, m_Pos{ 0.0f, 0.0f, 0.0f } {// ワールド行列の初期化
		D3DXMatrixIdentity(&m_MtxWorld); };	// デフォルトコンストラクタ
	~CSpline_Test() { Uninit(); };	// デストラクタ

	HRESULT Init();		// 初期設定
	void	Uninit();	// 終了処理
	void	Update();	// 更新処理
	void	Draw();		// 描画処理

private:

	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	HRESULT CreateIdxBuff();	// インデックスバッファの生成
	void	SetMtxWorld();		// ワールド行列設定

	JSON					m_Json;		// ジェイソンオブジェクト
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// 頂点バッファのポインタ
	int						m_nNumVtx;	// 頂点数
	int						m_nNumPrim;	// プリミティブ数
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// インデックスバッファのポインタ
	int						m_nNumIdx;	// インデックス数
	D3DXVECTOR3				m_Pos;		// 座標
	D3DXMATRIX				m_MtxWorld;	// ワールド行列
};

#endif	// _SPLINE_TEST_