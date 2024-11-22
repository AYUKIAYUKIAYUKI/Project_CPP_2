//============================================================================
//
// メッシュドーム、ヘッダファイル [mesh_dome.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _MESH_DOME_H_
#define _MESH_DOME_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// メッシュドームクラス
//****************************************************
class CMesh_Dome : public CObject
{
public:

	// <special funvtion>
	CMesh_Dome();	// コンストラクタ
	~CMesh_Dome();	// デストラクタ

	// <function>
	HRESULT Init() override;		// 初期設定
	void	Uninit() override;		// 終了設定
	void	Update() override;		// 更新処理
	void	Draw() override;		// 描画処理

	// <static function>
	static CMesh_Dome* Create();	// 生成

private:

	// <function>
	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	HRESULT CreateIdxBuff();	// インデックスバッファの生成
	void	SetVtx();			// 頂点の設定
	void	SetMtxWorld();		// ワールド行列の設定

	// <data>
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// インデックスバッファのポインタ
	D3DXVECTOR3				m_Pos;		// 座標
	D3DXMATRIX				m_MtxWorld;	// ワールド行列
};

#endif // _MESH_DOME_H_