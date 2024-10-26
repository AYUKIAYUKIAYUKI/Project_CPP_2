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

	/// <summary> 制御点の数 </summary>
	static constexpr int NUM_CONTROLPOINT = 3;

	CQuadratic_Bezier(const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Pos3);	// 座標渡しコンストラクタ
	CQuadratic_Bezier(const std::array<D3DXVECTOR3, NUM_CONTROLPOINT>& ControlPoint);				// 座標渡しコンストラクタ
	~CQuadratic_Bezier();																			// デストラクタ

	HRESULT Init();		// 初期設定
	void	Uninit();	// 終了処理
	void	Update();	// 更新処理
	void	Draw();		// 描画処理

private:

	/// <summary> 軌跡の数 </summary>
	static constexpr WORD NUM_TRAJECTORY = 3;

	/// <summary> 移動速度 </summary>
	static constexpr float MOVE_SPEED = 0.005f;

	HRESULT CreateVtxBuff();	// 頂点バッファを生成

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							// 頂点バッファのポインタ
	D3DXVECTOR3				m_ControlPoint[NUM_CONTROLPOINT];	// 制御点
	float					m_fParameter;						// 進行度
	CObject_X*				m_pTrajectory[NUM_TRAJECTORY];		// 軌跡
};

#endif	// _QUADRATIC_BEZIER_