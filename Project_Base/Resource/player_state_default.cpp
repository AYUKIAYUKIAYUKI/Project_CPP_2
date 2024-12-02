//============================================================================
//
// �v���C���[�ʏ�X�e�[�g [player_state_default.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state_default.h"
#include "character.h"
#include "player_state_dash.h"
#include "player_state_jump.h"
#include "player_state_slash.h"
#include "player_state_damage.h"

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
// �R���X�g���N�^
//============================================================================
CPlayer_State_Default::CPlayer_State_Default() :
	CPlayer_State{}
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer_State_Default::~CPlayer_State_Default()
{

}

//============================================================================
// �X�V����
//============================================================================
void CPlayer_State_Default::Update()
{
	// �v���C���[�X�e�[�g�N���X�̍X�V����
	CPlayer_State::Update();

	// ����
	Control();

	// �d�͉���
	m_pCharacter->SetVelY(m_pCharacter->GetVelY() + CField_Manager::FIELD_GRAVITY);
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
void CPlayer_State_Default::Control()
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

		// �ړ����͎��ɂ̂�
		if (pMouse->GetTrigger(1))
		{
			// �_�b�V����Ԃ�
			To_Dash();
		}
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{ // �J�������猩�ĉE��
		
		 // ���p��ϓ�
		fDirectionTarget += fMoveSpeed;

		// �����ŖڕW���W��ϓ��������p�ɍ��킹��
		m_pCharacter->AutoSetRotTarget();

		// �ړ����͎��ɂ̂�
		if (pMouse->GetTrigger(1))
		{
			// �_�b�V����Ԃ�
			To_Dash();
		}
	}

	if (pMouse->GetTrigger(0))
	{
		// �W�����v��Ԃ�
		To_Jump();
	}
	else if (pKeyboard->GetTrigger(DIK_SPACE))
	{
		// �a����Ԃ�
		To_Slash();
	}

	// �ڕW���p�𔽉f
	m_pCharacter->SetDirectionTarget(fDirectionTarget);
}

//============================================================================
// �_�b�V����Ԃ�
//============================================================================
void CPlayer_State_Default::To_Dash()
{
	// �_�b�V���̃L���X�g�����܂��Ă���Ώ������s��Ȃ�
	if (m_nCntDashCast > 0)
		return;

	if (GetNextState() == nullptr)
	{
		// �ă_�b�V���܂ł̃L���X�g�J�E���g��ݒ�
		m_nCntDashCast = CPlayer_State_Dash::MAX_DASH_CAST;

		SetNextState(DBG_NEW CPlayer_State_Dash());
	}
}

//============================================================================
// �W�����v��Ԃ�
//============================================================================
void CPlayer_State_Default::To_Jump()
{
	// Y�������ɉ����x�̂����������(��)�Ȃ�W�����v�ɕύX���Ȃ�
	if (m_pCharacter->GetVelY() != 0.0f)
		return;

	if (GetNextState() == nullptr)
	{
		// Y���̉����x��啝�ɑ���
		m_pCharacter->SetVelY(CPlayer_State_Jump::AMOUNT_JUMPACCEL);

		SetNextState(DBG_NEW CPlayer_State_Jump());
	}
}

//============================================================================
// �a����Ԃ�
//============================================================================
void CPlayer_State_Default::To_Slash()
{
	// �a���̃L���X�g�����܂��Ă���Ώ������s��Ȃ�
	if (m_nCntSlashCast > 0)
		return;

	if (GetNextState() == nullptr)
	{
		// �čU���܂ł̃L���X�g�J�E���g��ݒ�
		m_nCntSlashCast = CPlayer_State_Slash::MAX_SLASH_CAST;

		SetNextState(DBG_NEW CPlayer_State_Slash());
	}
}

//============================================================================
// �_���[�W��Ԃ�
//============================================================================
void CPlayer_State_Default::To_Damage()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Damage());
	}
}