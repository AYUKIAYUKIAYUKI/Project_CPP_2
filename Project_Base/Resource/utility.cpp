//============================================================================
// 
// �֗��֐� [utility.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "renderer.h"

//============================================================================
// �p�x�̍��̕␳(���W�A��)
//============================================================================
void CUtility::AdjustAngle(float& fAngle, const float& fDest)
{
	// �ڕW�p�x�ƌ��݊p�x�Ƃ̍����v�Z
	const float& fDifference = fAngle - fDest;

	if (fDifference > D3DX_PI)
	{
		fAngle += -D3DX_PI * 2.0f;

#ifdef _DEBUG

		// �ǂꂾ���̍������邩�\��
		CRenderer::GetInstance()->SetTimeString("[��ꂫ1]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
	else if (fDifference < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	
#ifdef _DEBUG

		// �ǂꂾ���̍������邩�\��
		CRenderer::GetInstance()->SetTimeString("[��ꂫ2]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
}

//============================================================================
// �p�x�̍��̕␳(���W�A��)
//============================================================================
void CUtility::AdjustAngle(float& fAngle, const float& fDest, const float& fRadius)
{
	// �ڕW�p�x�ƌ��݊p�x�Ƃ̍����v�Z
	const float& fDifference = fAngle - fDest;

	if (fDifference > fRadius)
	{
		fAngle += -fRadius * 2.0f;

#ifdef _DEBUG

		// �ǂꂾ���̍������邩�\��
		CRenderer::GetInstance()->SetTimeString("[��ꂫ1]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
	else if (fDifference < -fRadius)
	{
		fAngle += fRadius * 2.0f;

#ifdef _DEBUG

		// �ǂꂾ���̍������邩�\��
		CRenderer::GetInstance()->SetTimeString("[��ꂫ2]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
}

//============================================================================
// �����͈̔͂̕␳(���W�A��)
//============================================================================
void CUtility::AdjustDirection(float& fAngle)
{
	if (fAngle > D3DX_PI)
	{
		fAngle += -D3DX_PI * 2.0f;
	}
	else if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
}

//============================================================================
// �����͈̔͂̕␳(���W�A��)
//============================================================================
void CUtility::AdjustDirection(float& fAngle1, float& fAngle2)
{
	if (fAngle1 > D3DX_PI)
	{
		fAngle1 += -D3DX_PI * 2.0f;
		fAngle2 += -D3DX_PI * 2.0f;
	}
	else if (fAngle1 < -D3DX_PI)
	{
		fAngle1 += D3DX_PI * 2.0f;
		fAngle2 += D3DX_PI * 2.0f;
	}
}

//============================================================================
// ���`�ǂ����̏Փ˔���
//============================================================================
bool CUtility::OnlySphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget)
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
// �~���Ɠ_�̏Փ˔���
//============================================================================
bool CUtility::CylinderAndPoint(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget)
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
bool CUtility::CylinderAndSphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget)
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
bool CUtility::CylinderAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& sizeTarget)
{
	// �S�Ă̎��������炨�݂��ɂ߂荞��ł���Ƃ��Փ�
	if (PosSelf.x + fRadiusSelf > PosTarget.x - sizeTarget.x &&
		PosSelf.x - fRadiusSelf < PosTarget.x + sizeTarget.x &&
		PosSelf.y + fHeight > PosTarget.y - sizeTarget.y &&
		PosSelf.y < PosTarget.y + sizeTarget.y &&
		PosSelf.z + fRadiusSelf > PosTarget.z - sizeTarget.z &&
		PosSelf.z - fRadiusSelf < PosTarget.z + sizeTarget.z)
	{
		return 1;
	}

	return 0;
}