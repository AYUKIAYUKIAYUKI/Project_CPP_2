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

		// �ڕW�������ړ������ɐݒ�
		SetRotTargetToMoveDirection();

		if (CManager::GetKeyboard()->GetTrigger(DIK_RSHIFT))
		{
			// �_�b�V��������
			To_Dash(false);
		}
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{ // �J�������猩�ĉE��
		
		 // ���p��ϓ�
		fDirection += fMoveSpeed;

		// �ڕW�������ړ������ɐݒ�
		SetRotTargetToMoveDirection();

		if (CManager::GetKeyboard()->GetTrigger(DIK_RSHIFT))
		{
			// �_�b�V��������
			To_Dash(true);
		}
	}

	// ���p�𔽉f
	m_pCharacter->SetDirection(fDirection);

	// �ڕW���W����p�ɍ��킹�Đݒ�
	SetPosTargetByDirection();
}

//============================================================================
// �ڕW�������ړ������ɐݒ�
//============================================================================
void CPlayer_State_Default::SetRotTargetToMoveDirection()
{
	/* �L�[���͂̔���O�ł��̊֐����ĂԂƁA���W�ړ��̏I��肪���̊����͂Ō��������� */

	Vec3 NewRotTarget = m_pCharacter->GetRotTarget();								// �ڕW�������擾
	const Vec3& MoveVec = m_pCharacter->GetPosTarget() - m_pCharacter->GetPos();	// �ړ������̃x�N�g�����쐬
	NewRotTarget.y = atan2f(-MoveVec.x, -MoveVec.z);								// �ڕW�������ړ�������
	m_pCharacter->SetRotTarget(NewRotTarget);										// �ڕW�����𔽉f
}

//============================================================================
// ���p����ڕW���W��ݒ�
//============================================================================
void CPlayer_State_Default::SetPosTargetByDirection()
{
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace field_manager;

	// ���p���擾
	const float& fDirection = m_pCharacter->GetDirection();

	Vec3 NewPosTarget = VEC3_INIT;										// �V�K�ڕW���W���쐬
	NewPosTarget.x = cosf(fDirection) * CField_Manager::FIELD_RADIUS;	// X�����̍��W��ݒ�
	NewPosTarget.z = sinf(fDirection) * CField_Manager::FIELD_RADIUS;	// Z�����̍��W��ݒ�
	m_pCharacter->SetPosTarget(NewPosTarget);							// �ڕW���W�𔽉f
}

//============================================================================
// �_�b�V��������
//============================================================================
void CPlayer_State_Default::To_Dash(bool bDirection)
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Dash(bDirection));
	}
}