//============================================================================
//
// �v���C���[�_���[�W�X�e�[�g [player_state_damage.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state_damage.h"
#include "player_state_default.h"
#include "manager.h"

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
CPlayer_State_Damage::CPlayer_State_Damage() :
	CPlayer_State{},
	m_nDamageDuration{ 0 }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer_State_Damage::~CPlayer_State_Damage()
{

}

//============================================================================
// �X�V����
//============================================================================
void CPlayer_State_Damage::Update()
{
	// �p�����Ԃ��J�E���g�A�b�v
	m_nDamageDuration++;

	// �p�����Ԃ̔����܂ōs���ƕ�����
	if (m_nDamageDuration >= MAX_DASH_DURATION * 0.5f)
	{
		// �C���v�b�g�n�擾
		CInputKeyboard* pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
		CInputMouse* pMouse = CManager::GetMouse();				// �}�E�X
		CInputPad* pPad = CManager::GetPad();					// �p�b�h

		// �v���C���[�̃p�����[�^���擾
		float fDirectionTarget = m_pCharacter->GetDirectionTarget();	// �ڕW���p���擾
		const float& fMoveSpeed = m_pCharacter->GetMoveSpeed();			// �ړ����x���擾

		// X���̓���
		if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
		{ // �J�������猩�č���

			// ���p��ϓ�
			fDirectionTarget += -fMoveSpeed;

			// �����ŖڕW���W��ϓ��������p�ɍ��킹��
			m_pCharacter->AutoSetRotTarget();
		}
		else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
		{ // �J�������猩�ĉE��

			 // ���p��ϓ�
			fDirectionTarget += fMoveSpeed;

			// �����ŖڕW���W��ϓ��������p�ɍ��킹��
			m_pCharacter->AutoSetRotTarget();
		}

		// �ڕW���p�𔽉f
		m_pCharacter->SetDirectionTarget(fDirectionTarget);
	}

	// �_��
	if (m_nDamageDuration % 2 == 0)
	{
		//m_pCharacter->SetUseCol(true);
		//m_pCharacter->SetCol({ 1.0f, 0.0f, 0.0f, 0.25f });
	}
	else
	{
		//m_pCharacter->SetUseCol(false);
	}

	// �p�����Ԃ��ő�ɓ��B��
	if (m_nDamageDuration >= MAX_DASH_DURATION)
	{
		// �ʏ�J���[�ɐݒ�
		//m_pCharacter->SetUseCol(false);

		// �ʏ��Ԃ�
		To_Default();
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �ʏ��Ԃ�
//============================================================================
void CPlayer_State_Damage::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}