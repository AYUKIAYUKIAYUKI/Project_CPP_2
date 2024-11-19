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
#include "player_state_default.h"
#include "player_state_damage.h"

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
// �R���X�g���N�^
//============================================================================
CPlayer_State_Dash::CPlayer_State_Dash() :
	CPlayer_State{},
	m_bDirection{ false },
	m_nDashDuration{ 0 }
{
	// ����������͂��Ă�����
	if (CManager::GetKeyboard()->GetPress(DIK_A) || CManager::GetKeyboard()->GetRelease(DIK_A))
	{
		m_bDirection = 0;
	}
	else
	{
		m_bDirection = 1;
	}
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
// �ʏ��Ԃ�
//============================================================================
void CPlayer_State_Dash::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}

//============================================================================
// �_���[�W��Ԃ�
//============================================================================
void CPlayer_State_Dash::To_Damage()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Damage());
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �ڕW���W���_�b�V�������ɑ���
//============================================================================
void CPlayer_State_Dash::SetPosTarget_Unnamed()
{
	// ���݂̕��p���擾
	float fDirection = m_pCharacter->GetDirection();

	// �ݒ肳��Ă���ړ������ɕ��p�𑝉�
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