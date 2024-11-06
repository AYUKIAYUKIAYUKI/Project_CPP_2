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
	m_bDirection{ false },
	m_nDashDuration{ 0 }
{

}

//============================================================================
// �����w��R���X�g���N�^
//============================================================================
CPlayer_State_Dash::CPlayer_State_Dash(bool bDirection) :
	CPlayer_State{},
	m_bDirection{ bDirection },
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
// �ڕW�������ړ������ɐݒ�
//============================================================================
void CPlayer_State_Dash::SetRotTargetToMoveDirection()
{
	Vec3 NewRotTarget = m_pCharacter->GetRotTarget();								// �ڕW�������擾
	const Vec3& MoveVec = m_pCharacter->GetPosTarget() - m_pCharacter->GetPos();	// �ړ������̃x�N�g�����쐬
	NewRotTarget.y = atan2f(-MoveVec.x, -MoveVec.z);								// �ڕW�������ړ�������
	m_pCharacter->SetRotTarget(NewRotTarget);										// �ڕW�����𔽉f
}

//============================================================================
// �ڕW���W���_�b�V�������ɑ���
//============================================================================
void CPlayer_State_Dash::SetPosTarget_Unnamed()
{
	// ���p��ݒ�
	float fDirection = m_pCharacter->GetDirection();

	// �ݒ肳��Ă�����p�ɍ��킹�Ĉړ�
	if (m_bDirection)
	{
		fDirection += m_pCharacter->GetMoveSpeed() * 5.0f;
	}
	else
	{
		fDirection += m_pCharacter->GetMoveSpeed() * -5.0f;
	}

	m_pCharacter->SetDirection(fDirection);

	Vec3 NewPosTarget = VEC3_INIT;										// �V�K�ڕW���W���쐬
	NewPosTarget.x = cosf(fDirection) * CField_Manager::FIELD_RADIUS;	// X�����̍��W��ݒ�
	NewPosTarget.z = sinf(fDirection) * CField_Manager::FIELD_RADIUS;	// Z�����̍��W��ݒ�
	m_pCharacter->SetPosTarget(NewPosTarget);							// �ڕW���W�𔽉f
}

//============================================================================
// �X�e�[�g - �ʏ��Ԃ�
//============================================================================
void CPlayer_State_Dash::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}