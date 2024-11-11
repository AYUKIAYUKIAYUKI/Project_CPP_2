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

//****************************************************
// バウンディングスフィアクラス
//****************************************************
class CBounding_Sphere : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Sphere();				// デフォルトコンストラクタ
	~CBounding_Sphere() override;	// デストラクタ

	// <getter/setter>
	const float& GetRadius() const;	// 半径を取得
	void SetRadius(float fRad);		// 半径を設定

private:

	// <data>
	float m_fRadius;	// 半径
};

#endif	// _BOUNDING_SPHERE_H_