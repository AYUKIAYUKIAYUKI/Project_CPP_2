//============================================================================
// 
// 円柱表示、ヘッダファイル [render_cylinder.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _RENDER_CYLINDER_H_
#define _RENDER_CYLINDER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "render_collision.h"

//****************************************************
// シリンダー判定表示クラス
//****************************************************
class CRender_Cylinder : public CRender_Collision
{
public:

	// <special function>
	CRender_Cylinder(LAYER Priority = LAYER::FRONT);	// 描画優先度指定コンストラクタ
	~CRender_Cylinder() override;						// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <setter>
	void SetSyncRadius(float fRadius);	// シンクロ半径を設定
	void SetSyncHeight(float fHeight);	// シンクロ高さを設定

	// <static function>
	static CRender_Cylinder* Create();	// 生成

private:

	// <data>
	float	   m_fSyncRadius;		// シンクロ半径
	float	   m_fSyncHeight;		// シンクロ高さ
	CObject_X* m_pCylinderModel;	// 円柱モデル
};

#endif	// _RENDER_CYLINDER_H_