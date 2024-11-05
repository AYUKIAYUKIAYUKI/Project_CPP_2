//============================================================================
// 
// 四角形マスク、ヘッダファイル [mask_rectangle.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _MASK_RECTANGLE_H_
#define _MASK_RECTANGLE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "texture_manager.h"

//****************************************************
// 四角形マスク
//****************************************************
class CMask_Rectangle
{
public:

	CMask_Rectangle(CTexture_Manager::TYPE Type);	// テクスチャ指定コンストラクタ
	~CMask_Rectangle();								// デストラクタ

	HRESULT	Init();		// 初期設定
	void	Uninit();	// 終了処理
	void	Update();	// 更新処理
	void	Draw();		// 描画処理

	void BindTex(LPDIRECT3DTEXTURE9 pTex);		// テクスチャ割当
	void BindTex(CTexture_Manager::TYPE Type);	// もっとテクスチャ割当

	static CMask_Rectangle* Create(CTexture_Manager::TYPE Type);	// 生成

private:

	/// <summary> 頂点数 </summary>
	static constexpr WORD NUM_VTX = 4;

	/// <summary> プリミティブ数 </summary>
	static constexpr WORD NUM_PRIM = 2;

	HRESULT CreateVtxBuff();	// 頂点バッファの生成

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9		m_pTex;			// テクスチャのポインタ
	D3DXVECTOR3				m_Pos;			// 座標
	D3DXVECTOR3				m_Rot;			// 向き
	D3DXVECTOR3				m_Size;			// サイズ
	D3DXCOLOR				m_Col;			// 色
	float					m_fLength;		// 展開用対角線
	float					m_fAngle;		// 対角線用角度
};

#endif // _MASK_RECTANGLE_H_