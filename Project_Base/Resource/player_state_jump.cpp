//============================================================================
//
// �v���C���[�W�����v�X�e�[�g [player_state_jump.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state_jump.h"

// �ύX��X�e�[�g
#include "player_state_default.h"

// �t�B�[���h�擾�p
#include "field_manager.h"

// �C���v�b�g�擾�p
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
// �f�t�H���g�R���X�g���N�^
//============================================================================
CPlayer_State_Jump::CPlayer_State_Jump() :
	CPlayer_State{},
	m_nJumpRemainDuration{ 0 },
	m_bEndRemain{ false }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer_State_Jump::~CPlayer_State_Jump()
{

}

//============================================================================
// �X�V����
//============================================================================
void CPlayer_State_Jump::Update()
{
	// �ԈႢ�Ȃ��v���C���[�����n���Ă���ꍇ
	if (m_pCharacter->GetAccelY() <= 0.0f && m_pCharacter->GetPos().y <= 1.0f)	// �ꎞ�I��
	{
		// �ʏ��Ԃ�
		To_Default();
	}

	// ����
	Control();

	// �d�͂̕␳
	AdjustGravity();
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
void CPlayer_State_Jump::Control()
{
	// �C���v�b�g�n�擾
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad* pPad = CManager::GetPad();					// �p�b�h

	// �v���C���[�̃p�����[�^���擾
	float fDirection = m_pCharacter->GetDirection();		// ���p���擾
	const float& fMoveSpeed = m_pCharacter->GetMoveSpeed();	// �ړ����x���擾

	// X���̓���
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
	{ // �J�������猩�č���

		// ���p��ϓ�
		fDirection += -fMoveSpeed * 0.5f;
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{ // �J�������猩�ĉE��

		 // ���p��ϓ�
		fDirection += fMoveSpeed * 0.5f;
	}

	// ���p�𔽉f
	m_pCharacter->SetDirection(fDirection);
}

//============================================================================
// �d�͂̕␳
//============================================================================
void CPlayer_State_Jump::AdjustGravity()
{
	// ���͉�������
	m_nJumpRemainDuration++;

	// �W�����v�̓��͂���߂邩�A���͂̌p�����Ԃ��ő剄���P�\�ɒB����Ɖ������I������
	if (CManager::GetKeyboard()->GetRelease(DIK_SPACE) || m_nJumpRemainDuration > MAX_JUMPREMAIN_DURATION)
	{
		m_bEndRemain = true;
	}

	// �d�͉���
	if (!m_bEndRemain)
	{
		// �W�����v�������͂킸���ɏd�͂ɋt�炤
		m_pCharacter->SetAccelY(m_pCharacter->GetAccelY() + -CField_Manager::FIELD_GRAVITY * 0.25f);
	}
	else
	{
		// �������Ԃ��I������ƒʏ�̏d�͉������s��
		m_pCharacter->SetAccelY(m_pCharacter->GetAccelY() + CField_Manager::FIELD_GRAVITY);
	}
}

//============================================================================
// �X�e�[�g - �ʏ��Ԃ�
//============================================================================
void CPlayer_State_Jump::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}