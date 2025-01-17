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
	void Update() override;	// 更新処理

	// <static function>
	static CField_Type_Normal* Create();	// 生成
};

#endif // _FIELD_TYPE_NORMAL_H_