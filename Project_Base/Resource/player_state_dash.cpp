//============================================================================
//
// �v���C���[�_�b�V���X�e�[�g [player_state_dash.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state_dash.h"

// �ύX��X�e�[�g
#include "player_state_default.h"

// �t�B�[���h�T�C�Y�擾�p
#include "field_manager.h"

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
CPlayer_State_Dash::CPlayer_State_Dash() :
	CPlayer_State{},
	m_nDashDuration{ 0 }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer_State_Dash::~CPlayer_State_Dash()
{

}

//============================================================================
// �X�V����
//============================================================================
void CPlayer_State_Dash::Update()
{
	// �ڕW�������ړ������ɐݒ�
	SetRotTargetToMoveDirection();

	// �ڕW���W���_�b�V�������ɑ���
	SetPosTarget_Unnamed();

	// �p�����Ԃ��J�E���g�A�b�v
	m_nDashDuration++;

	// �p�����Ԃ��ő�ɓ��B��
	if (m_nDashDuration >= MAX_DASH_DURATION)
	{
		// ���ɖ߂�
		To_Default();
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �ڕW�������ړ������ɐݒ�
//============================================================================
void CPlayer_State_Dash::SetRotTargetToMoveDirection()
{
	Vec3 NewRotTarget = m_pPlayer->GetRotTarget();							// �ڕW�������擾
	const Vec3& MoveVec = m_pPlayer->GetPosTarget() - m_pPlayer->GetPos();	// �ړ������̃x�N�g�����쐬
	NewRotTarget.y = atan2f(-MoveVec.x, -MoveVec.z);						// �ڕW�������ړ�������
	m_pPlayer->SetRotTarget(NewRotTarget);									// �ڕW�����𔽉f
}

//============================================================================
// �ڕW���W���_�b�V�������ɑ���
//============================================================================
void CPlayer_State_Dash::SetPosTarget_Unnamed()
{
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace field_manager;

	// ���p��ݒ�
	float fDirection = m_pPlayer->GetDirection();		// �擾
	fDirection += -m_pPlayer->GetMoveSpeed() * 20.0f;	// �ϓ�
	m_pPlayer->SetDirection(fDirection);				// ���f

	Vec3 NewPosTarget = VEC3_INIT;										// �V�K�ڕW���W���쐬
	NewPosTarget.x = cosf(fDirection) * CField_Manager::FIELD_RADIUS;	// X�����̍��W��ݒ�
	NewPosTarget.z = sinf(fDirection) * CField_Manager::FIELD_RADIUS;	// Z�����̍��W��ݒ�
	m_pPlayer->SetPosTarget(NewPosTarget);								// �ڕW���W�𔽉f
}

//============================================================================
// ���ɖ߂�
//============================================================================
void CPlayer_State_Dash::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}