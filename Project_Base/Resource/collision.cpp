//============================================================================
// 
// �R���W�����n�֐� [collision.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "collision.h"

//============================================================================
// ���ǂ����̏Փ˔���
//============================================================================
bool collision::OnlySphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget)
{
	// �Ώۈʒu�ւ̃x�N�g�����Z�o
	const D3DXVECTOR3& Vec = PosTarget - PosSelf;

	// ���݂��̋������A���݂��̔��a�̘a�ȉ��ł���ΏՓ�
	if (sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= fRadiusSelf - fRadiusTarget)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// ����AABB�̏Փ˔���
//============================================================================
bool collision::SphereAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTarget)
{
	// �S�Ă̎��������炨�݂��ɂ߂荞��ł���Ƃ��Փ�
	if (PosSelf.x + fRadiusSelf > PosTarget.x - SizeTarget.x &&
		PosSelf.x - fRadiusSelf < PosTarget.x + SizeTarget.x &&
		PosSelf.y + fRadiusSelf > PosTarget.y - SizeTarget.y &&
		PosSelf.y - fRadiusSelf < PosTarget.y + SizeTarget.y &&
		PosSelf.z + fRadiusSelf > PosTarget.z - SizeTarget.z &&
		PosSelf.z - fRadiusSelf < PosTarget.z + SizeTarget.z)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// �~���Ɠ_�̏Փ˔���
//============================================================================
bool collision::CylinderAndPoint(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget)
{
	// �Ώۈʒu�ւ̃x�N�g�����Z�o
	const D3DXVECTOR3& Vec = PosTarget - PosSelf;

	// ���݂��̋������A���a�ȉ���
	if (sqrtf(Vec.x * Vec.x + Vec.z * Vec.z) <= fRadiusSelf)
	{
		// �~���̍����͈͓̔��ɓ_�����݂���ΏՓ�
		if (PosSelf.y + fHeight > PosTarget.y &&
			PosSelf.y - fHeight < PosTarget.y)
		{
			return 1;
		}
	}

	return 0;
}

//============================================================================
// �~���Ƌ��̏Փ˔���
//============================================================================
bool collision::CylinderAndSphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget)
{
	// �Ώۈʒu�ւ̃x�N�g�����Z�o
	const D3DXVECTOR3& Vec = PosTarget - PosSelf;

	// ���݂��̋������A���a�ȉ���
	if (sqrtf(Vec.x * Vec.x + Vec.z * Vec.z) <= fRadiusSelf - fRadiusTarget)
	{
		// �~���̍����͈͓̔��ɋ������݂���ΏՓ�
		if (PosSelf.y + fHeight > PosTarget.y &&
			PosSelf.y - fHeight < PosTarget.y)
		{
			return 1;
		}
	}

	return 0;
}

//============================================================================
// �~����AABB�̏Փ˔���
//============================================================================
bool collision::CylinderAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTarget)
{
	// �S�Ă̎��������炨�݂��ɂ߂荞��ł���Ƃ��Փ�
	if (PosSelf.x + fRadiusSelf > PosTarget.x - SizeTarget.x &&
		PosSelf.x - fRadiusSelf < PosTarget.x + SizeTarget.x &&
		PosSelf.y + fHeight > PosTarget.y - SizeTarget.y &&
		PosSelf.y < PosTarget.y + SizeTarget.y &&
		PosSelf.z + fRadiusSelf > PosTarget.z - SizeTarget.z &&
		PosSelf.z - fRadiusSelf < PosTarget.z + SizeTarget.z)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// �~����AABB�̏Փ˖ʂ��擾
//============================================================================
int collision::GetCylinderAndAABB(const D3DXVECTOR3& SelfOldPos, const D3DXVECTOR3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const D3DXVECTOR3& OtherPos, const D3DXVECTOR3& OtherSize)
{
	// �Փ˂��������
	if (!CylinderAndAABB(SelfNowPos, fSelfRadius, fSelfHeight, OtherPos, OtherSize))
	{
		return 0;
	}

	// ���Ƃ���Y���̎ˉe�����ɏd�Ȃ��Ă����� (�v�Z�݌v��O��ɏՓ˂͂��Ȃ�)
	if (SelfOldPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
		SelfOldPos.x - fSelfRadius < OtherPos.x + OtherSize.x)
	{ // �d�Ȃ��Ă��� -> �㉺�̔���

		// ��ɂ���
		if (SelfOldPos.y > OtherPos.y)
		{
			return 1;
		}

		// ���ɂ���
		if (SelfOldPos.y < OtherPos.y)
		{
			return 2;
		}
	}
	else
	{ // �d�Ȃ��Ă��Ȃ����� -> ���E�̔���

		// ���ɂ���
		if (SelfOldPos.x < OtherPos.x)
		{
			return 3;
		}

		// �E�ɂ���
		if (SelfOldPos.x > OtherPos.x)
		{
			return 4;
		}
	}

	return 0;
}