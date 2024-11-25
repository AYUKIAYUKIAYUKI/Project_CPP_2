//============================================================================
// 
// ���[�V�����Z�b�g [motion_set.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "motion_set.h"
#include "object_Parts.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CMotion_Set::CMotion_Set() :
	m_wNowMotion{ 0 },
	m_wNowKey{ 0 },
	m_wNowFrame{ 0 },
	m_vpModelParts{},
	m_wMaxParts{ 0 },
	m_wMaxMotion{ 0 },
	m_vpMotion{}
{
	m_vpModelParts.clear();
	m_vpMotion.clear();
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CMotion_Set::~CMotion_Set()
{

}

//============================================================================
// ���
//============================================================================
void CMotion_Set::Release()
{
	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		CMotion_Set::Motion* const pMotion = &m_vpMotion[wCntMotion];

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// �L�[���̃|�C���^���쐬
			CMotion_Set::Key* const pKey = &pMotion->vpKey[wCntMotionKey];

			// �ڕW�l���̃|�C���^�z���j��
			if (pKey->apDest != nullptr)
			{
				delete[] pKey->apDest;
				pKey->apDest = nullptr;
			}
		}
	}

	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������ƂɃL�[���x�N�^�[�̃N���A
		m_vpMotion[wCntMotion].vpKey.clear();
	}

	// ���[�V�������̃x�N�^�[�̃N���A
	m_vpMotion.clear();

	// �p�[�c�p�I�u�W�F�N�g�̏���
	for (auto it : m_vpModelParts)
	{
		// �j���\��
		it->SetRelease();
	}

	// ���f���p�[�c�x�N�^�[�̃N���A
	m_vpModelParts.clear();

	// �Ō�ɂ��̃��[�V�����Z�b�g��j��
	delete this;
}

//============================================================================
// �X�V����
//============================================================================
void CMotion_Set::Update()
{
	// ����
	Animation();
}

//============================================================================
// ����
//============================================================================
void CMotion_Set::Animation()
{
	// �t���[���J�E���g
	CountFrame();

	// �ڕW�l�ւ̕␳
	CorrectTarget();
}

//============================================================================
// �t���[���J�E���g
//============================================================================
void CMotion_Set::CountFrame()
{
	// ���݂̃t���[�������C���N�������g
	m_wNowFrame++;

	// �t���[�������A���ݍĐ����̃L�[�̑��t���[�����ɒB������
	if (m_wNowFrame >= GetNowKey()->nMaxFrame)
	{
		// ���݂̃t���[���������Z�b�g
		m_wNowFrame = 0;

		// ���݂̃L�[�����C���N�������g
		m_wNowKey++;

		// �L�[�����A���ݍĐ����̃��[�V�����̑��L�[���ɒB������
		if (m_wNowKey >= GetNowMotion()->wMaxKey)
		{
			// ���݂̃L�[�������Z�b�g
			m_wNowKey = 0;
		}
	}
}

//============================================================================
// �ڕW�l�ւ̕␳
//============================================================================
void CMotion_Set::CorrectTarget()
{
	// �t���[���̐i�s�x�����쐬 (���t���[���� - ���݂̃t���[��)
	const WORD wFrameCoef = m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].nMaxFrame - m_wNowFrame;

	// �S�Ẵp�[�c�����ꂼ��̖ڕW�l�֕␳�����p�����[�^��ݒ肷��
	for (WORD wCntModelParts = 0; wCntModelParts < m_wMaxParts; ++wCntModelParts)
	{
		// �ڕW�k��
		Vec3 NewScale = m_vpModelParts[wCntModelParts]->GetScale();
		NewScale += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].ScaleTarget - NewScale) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetScale(NewScale);

		// �ڕW����
		Vec3 NewRot = m_vpModelParts[wCntModelParts]->GetRot();
		NewRot += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].RotTarget - NewRot) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetRot(NewRot);

		// �ڕW���W
		Vec3 NewPos = m_vpModelParts[wCntModelParts]->GetPos();
		NewPos += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].PosTarget - NewPos) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetPos(NewPos);
	}
}

//============================================================================
// ����
//============================================================================
CMotion_Set* CMotion_Set::Create(JSON Json)
{
	// ���[�V�����Z�b�g�C���X�^���X�𐶐�
	CMotion_Set* pNew = DBG_NEW CMotion_Set();

	// �������s
	if (!pNew)
	{
		return nullptr;
	}

	// ���p�[�c�����擾
	pNew->m_wMaxParts = static_cast<WORD>(Json["MaxParts"]);

	// �p�[�c�����̃p�[�c�I�u�W�F�N�g���s���Đ���
	for (WORD wCntParts = 0; wCntParts < pNew->m_wMaxParts; ++wCntParts)
	{
		pNew->m_vpModelParts.push_back(CObject_Parts::Create(static_cast<CX_Manager::TYPE>(Json["ModelType"][wCntParts]), nullptr));
	}

	// �������ꂽ�p�[�c�ɑ΂��A�e��ݒ���s��
	for (WORD wCntParts = 0; wCntParts < pNew->m_wMaxParts; ++wCntParts)
	{
		// �e�p�[�c�̃C���f�b�N�X�ԍ����擾
		const SHORT& shParentIdx = static_cast<SHORT>(Json["ParentIdx"][wCntParts]);

		// �p�[�c�̃|�C���^���R�s�[
		CObject_Parts* pParts = pNew->m_vpModelParts[wCntParts];

		if (shParentIdx == -1)
		{
			// �C���f�b�N�X��-1�̂��̂͐e�������Ȃ�
			pParts->SetParent(nullptr);
		}
		else
		{
			// �e�p�[�c�̃|�C���^��n��
			pParts->SetParent(pNew->m_vpModelParts[shParentIdx]);
		}

		// �I�t�Z�b�g�l��ݒ�
		//pParts->SetScaleOffset(utility::JsonConvertToVec3(Json["ScaleOffset"][wCntParts]));
		//pParts->SetRotOffset(utility::JsonConvertToVec3(Json["RotOffset"][wCntParts]));
		pParts->SetPosOffset(utility::JsonConvertToVec3(Json["PosOffset"][wCntParts]));
	}

	// �����[�V���������擾
	pNew->m_wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// ���[�V���������̃��[�V�������𐶐�
	pNew->m_vpMotion.resize(pNew->m_wMaxMotion);

	// ���[�V�������̐ݒ�
	for (WORD wCntMotion = 0; wCntMotion < pNew->m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		Motion* const pMotion = &pNew->m_vpMotion[wCntMotion];

		// ���[�v�t���O���擾
		pMotion->bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// ���[�V�����̑��L�[�����擾
		pMotion->wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// �L�[���̐ݒ�
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// �L�[���I�u�W�F�N�g���쐬
			Key Key;

			// �L�[�̑��t���[�������擾
			Key.nMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotion][wCntMotionKey]);

			// �p�[�c�����̖ڕW�l���𐶐�
			Key.apDest = DBG_NEW CMotion_Set::KeyDest[pNew->m_wMaxParts];

			for (WORD wCntModelParts = 0; wCntModelParts < pNew->m_wMaxParts; ++wCntModelParts)
			{
				// �ڕW�l���̃|�C���^���쐬
				KeyDest* const pDest = &Key.apDest[wCntModelParts];

				// �e��p�����[�^��ݒ�
				pDest->ScaleTarget = utility::JsonConvertToVec3(Json["ScaleTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);	// �ڕW�k��
				pDest->RotTarget = utility::JsonConvertToVec3(Json["RotTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// �ڕW����
				pDest->PosTarget = utility::JsonConvertToVec3(Json["PosTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// �ڕW���W
			}

			pMotion->vpKey.push_back(Key);
		}
	}

	return pNew;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���݂̃��[�V�����̃|�C���^���擾
//============================================================================
CMotion_Set::Motion* const CMotion_Set::GetNowMotion()
{
	return &m_vpMotion[m_wNowMotion];
}

//============================================================================
// ���݂̃L�[�̃|�C���^���擾
//============================================================================
CMotion_Set::Key* const CMotion_Set::GetNowKey()
{
	return &GetNowMotion()->vpKey[m_wNowKey];
}
