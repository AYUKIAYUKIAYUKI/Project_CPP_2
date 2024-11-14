//============================================================================
// 
// �R���W�����n�֐��A�w�b�_�t�@�C�� [collision.h]
// Author : ���c����
// 
//============================================================================

#ifndef _COLLISION_H_
#define _COLLISION_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bounding_box.h"
#include "bounding_cylinder.h"
#include "bounding_sphere.h"

//****************************************************
// ���O���collision���`
//****************************************************
namespace collision
{
	// �����m
	bool HitSphereToSphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget);

	// ����AABB
	bool HitSphereToAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTaret);

	// �~�����m
	bool HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other);

	// �~���Ɠ_
	bool HitCylinderToPoint(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget);

	// �~���Ƌ�
	bool HitCylinderToSphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget);

	// �~����AABB
	bool HitCylinderToAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTarget);

	// �~����AABB
	int GetCylinderAndAABB(const D3DXVECTOR3& SelfOldPos, const D3DXVECTOR3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const D3DXVECTOR3& OtherPos, const D3DXVECTOR3& OtherSize);
};

#endif // _COLLISION_H_