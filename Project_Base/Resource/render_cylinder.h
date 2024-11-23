//============================================================================
// 
// シリンダー表示、ヘッダファイル [render_cylinder.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _RENDER_CYLINDER_H_
#define _RENDER_CYLINDER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "render_collision.h"
#include "X_manager.h"

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
	void ChangeModel(CX_Manager::TYPE Type);	// モデルを変更

	// <static function>
	static CRender_Cylinder* Create(CObject_X* pRef);	// 生成

private:

	// <data>
	CObject_X* m_pCylinder;	// シリンダー
};

#endif	// _RENDER_CYLINDER_H_