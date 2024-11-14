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
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace abbr;

	// �����m
	bool HitSphereToSphere(const CBounding_Sphere* const Self, const CBounding_Sphere* const Other);

	// ����AABB
	bool HitSphereToAABB(const CBounding_Sphere* const Self, const CBounding_Box* const Other);

	// �~�����m
	bool HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other);

	// �~���Ɠ_
	bool HitCylinderToPoint(const CBounding_Cylinder* const Self, const Vec3& Other);

	// �~���Ƌ�
	bool HitCylinderToSphere(const CBounding_Cylinder* const Self, const CBounding_Sphere* const Other);
	bool HitCylinderToSphere(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const float& fOtherRadius);

	// �~����AABB
	bool HitCylinderToAABB(const CBounding_Cylinder* const Self, const CBounding_Box* const Other);
	bool HitCylinderToAABB(const Vec3& SelfPos, const float& fSelfRadius, const float& fHeight, const Vec3& PosTarget, const Vec3& SizeTarget);

	// �~����AABB�̏Փ˖ʂ��擾
	int GetCylinderToAABB(const Vec3& SelfOldPos, const Vec3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize);
};

#endif // _COLLISION_H_