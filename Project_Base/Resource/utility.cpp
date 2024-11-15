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
D3DXVECTOR3 utility::SetCubeVtxFromQuadrant(const WORD& wIdx)
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
// �_��Y���ŉ�]������
//============================================================================
D3DXVECTOR3 utility::RotatePointAroundY(const float& fDirection, const D3DXVECTOR3& Pos)
{
	// Y���̉�]�ʂ���sin��cos�̒l���v�Z
	const float& fSinY = sinf(fDirection), fCosY = cosf(fDirection);

	// ��]�s���p���āA���_���W��XZ���ʂŉ�]������
	const D3DXVECTOR3& ResultPos = {
		Pos.x * fCosY + Pos.z * fSinY,
		Pos.y,
		-Pos.x * fSinY + Pos.z * fCosY
	};

	return ResultPos;
}

//============================================================================
// �����̂�Y���ŉ�]������
//============================================================================
D3DXVECTOR3 utility::RotateBoxAroundY(const WORD& wIdx, const float& fDirection, const D3DXVECTOR3& Size)
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
// �����̂�Y���̉�]��ł�����
//============================================================================
D3DXVECTOR3 utility::InverseRotateBoxAroundY(const float& fDirection, const D3DXVECTOR3& VtxPos)
{
	// Y���̋t�̉�]�ʂ���sin��cos�̒l���v�Z
	const float& fSinY = sinf(-fDirection), fCosY = cosf(-fDirection);

	// ��]�s��̋t��p���āAXZ���ʂŉ�]�������_�̍��W�����ɖ߂�
	const D3DXVECTOR3& ResultPos = {
		VtxPos.x * fCosY + VtxPos.z * fSinY,
		VtxPos.y,
		-VtxPos.x * fSinY + VtxPos.z * fCosY
	};

	return ResultPos;
}

//============================================================================
// �p�x�̍��̕␳(���W�A��)
//============================================================================
void utility::AdjustAngle(float& fAngle, const float& fDest)
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
void utility::AdjustAngle(float& fAngle, const float& fDest, const float& fRadius)
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
void utility::AdjustDirection(float& fAngle)
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
void utility::AdjustDirection(float& fAngle1, float& fAngle2)
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
// JSON�t�@�C���̓W�J
//============================================================================
nlohmann::json utility::OpenJsonFile(std::string FilePath)
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
#if 0
		CRenderer::SetTimeString("JSON�t�@�C���y" + FilePath + "�z�̓W�J�Ɏ��s���܂���", 90);
#else
		assert(false && "JSON�t�@�C���̓W�J�Ɏ��s");
#endif
	}

	return nullptr;
}