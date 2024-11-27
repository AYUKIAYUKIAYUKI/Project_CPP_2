//============================================================================
//
// バウンディングスフィア、ヘッダファイル [bouding_sphere.h]
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
#include "X_manager.h"

//****************************************************
// バウンディングスフィアクラス
//****************************************************
class CBounding_Sphere : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Sphere();					// コンストラクタ
	CBounding_Sphere(CObject_X* pObj);	// スフィア表示コンストラクタ
	~CBounding_Sphere() override;		// デストラクタ

	// <setter>
	void ChangeModel(CX_Manager::TYPE Type);	// モデルを変更

	// <getter/setter>
	const float& GetRadius() const;					// 半径を取得
	void SetRadius(float fRad);						// 半径を設定
	void SetCenterPos(D3DXVECTOR3 Pos) override;	// 中心点を設定

private:

	// <data>
	float			m_fRadius;			// 半径
	CRender_Sphere* m_pRenderSphere;	// スフィア表示
};

#endif	// _BOUNDING_SPHERE_H_