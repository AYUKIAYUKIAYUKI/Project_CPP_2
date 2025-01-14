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
#include "sound.h"
#include "constellation.h"

// �A�N�V�����f�[�^�p
#include "field_manager.h"
#include "field_builder.h"

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
	m_bDirection{ false }
{
	// �A�N�V�����f�[�^�̃_�b�V�������񐔂��C���N�������g
	CField_Manager::GetInstance()->GetFieldBuilder()->IncrementCntDash();

	// ���E�̓��͂ɍ��킹�Ĉړ������̃t���O��ύX
	if (CManager::GetKeyboard()->GetPress(DIK_A) || CManager::GetKeyboard()->GetRelease(DIK_A))
		m_bDirection = 0;
	else
		m_bDirection = 1;

	// �_�b�V�����[�V�������Đ�
	m_pCharacter->SetNowMotion(6);

	// �_�b�V����炷
	CSound::GetInstance()->Play(CSound::LABEL::DASH);
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
	// �v���C���[�X�e�[�g�N���X�̍X�V����
	CPlayer_State::Update();

	// �����G�t�F�N�g�𔭐�
	for (WORD wCnt = 0; wCnt < 2; ++wCnt)
	{
		CConstellation::GenerateSpread(m_pCharacter->GetPos());
		CConstellation::GenerateSpread(m_pCharacter->GetPosTarget());
	}

	// Y�������̉����x�𖳂���
	m_pCharacter->SetVelY(0.0f);

	// �ڕW���W���_�b�V�������ɑ���
	SetPosToFacing();

	// �p�����Ԃ��ő�ɓ��B��
	if (m_pCharacter->GetStopState())
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

		// �󒆂̃_�b�V��������ɒ��n�����Ă��܂�Ȃ�����
		m_pCharacter->SetVelY(-0.001f);

		// �������l�����āA��s���[�V�������Z�b�g���Ȃ���ʏ��Ԃ�
		m_pCharacter->SetNowMotion(8);
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
void CPlayer_State_Dash::SetPosToFacing()
{
	// �ڕW���p���R�s�[
	float fDirectionTarget = m_pCharacter->GetDirectionTarget();

	// �ݒ肳��Ă���ړ������ɕ��p�𑝉�
	if (m_bDirection)
	{
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * 1.5f;
	}
	else
	{
		fDirectionTarget += m_pCharacter->GetMoveSpeed() * -1.5f;
	}

	// �ڕW���p�𔽉f
	m_pCharacter->SetDirectionTarget(fDirectionTarget);

	// �����ŖڕW���W��ϓ��������p�ɍ��킹��
	m_pCharacter->AutoSetRotTarget();
}