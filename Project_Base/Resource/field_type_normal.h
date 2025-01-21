//============================================================================
//
// フィールドタイプ-ノーマル、ヘッダファイル [field_type_normal.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _FIELD_TYPE_NORMAL_H_
#define _FIELD_TYPE_NORMAL_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "field_type.h"
#include "X_manager.h"

//****************************************************
// フィールドタイプ-ノーマルクラス
//****************************************************
class CField_Type_Normal : public CField_Type
{
public:

	// <special function>
	CField_Type_Normal();			// コンストラクタ
	~CField_Type_Normal() override;	// デストラクタ

	// <function>
	void GenerateBlock(float) override;	// 更新処理

	// <static function>
	static CField_Type_Normal* Create();	// 生成

private:

	// <fuction>
	bool DetectOverlapItem(D3DXVECTOR3);					// アイテムとの重複を検出
	bool DetectOverlapBlock(CX_Manager::TYPE, D3DXVECTOR3);	// ブロックの重複を検出

	// <data>
	CX_Manager::TYPE m_LastModel;	// 最後のモデル
};

#endif // _FIELD_TYPE_NORMAL_H_