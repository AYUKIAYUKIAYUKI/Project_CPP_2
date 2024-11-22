//============================================================================
//
// 扇形、ヘッダファイル [fan.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _FAN_H_
#define _FAN_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// 扇形クラス
//****************************************************
class CFan
{
public:
	
	// <fuction>
	void Release();							// 破棄
	void Update();							// 更新処理
	void Draw();							// 描画処理
	bool DetectInFanRange(D3DXVECTOR3 Pos);	// 扇形範囲内にあるか検出

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// 座標を取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標を設定

	// <getter/setter>
	const float& GetDirection() const;		// 方角を取得
	void SetDirection(float fDirection);	// 方角を設定

	//// <getter/setter>
	//const float& GetLength() const;	// 長さを取得
	//void SetLength(float fLength);	// 長さを設定

	// <getter/setter>
	const float& GetRange() const;	// 範囲を取得
	void SetRange(float fRange);	// 範囲を設定

	// <staic function>
	static CFan* Create();													// 生成
	static CFan* Create(D3DXVECTOR3 Pos, float fDirection, float fRange);	// 生成

private:

	/// <summary> 描画用頂点数 </summary>
	static constexpr WORD NUM_VTX = 3;

	/// <summary> 描画用プリミティブ数 </summary>
	static constexpr WORD NUM_PRIM = 2;

	// <special fuction>
	CFan();		// コンストラクタ
	~CFan();	// デストラクタ

	// <fuction>
	HRESULT	Init();				// 初期設定
	HRESULT	CreateVtxBuff();	// 頂点バッファの生成
	void	Uninit();			// 終了処理
	void	SetVtx();			// 頂点座標の設定
	void	SetMtxWorld();		// ワールド行列設定

	// <data>
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファのポインタ
	D3DXMATRIX				m_MtxWorld;		// ワールド行列
	D3DXVECTOR3				m_Pos;			// 座標
	float					m_fDirection;	// 方角
	float					m_fRange;		// 範囲
};

#endif	// _FAN_H_