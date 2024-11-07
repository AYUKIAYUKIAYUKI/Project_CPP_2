//============================================================================
// 
// 2Dオブジェクト、ヘッダファイル [object_2D.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_2D_H_
#define _OBJECT_2D_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

// テクスチャ割り当て用
#include "texture_manager.h"

//****************************************************
// 2Dオブジェクトクラス
//****************************************************
class CObject_2D : public CObject
{
public:

	CObject_2D(LAYER Priority = LAYER::UI);		// 描画優先度指定コンストラクタ
	CObject_2D(CTexture_Manager::TYPE Type);	// テクスチャ指定コンストラクタ
	~CObject_2D() override;						// デストラクタ

	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	void BindTex(LPDIRECT3DTEXTURE9 pTex);		// テクスチャ割当
	void BindTex(CTexture_Manager::TYPE Type);	// もっとテクスチャ割当

	const D3DXVECTOR3& GetPos() const;	// 座標取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標設定

	const D3DXVECTOR3& GetRot() const;	// 向き取得
	void SetRot(D3DXVECTOR3 Rot);		// 向き設定

	const D3DXVECTOR3& GetSize() const;	// サイズ取得
	void SetSize(D3DXVECTOR3 Size);		// サイズ設定

	const D3DXCOLOR& GetCol() const;	// 色取得
	void SetCol(D3DXCOLOR Col);			// 色設定

	const float& GetAlpha() const;	// アルファ値取得
	void SetAlpha(float fAlpha);	// アルファ値設定

	const float& GetLength() const;	// 展開用対角線取得

	void SetTexWidth(float fWidth);		// 横テクスチャ分割幅設定
	void SetTexHeight(float fHeight);	// 縦テクスチャ分割幅設定

	const int& GetNowPatternU() const;		// 現在のテクスチャ横分割幅取得
	void SetNowPatternU(int nNowPatternU);	// 現在のテクスチャ横分割幅設定

	const int& GetNowPatternV() const;		// 現在のテクスチャ縦分割幅取得
	void SetNowPatternV(int nNowPatternV);	// 現在のテクスチャ縦分割幅設定

	static CObject_2D* Create();	// 生成

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
	float					m_fTexWidth;	// 横テクスチャ分割幅
	float					m_fTexHeight;	// 縦テクスチャ分縦幅
	int						m_nNowPatternU;	// 現在の横テクスチャ種類
	int						m_nNowPatternV;	// 現在の縦テクスチャ種類
};

#endif // _OBJECT_2D_H_