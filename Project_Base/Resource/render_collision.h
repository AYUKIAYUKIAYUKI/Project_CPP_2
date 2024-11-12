//============================================================================
// 
// 判定表示、ヘッダファイル [render_collision.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _RENDER_COLLISION_H_
#define _RENDER_COLLISION_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// 前方宣言
//****************************************************
class CObject_X;

//****************************************************
// 判定表示クラス
//****************************************************
class CRender_Collision : public CObject
{
public:

	// <special function>
	CRender_Collision(LAYER Priority = LAYER::FRONT);	// 描画優先度指定コンストラクタ
	~CRender_Collision() override;						// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	
	// <virtual function>
	virtual void Draw() override = 0;	// 描画処理

	// <setter>
	void SetRefObj(CObject_X* pRef);	// 対象オブジェクトの設定

protected:

	// <data>
	CObject_X*	m_pRef;		// 対象オブジェクト
	D3DXMATRIX	m_MtxWorld;	// ワールド行列

private:

	// <function>
	void SetMtxWorld();	// ワールド行列設定
};

#endif	// _RENDER_COLLISION_H_