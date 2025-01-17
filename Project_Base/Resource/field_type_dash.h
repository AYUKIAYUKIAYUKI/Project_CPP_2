//============================================================================
//
// フィールドタイプ-ダッシュ、ヘッダファイル [field_type_dash.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _FIELD_TYPE_DASH_H_
#define _FIELD_TYPE_DASH_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "field_type.h"

//****************************************************
// フィールドタイプ-ダッシュクラス
//****************************************************
class CField_Type_Dash : public CField_Type
{
public:

	// <special function>
	CField_Type_Dash();				// コンストラクタ
	~CField_Type_Dash() override;	// デストラクタ

	// <function>
	void Update() override;	// 更新処理

	// <static function>
	static CField_Type_Dash* Create();	// 生成
};

#endif // _FIELD_TYPE_DASH_H_