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
#include "player_state_default.h"
#include "player_state_dash.h"
#include "player_state_wall.h"
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
CPlayer_State_Jump::CPlayer_State_Jump() :
	CPlayer_State{},
	m_nJumpRemainDuration{ 0 },
	m_bEndRemain{ false }
{
	// �A�N�V�����f�[�^�̃W�����v�����񐔂��C���N�������g
	CField_Manager::GetInstance()->IncrementCntJump();
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
	// �v���C���[�X�e�[�g�N���X�̍X�V����
	CPlayer_State::Update();

	// Y�������̉����x�������AY�����̈ړ��ڕW�̃m��������������
	if (m_pCharacter->GetVelY() == 0.0f &&
		fabsf(m_pCharacter->GetPosTarget().y - m_pCharacter->GetPos().y) <= 0.1f)
	{
		// �ʏ��Ԃ�
		To_Default();
	}

	// �d�͂̕␳
	AdjustGravity();

	// ����
	Control();
}

//============================================================================
// �ʏ��Ԃ�
//============================================================================
void CPlayer_State_Jump::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}

//============================================================================
// �_�b�V����Ԃ�
//============================================================================
void CPlayer_State_Jump::To_Dash()
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
// �ǂ����Ԃ�
//============================================================================
void CPlayer_State_Jump::To_Wall()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Wall ());
	}
}

//============================================================================
// �a����Ԃ�
//============================================================================
void CPlayer_State_Jump::To_Slash()
{
	// �a���̃L���X�g�����܂��Ă���Ώ������s��Ȃ�
	if (m_nCntSlashCast > 0)
		return;

	if (GetNextState() == nullptr)
	{
		// �čU���܂ł̃L���X�g�J�E���g��ݒ�
		m_nCntSlashCast = CPlayer_State_Slash::MAX_SLASH_CAST;

		// �y���ӂ���ƕ����オ��悤�ȉ����x����������
		m_pCharacter->SetVelY(0.35f);

		SetNextState(DBG_NEW CPlayer_State_Slash());
	}
}

//============================================================================
// �_���[�W��Ԃ�
//============================================================================
void CPlayer_State_Jump::To_Damage()
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
// ����
//============================================================================
void CPlayer_State_Jump::Control()
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
		fDirectionTarget += -fMoveSpeed * 1.25f;

		// �����ŖڕW���W��ϓ��������p�ɍ��킹��
		m_pCharacter->AutoSetRotTarget();

		// �ړ����͎��̂�
		if (pMouse->GetTrigger(1))
		{
			// �_�b�V����Ԃ�
			To_Dash();
		}
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{ // �J�������猩�ĉE��

		 // ���p��ϓ�
		fDirectionTarget += fMoveSpeed * 1.25f;

		// �����ŖڕW���W��ϓ��������p�ɍ��킹��
		m_pCharacter->AutoSetRotTarget();

		// �ړ����͎��̂�
		if (pMouse->GetTrigger(1))
		{
			// �_�b�V����Ԃ�
			To_Dash();
		}
	}

	if (pKeyboard->GetTrigger(DIK_SPACE))
	{
		// �a����Ԃ�
		To_Slash();
	}

	// ���p�𔽉f
	m_pCharacter->SetDirectionTarget(fDirectionTarget);
}

//============================================================================
// �d�͂̕␳
//============================================================================
void CPlayer_State_Jump::AdjustGravity()
{
	// ���͉�������
	m_nJumpRemainDuration++;

	// �W�����v�̓��͂���߂邩�A���͂̌p�����Ԃ��ő剄���P�\�ɒB����Ɖ������I������
	if (CManager::GetMouse()->GetRelease(0) || m_nJumpRemainDuration > MAX_JUMPREMAIN_DURATION)
	{
		m_bEndRemain = true;
	}

	// �d�͉���
	if (!m_bEndRemain)
	{
		JSON Json = utility::OpenJsonFile("Data\\debug.json");

		// �W�����v�������͂킸���ɏd�͂ɋt�炤
		m_pCharacter->SetVelY(m_pCharacter->GetVelY() + -CField_Manager::FIELD_GRAVITY * static_cast<float>(Json["Anti"]));
	}
	else
	{
		// �������Ԃ��I������ƒʏ�̏d�͉������s��
		m_pCharacter->SetVelY(m_pCharacter->GetVelY() + CField_Manager::FIELD_GRAVITY);
	}
}