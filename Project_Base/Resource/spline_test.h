//============================================================================
//
// スプライン曲線テスト、ヘッダファイル [spline_test.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _SPLINE_TEST_
#define _SPLINE_TEST_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "quadratic_bezier.h"

//****************************************************
// スプラインテストクラス
//****************************************************
class CSpline_Test
{
public:

	CSpline_Test();		// デフォルトコンストラクタ
	~CSpline_Test();	// デストラクタ

	HRESULT Init();		// 初期設定
	void	Uninit();	// 終了処理
	void	Update();	// 更新処理
	void	Draw();		// 描画処理

private:

	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	void	SetMtxWorld();		// ワールド行列設定

	JSON					m_Json;					// ジェイソンオブジェクト
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;				// 頂点バッファのポインタ
	int						m_nNumVtx;				// 頂点数
	int						m_nNumPrim;				// プリミティブ数
	D3DXVECTOR3				m_Pos;					// 座標
	D3DXMATRIX				m_MtxWorld;				// ワールド行列
	CQuadratic_Bezier*		m_pQuadratic_Bezier;	// 二次ベジェ曲線
};

#endif	// _SPLINE_TEST_