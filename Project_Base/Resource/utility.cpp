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

#if 0	// �ǂꂾ���̍������邩�\��
#ifdef _DEBUG
		CRenderer::SetTimeString("[��ꂫ1]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);
#endif	// _DEBUG
#endif
	}
	else if (fDifference < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	
#if 0	// �ǂꂾ���̍������邩�\��
#ifdef _DEBUG
		CRenderer::SetTimeString("[��ꂫ2]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);
#endif	// _DEBUG
#endif
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

#if 0	// �ǂꂾ���̍������邩�\��
#ifdef _DEBUG
		CRenderer::SetTimeString("[��ꂫ1]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);
#endif	// _DEBUG
#endif
	}
	else if (fDifference < -fRadius)
	{
		fAngle += fRadius * 2.0f;

#if 0	// �ǂꂾ���̍������邩�\��
#ifdef _DEBUG
		CRenderer::SetTimeString("[��ꂫ2]���݊p�x�ƖڕW�p�x�̍� : " + std::to_string(fDifference), 120);
#endif	// _DEBUG
#endif
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

//============================================================================
// Json�f�[�^��D3DXVECTOR2�ɕϊ�
//============================================================================
D3DXVECTOR2 utility::JsonConvertToVec2(nlohmann::json Json)
{
	return D3DXVECTOR2(static_cast<float>(Json[0]), static_cast<float>(Json[1]));
}

//============================================================================
// Json�f�[�^��D3DXVECTOR3�ɕϊ�
//============================================================================
D3DXVECTOR3 utility::JsonConvertToVec3(nlohmann::json Json)
{
	return D3DXVECTOR3(static_cast<float>(Json[0]), static_cast<float>(Json[1]), static_cast<float>(Json[2]));
}

//============================================================================
// UTF8��Json�f�[�^��SJIS�ɕϊ�
//============================================================================
std::string utility::JsonConvertToSJIS(nlohmann::json Json)
{
	// ������̒������i�[
	int nLength = 0;

	// UTF8�̕������string�^�ɃR�s�[
	const std::string& Str = Json;

	// UTF8�̕�����̒������擾
	nLength = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);

	// �G���[����
	if (nLength == 0) 
	{
		// �G���[�R�[�h���擾
		DWORD error = GetLastError();

		// ��O���X���[
		throw std::runtime_error("Failed");
	}

	// UTF16�ϊ���̕�������i�[
	std::wstring WideStr(nLength, L'\0');

	// UTF8��UTF16�ɕϊ�
	MultiByteToWideChar(
		CP_UTF8,		// �ϊ��R�[�h�y�[�W
		0,				// �ϊ��̎��
		Str.c_str(),	// �ϊ��������������
		-1,				// ������̒��� (-1�őS�̎w��)
		&WideStr[0],	// �ϊ���̕�������󂯎��|�C���^
		nLength);		// ������̃o�b�t�@�T�C�Y

	// UTF16�̕�����̒������擾
	nLength = WideCharToMultiByte(CP_ACP, 0, WideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	// �G���[����
	if (nLength == 0)
	{
		// �G���[�R�[�h���擾
		DWORD error = GetLastError();

		// ��O���X���[
		throw std::runtime_error("Failed");
	}

	// SJIS�ϊ���̕�������i�[
	std::string SjisStr(nLength, '\0');

	// UTF16��SJIS�ɕϊ�
	WideCharToMultiByte(
		CP_ACP,				// �ϊ��R�[�h�y�[�W
		0,					// �ϊ��̎��
		WideStr.c_str(),	// �ϊ����������C�h������
		-1,					// ������̒��� (-1�őS�̎w��)
		&SjisStr[0],		// �ϊ���̕�������󂯎��|�C���^
		nLength,			// ������̃o�b�t�@�T�C�Y
		nullptr,			// �ϊ��ł��Ȃ������̑���ɏo������
		nullptr);			// ����̕����̎g�p������󂯎��|�C���^

	return SjisStr;
}