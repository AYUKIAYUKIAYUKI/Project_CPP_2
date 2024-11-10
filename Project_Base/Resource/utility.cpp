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
// �ی��ɉ����āA�P�ʗ����̗p�̒��_�̍��W���쐬
//============================================================================
const D3DXVECTOR3& CUtility::SetCubeVtxFromQuadrant(const WORD& wIdx)
{
	D3DXVECTOR3 VtxPos = { 0.0f, 0.0f, 0.0f };

	// �n���ꂽ���_�̏ی��ɉ����āA������Ԃ�
	switch (wIdx)
	{
	case 0: VtxPos = { -1.0f, +1.0f, -1.0f }; break;
	case 1: VtxPos = { +1.0f, +1.0f, -1.0f }; break;
	case 2: VtxPos = { -1.0f, -1.0f, -1.0f }; break;
	case 3: VtxPos = { +1.0f, -1.0f, -1.0f }; break;
	case 4: VtxPos = { -1.0f, +1.0f, +1.0f }; break;
	case 5: VtxPos = { +1.0f, +1.0f, +1.0f }; break;
	case 6: VtxPos = { -1.0f, -1.0f, +1.0f }; break;
	case 7: VtxPos = { +1.0f, -1.0f, +1.0f }; break;
	default: CRenderer::SetTimeString("�ی��̔���ɑΉ����Ă��Ȃ��ԍ����n����܂����F" + std::to_string(wIdx), 120); break;
	}

	return VtxPos;
}

//============================================================================
// �����̂�Y���ŉ�]������
//============================================================================
const D3DXVECTOR3& CUtility::RotateRectAroundY(const WORD& wIdx, const float& fDirection, const D3DXVECTOR3& Size)
{
	// �ی��ɉ����āA���_�̍��W���쐬
	const D3DXVECTOR3& VtxPos = SetCubeVtxFromQuadrant(wIdx);

	// Y���̉�]�ʂ���sin��cos�̒l���v�Z
	const float& fSinY = sinf(fDirection), fCosY = cosf(fDirection);

	// ��]�s���p���āA���_���W��XZ���ʂŉ�]������
	const D3DXVECTOR3& ResultPos = {
		VtxPos.x * fCosY * Size.x + VtxPos.z * fSinY * Size.z,
		VtxPos.y * Size.y,
		-VtxPos.x * fSinY * Size.x + VtxPos.z * fCosY * Size.z
	};

	return ResultPos;
}

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
		CRenderer::SetTimeString("[��ꂫ1]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
	else if (fDifference < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	
#ifdef _DEBUG

		// �ǂꂾ���̍������邩�\��
		CRenderer::SetTimeString("[��ꂫ2]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);

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
		CRenderer::SetTimeString("[��ꂫ1]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
	else if (fDifference < -fRadius)
	{
		fAngle += fRadius * 2.0f;

#ifdef _DEBUG

		// �ǂꂾ���̍������邩�\��
		CRenderer::SetTimeString("[��ꂫ2]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);

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

//============================================================================
// JSON�t�@�C���̓W�J
//============================================================================
nlohmann::json CUtility::OpenJsonFile(std::string FilePath)
{
	// JSON�t�@�C����ǂݎ��W�J
	std::ifstream ifs(FilePath);

	// �t�@�C�����W�J�o���Ă�����
	if (ifs.good())
	{
		// JSON�f�[�^���p�[�X
		nlohmann::json Json;
		ifs >> Json;

		// JSON�I�u�W�F�N�g�������n��
		return Json;
	}
	else
	{
		CRenderer::SetTimeString("JSON�t�@�C���y" + FilePath + "�z�̓W�J�Ɏ��s���܂���", 90);
	}

	return nullptr;
}