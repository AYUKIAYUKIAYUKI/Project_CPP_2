//============================================================================
//
// バウンディングシリンダー、ヘッダファイル [bouding_cylinder.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _BOUNDING_CYLINDER_H_
#define _BOUNDING_CYLINDER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "bounding_volume.h"
#include "render_cylinder.h"
#include "X_manager.h"

//****************************************************
// バウンディングシリンダークラス
//****************************************************
class CBounding_Cylinder : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Cylinder();					// コンストラクタ
	CBounding_Cylinder(CObject_X* pObj);	// シリンダー表示コンストラクタ
	~CBounding_Cylinder() override;			// デストラクタ

	// <setter>
	void ChangeModel(CX_Manager::TYPE Type);	// モデルを変更

	// <getter/setter>
	const float& GetRadius() const;	// 半径を取得
	void SetRadius(float fRad);		// 半径を設定

	// <getter/setter>
	const float& GetHeight() const;	// 高さを取得
	void SetHeight(float fHeight);	// 高さを設定

private:

	// <data>
	float m_fRadius;						// 半径
	float m_fHeight;						// 高さ
	CRender_Cylinder* m_pRenderCylinder;	// シリンダー表示
};

#endif	// _BOUNDING_CYLINDER_H_