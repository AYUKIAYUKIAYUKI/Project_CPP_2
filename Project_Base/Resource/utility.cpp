//============================================================================
// 
// �֗��֐� [utility.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "utility.h"

//============================================================================
// ���`�ǂ����̏Փ˔���
//============================================================================
bool CUtility::OnlySphere(const D3DXVECTOR3& posSelf, const float& fRadiusSelf, const D3DXVECTOR3& posTarget, const float& fRadiusTarget)
{
	// �ڕW�n�_�ւ̃x�N�g�����Z�o
	D3DXVECTOR3 vec = posTarget - posSelf;

	// ���݂��̋������A���݂��̔��a�̘a�ȉ��ł���ΏՓ�
	if (sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z) <= (fRadiusSelf + fRadiusTarget))
	{
		return 1;
	}

	return 0;
}

//============================================================================
// �~���Ɠ_�̏Փ˔���
//============================================================================
bool CUtility::CylinderAndPoint(const D3DXVECTOR3& posSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& posTarget)
{
	// �ڕW�n�_�ւ̃x�N�g�����Z�o
	D3DXVECTOR3 vec = posTarget - posSelf;

	// ���݂��̋������A���a�ȉ���
	//if (sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z) <= fRadiusSelf)
	if (sqrtf(vec.x * vec.x + vec.z * vec.z) <= fRadiusSelf)
	{
		// �~���̍����͈͓̔��ɓ_�����݂���ΏՓ�
		if (posSelf.y + fHeight > posTarget.y &&
			posSelf.y - fHeight < posTarget.y)
		{
			return 1;
		}
	}

	return 0;
}