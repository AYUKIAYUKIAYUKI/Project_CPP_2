//============================================================================
//
// 球バウンディング、ヘッダファイル [bouding_sphere.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "bounding_volume.h"
#include "render_sphere.h"

//****************************************************
// 球バウンディングクラス
//****************************************************
class CBounding_Sphere : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Sphere();				// コンストラクタ
	~CBounding_Sphere() override;	// デストラクタ

	// <setter>
	void SetCenterPos(D3DXVECTOR3 Pos) override;	// 中心点を設定

	// <getter/setter>
	const float& GetRadius() const;	// 半径を取得
	void SetRadius(float fRad);		// 半径を設定

private:

	// <data>
	float			m_fRadius;			// 半径
#ifdef _DEBUG
	CRender_Sphere* m_pRenderSphere;	// 球表示
#endif
};

#endif	// _BOUNDING_SPHERE_H_