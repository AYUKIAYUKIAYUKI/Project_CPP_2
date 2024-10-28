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
	
	void Release();							// 破棄
	bool DetectInFanRange(D3DXVECTOR3 Pos);	// 扇形範囲内にあるか検出

	const D3DXVECTOR3& GetPos() const;	// 座標を取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標を設定

	const float& GetDirection() const;		// 方角を取得
	void SetDirection(float fDirection);	// 方角を設定

	const float& GetLength() const;	// 長さを取得
	void SetLength(float fLength);	// 長さを設定

	const float& GetRange() const;	// 範囲を取得
	void SetRange(float fRange);	// 範囲を設定

	static CFan* Create();																	// 生成
	static CFan* Create(D3DXVECTOR3 Pos, float fDirection, float fLength, float fRange);	// 生成

private:

	CFan();		// デフォルトコンストラクタ
	~CFan();	// デストラクタ

	D3DXVECTOR3 m_Pos;			// 座標
	float		m_fDirection;	// 方角
	float		m_fLength;		// 長さ
	float		m_fRange;		// 範囲
};

#endif	// _FAN_H_