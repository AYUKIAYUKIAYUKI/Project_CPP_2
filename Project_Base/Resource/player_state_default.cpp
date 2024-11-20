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
#include "player_state_dash.h"
#include "player_state_jump.h"
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
	CInputPad* pPad = CManager::GetPad();					// �p�b�h

	// �v���C���[�̃p�����[�^���擾
	float fDirection = m_pCharacter->GetDirection();		// ���p���擾
	const float& fMoveSpeed = m_pCharacter->GetMoveSpeed();	// �ړ����x���擾

	// X���̓���
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
	{ // �J�������猩�č���

		// ���p��ϓ�
		fDirection += -fMoveSpeed;

	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{ // �J�������猩�ĉE��
		
		 // ���p��ϓ�
		fDirection += fMoveSpeed;
	}

	if (pKeyboard->GetTrigger(DIK_RSHIFT))
	{
		// �_�b�V����Ԃ�
		To_Dash();
	}
	else if (pKeyboard->GetTrigger(DIK_SPACE))
	{
		// �W�����v��Ԃ�
		To_Jump();
	}

	// ���p�𔽉f
	m_pCharacter->SetDirection(fDirection);
}

//============================================================================
// �_�b�V����Ԃ�
//============================================================================
void CPlayer_State_Default::To_Dash()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Dash());
	}
}

//============================================================================
// �W�����v��Ԃ�
//============================================================================
void CPlayer_State_Default::To_Jump()
{
	if (GetNextState() == nullptr)
	{
		// Y���̉����x��啝�ɑ���
		m_pCharacter->SetVelY(CPlayer_State_Jump::AMOUNT_JUMPACCEL);

		SetNextState(DBG_NEW CPlayer_State_Jump());
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