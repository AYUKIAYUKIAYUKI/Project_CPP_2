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
#include "object_X.h"

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

	/// <summary> ムーブカウントの最大数 </summary>
	static constexpr int MAX_MOVE_COUNT = 60;

	//****************************************************
	// 2次ベジェ曲線構造体の定義
	//****************************************************
	struct QuadraticBezier
	{
		LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// 頂点バッファのポインタ

		CObject_X*	pObject;		// 物体
		D3DXVECTOR3 PinPos[2];		// ピンとなる座標	
		D3DXVECTOR3 MagPos;			// 磁石となる座標	
		float		fCoef;			// ピン間の割合
		CObject_X*	pCoefObj[3];	// ピン間の割合表示
		//int		nMoveLine;		// 移動している線
	};

	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	HRESULT CreateIdxBuff();	// インデックスバッファの生成
	void	CalcBezier();		// ベジェ曲線を計算
	void	SetMtxWorld();		// ワールド行列設定

	JSON					m_Json;				// ジェイソンオブジェクト
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;			// 頂点バッファのポインタ
	int						m_nNumVtx;			// 頂点数
	int						m_nNumPrim;			// プリミティブ数
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;			// インデックスバッファのポインタ
	int						m_nNumIdx;			// インデックス数
	D3DXVECTOR3				m_Pos;				// 座標
	D3DXMATRIX				m_MtxWorld;			// ワールド行列
	QuadraticBezier			m_QuadraticBezier;	// 二次ベジェ曲線構造体
};

#endif	// _SPLINE_TEST_