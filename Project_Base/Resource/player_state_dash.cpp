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
#include "constellation.h"

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
	// �A�N�V�����f�[�^�̃_�b�V�������񐔂��C���N�������g
	CField_Manager::GetInstance()->IncrementCntDash();

	// ���E�̓��͂ɍ��킹�Ĉړ������̃t���O��ύX
	if (CManager::GetKeyboard()->GetPress(DIK_A) || CManager::GetKeyboard()->GetRelease(DIK_A))
		m_bDirection = 0;
	else
		m_bDirection = 1;

	// Y�������̉����x�𖳂���
	m_pCharacter->SetVelY(0.0f);
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
	// �����G�t�F�N�g�𔭐�
	for (int nCnt = 0; nCnt < 2; ++nCnt)
		CConstellation::GenerateSpread(m_pCharacter->GetPos());
		CConstellation::GenerateSpread(m_pCharacter->GetPosTarget());

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
	// �ڕW���p���R�s�[
	float fDirectionTarget = m_pCharacter->GetDirectionTarget();

	// �ݒ肳��Ă���ړ������ɕ��p�𑝉�
	if (m_bDirection)
	{
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * 3.0f;
	}
	else
	{
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * -3.0f;
	}

	// �ڕW���p�𔽉f
	m_pCharacter->SetDirectionTarget(fDirectionTarget);

	// �����ŖڕW���W��ϓ��������p�ɍ��킹��
	m_pCharacter->AutoSetRotTarget();
}