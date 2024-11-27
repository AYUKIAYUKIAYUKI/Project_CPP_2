//============================================================================
// 
// 箱表示、ヘッダファイル [render_box.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _RENDER_BOX_H_
#define _RENDER_BOX_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "render_collision.h"

//****************************************************
// ボックス表示クラス
//****************************************************
class CRender_Box : public CRender_Collision
{
public:

	// <special function>
	CRender_Box(LAYER Priority = LAYER::FRONT);	// 描画優先度指定コンストラクタ
	~CRender_Box() override;					// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <setter>
	void SetSyncSize(D3DXVECTOR3 Size);	// シンクロサイズを設定
	void SetSyncRot(D3DXVECTOR3 Rot);	// シンクロ向きを設定

	// <static function>
	static CRender_Box* Create();	// 生成

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

	// <data>
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// インデックスバッファのポインタ
	D3DXVECTOR3				m_SyncSize;	// シンクロサイズ
	D3DXVECTOR3				m_SyncRot;	// シンクロ向き
};

#endif	// _RENDER_BOX_H_