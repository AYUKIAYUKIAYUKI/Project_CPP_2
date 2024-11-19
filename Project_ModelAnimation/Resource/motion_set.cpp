//============================================================================
// 
// ���[�V�����}�l�[�W���[ [motion_manager.cpp]
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
	m_wMaxMotion{ 0 },
	m_apMotion{ nullptr }
{
	m_vpModelParts.clear();
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CMotion_Set::~CMotion_Set()
{

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
	if (m_wNowFrame >= GetNowKey()->wMaxFrame)
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
	const WORD wFrameCoef = m_apMotion[m_wNowMotion].apKey[m_wNowKey].wMaxFrame - m_wNowFrame;

	// �p�[�c�����擾
	const WORD wSize = static_cast<WORD>(m_vpModelParts.size());

	// �S�Ẵp�[�c�����ꂼ��̖ڕW�l�֕␳�����p�����[�^��ݒ肷��
	for (WORD wCntModelParts = 0; wCntModelParts < wSize; ++wCntModelParts)
	{
		// �ڕW�k��
		Vec3 NewScale = m_vpModelParts[wCntModelParts]->GetScale();
		NewScale += (m_apMotion->apKey[m_wNowKey].apDest[wCntModelParts].ScaleTarget - NewScale) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetScale(NewScale);

		// �ڕW����
		Vec3 NewRot = m_vpModelParts[wCntModelParts]->GetRot();
		NewRot += (m_apMotion->apKey[m_wNowKey].apDest[wCntModelParts].RotTarget - NewRot) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetRot(NewRot);

		// �ڕW���W
		Vec3 NewPos = m_vpModelParts[wCntModelParts]->GetPos();
		NewPos += (m_apMotion->apKey[m_wNowKey].apDest[wCntModelParts].PosTarget - NewPos) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetPos(NewPos);
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���݂̃��[�V�����̃|�C���^���擾
//============================================================================
CMotion_Set::Motion* const CMotion_Set::GetNowMotion() const
{
	return &m_apMotion[m_wNowMotion];
}

//============================================================================
// ���݂̃L�[�̃|�C���^���擾
//============================================================================
CMotion_Set::Key* const CMotion_Set::GetNowKey() const
{
	return &GetNowMotion()->apKey[m_wNowKey];
}
