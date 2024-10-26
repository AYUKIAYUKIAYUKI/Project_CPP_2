//============================================================================
//
// 二次ベジェ曲線、ヘッダファイル [quadratic_bezier.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _QUADRATIC_BEZIER_
#define _QUADRATIC_BEZIER_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// 2次ベジェ曲線クラスの定義
//****************************************************
class CQuadratic_Bezier
{
public:

	CQuadratic_Bezier(const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Pos3);	// 座標渡しコンストラクタ
	~CQuadratic_Bezier();																			// デストラクタ

	HRESULT Init();		// 初期設定
	void	Uninit();	// 終了処理
	void	Update();	// 更新処理
	void	Draw();		// 描画処理

private:

	/// <summary> 制御点の数 </summary>
	static constexpr WORD NUM_CONTROLPOINT = 3;

	/// <summary> 見た目の数 </summary>
	static constexpr WORD NUM_VISUAL = 3;

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							// 頂点バッファのポインタ
	D3DXVECTOR3				m_ControlPoint[NUM_CONTROLPOINT];	// 制御点
	float					m_fParameter;						// 進行度
	CObject_X*				m_pParameterVisual[NUM_VISUAL];		// 進行度の見た目
};

#endif	// _QUADRATIC_BEZIER_