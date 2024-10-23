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

// �ύX��X�e�[�g
#include "player_state_dash.h"

// �t�B�[���h�T�C�Y�擾�p
#include "field_manager.h"

// �C���v�b�g�擾�p
#include "manager.h"

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
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
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace field_manager;

	// �v���C���[�p�����[�^�p
	float fDirection = m_pPlayer->GetDirection();			// ���p���擾
	const float& fMoveSpeed = m_pPlayer->GetMoveSpeed();	// �ړ����x���擾

	// �C���v�b�g�n�擾
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad* pPad = CManager::GetPad();					// �p�b�h

	// X���̓���
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
	{
		fDirection += -fMoveSpeed;
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{
		fDirection += fMoveSpeed;
	}

	// ���p�𔽉f
	m_pPlayer->SetDirection(fDirection);

	// ���W�𔽉f
	Vec3 NewPos = VEC3_INIT;
	NewPos.x = cosf(fDirection) * CField_Manager::FIELD_RADIUS;
	NewPos.z = sinf(fDirection) * CField_Manager::FIELD_RADIUS;
	m_pPlayer->SetPos(NewPos);

	// �����𔽉f
	Vec3 NewRot = VEC3_INIT;
	//NewRot.y = atan2f(NewRot.x - NewPos.x, NewRot.z - NewPos.z);
	m_pPlayer->SetRot(NewRot);

	if (CManager::GetKeyboard()->GetTrigger(DIK_RSHIFT))
	{
		// �_�b�V��������
		To_Dash();
	}
}

//============================================================================
// �_�b�V��������
//============================================================================
void CPlayer_State_Default::To_Dash()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Dash());
	}
}