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
	void	Draw() override;	// 描画処理

	// <setter>
	void SetRefObj(CObject_X* pRef);	// 対象オブジェクトの設定

	// <static function>
	static CRender_Collision* Create(CObject_X* pRef);	// 生成

private:

	/// <summary> 頂点数 </summary>
	static constexpr WORD NUM_VTX = 8;
	
	/// <summary> プリミティブ数 </summary>
	static constexpr WORD NUM_PRIM = 12;
	
	/// <summary> インデックス数 </summary>
	static constexpr WORD NUM_IDX = 24;

	// <function>
	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	HRESULT CreateIdxBuff();	// インデックスバッファの生成
	void	SetVtx();			// 頂点の設定
	void	SetMtxWorld();		// ワールド行列設定

	// <data>
	CObject_X*				m_pRef;		// 対象オブジェクト
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// インデックスバッファのポインタ
	D3DXCOLOR				m_Col;		// 色
	D3DXMATRIX				m_MtxWorld;	// ワールド行列
};

#endif	// _RENDER_COLLISION_H_