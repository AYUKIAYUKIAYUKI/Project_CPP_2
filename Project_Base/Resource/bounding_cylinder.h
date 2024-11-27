//============================================================================
//
// 円柱バウンディング、ヘッダファイル [bouding_cylinder.h]
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

//****************************************************
// 円柱バウンディングクラス
//****************************************************
class CBounding_Cylinder : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Cylinder();			// コンストラクタ
	~CBounding_Cylinder() override;	// デストラクタ

	// <setter>
	void SetCenterPos(D3DXVECTOR3 Pos) override;	// 中心点を設定

	// <getter/setter>
	const float& GetRadius() const;	// 半径を取得
	void SetRadius(float fRad);		// 半径を設定

	// <getter/setter>
	const float& GetHeight() const;	// 高さを取得
	void SetHeight(float fHeight);	// 高さを設定

private:

	// <data>
	float			  m_fRadius;			// 半径
	float			  m_fHeight;			// 高さ
#ifdef _DEBUG
	CRender_Cylinder* m_pRenderCylinder;	// シリンダー表示
#endif // _DEBUG
};

#endif	// _BOUNDING_CYLINDER_H_