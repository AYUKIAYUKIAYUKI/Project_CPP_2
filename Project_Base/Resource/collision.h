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
	// ���ǂ����̏Փ˔���
	bool OnlySphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget);

	// ����AABB�̓����蔻��
	bool SphereAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTaret);

	// �~���Ɠ_�̏Փ˔���
	bool CylinderAndPoint(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget);

	// �~���Ƌ��̏Փ˔���
	bool CylinderAndSphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget);

	// �~����AABB�̏Փ˔���
	bool CylinderAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTarget);

	// �~����AABB�̏Փ˖ʂ��擾
	int GetCylinderAndAABB(const D3DXVECTOR3& SelfOldPos, const D3DXVECTOR3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const D3DXVECTOR3& OtherPos, const D3DXVECTOR3& OtherSize);
};

#endif // _COLLISION_H_