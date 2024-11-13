//============================================================================
// 
// スフィア表示、ヘッダファイル [render_sphere.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _RENDER_SPHERE_H_
#define _RENDER_SPHERE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "render_collision.h"

//****************************************************
// 判定表示クラス
//****************************************************
class CRender_Sphere : public CRender_Collision
{
public:

	// <special function>
	CRender_Sphere(LAYER Priority = LAYER::FRONT);	// 描画優先度指定コンストラクタ
	~CRender_Sphere() override;					// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <setter>
	void SetColRenderSphere(D3DXCOLOR Col);	// スフィア表示の色を設定

	// <static function>
	static CRender_Sphere* Create(CObject_X* pRef);	// 生成

private:

	// <data>
	CObject_X* m_pSphere;	// スフィア
};

#endif	// _RENDER_SPHERE_H_