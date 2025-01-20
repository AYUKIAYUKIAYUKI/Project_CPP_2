//============================================================================
//
// フィールドタイプ、ヘッダファイル [field_type.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _FIELD_TYPE_H_
#define _FIELD_TYPE_H_	// 二重インクルード防止

//****************************************************
// フィールドタイプクラス
//****************************************************
class CField_Type
{
public:

	/// <summary> ブロック占有範囲 </summary>
	static D3DXVECTOR3 BLOCK_RANGE;

	// <special function>
	CField_Type();			// コンストラクタ
	virtual ~CField_Type();	// デストラクタ

	// <function>
	virtual void GenerateBlock(float) = 0;	// ブロック発生

	// <static function>
	static float GetAreaNorm();	// 占有範囲の大きさを取得

private:

	// <static function>
	static D3DXVECTOR3 SetBlockRange();	// ブロック占有範囲を設定
};

#endif // _FIELD_TYPE_H_