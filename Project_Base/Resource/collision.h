//============================================================================
// 
// コリジョン系関数、ヘッダファイル [collision.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _COLLISION_H_
#define _COLLISION_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "bounding_box.h"
#include "bounding_cylinder.h"
#include "bounding_sphere.h"

//****************************************************
// 名前空間collisionを定義
//****************************************************
namespace collision
{
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace abbr;

	// 球同士
	bool HitSphereToSphere(const CBounding_Sphere* const Self, const CBounding_Sphere* const Other);

	// 球とAABB
	bool HitSphereToAABB(const CBounding_Sphere* const Self, const CBounding_Box* const Other);

	// 円柱同士
	bool HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other);

	// 円柱と点
	bool HitCylinderToPoint(const CBounding_Cylinder* const Self, const Vec3& Other);

	// 円柱と球
	bool HitCylinderToSphere(const CBounding_Cylinder* const Self, const CBounding_Sphere* const Other);
	bool HitCylinderToSphere(const D3DXVECTOR3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const D3DXVECTOR3& OtherPos, const float& fOtherRadius);

	// 円柱とAABB
	bool HitCylinderToAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTarget);

	// 円柱とAABB
	int GetCylinderAndAABB(const D3DXVECTOR3& SelfOldPos, const D3DXVECTOR3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const D3DXVECTOR3& OtherPos, const D3DXVECTOR3& OtherSize);
};

#endif // _COLLISION_H_