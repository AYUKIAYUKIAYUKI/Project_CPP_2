//============================================================================
//
// 三次ベジェ曲線、ヘッダファイル [cubic_bezier.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _CUBIC_BEZIER_
#define _CUBIC_BEZIER_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// 3次ベジェ曲線クラスの定義
//****************************************************
class CCubic_Bezier
{
public:

	/// <summary> 制御点の数 </summary>
	static constexpr int NUM_CONTROLPOINT = 3;

	CCubic_Bezier(CObject_X* p1, CObject_X* p2);	// 二次軌跡渡しコンストラクタ
	~CCubic_Bezier();								// デストラクタ

	HRESULT Init();		// 初期設定
	void	Uninit();	// 終了処理
	void	Update();	// 更新処理
	void	Draw();		// 描画処理

private:

	/// <summary> 移動速度 </summary>
	static constexpr float MOVE_SPEED = 0.005f;

	HRESULT CreateVtxBuff();	// 頂点バッファを生成

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;					// 頂点バッファのポインタ
	float					m_fParameter;				// 進行度
	CObject_X* 				m_pQuadratic_Trajectory[2];	// 二次軌跡
	CObject_X*				m_pCubic_Trajectory;		// 三次軌跡
};

#endif	// _QUADRATIC_BEZIER_