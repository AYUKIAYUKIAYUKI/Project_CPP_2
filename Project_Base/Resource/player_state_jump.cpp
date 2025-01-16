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
#include "constellation.h"

// �t�B�[���h�p�����[�^�p
#include "field_manager.h"

// �A�N�V�����f�[�^�p
#include "field_builder.h"

// �C���v�b�g�擾�p
#include "manager.h"

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************

// �W�����v�p�p�����[�^
JSON CPlayer_State_Jump::m_JumpParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\jump_param.json");

// �W�����v��񓖂���̑��������x
float CPlayer_State_Jump::AMOUNT_JUMPACCEL = static_cast<float>(m_JumpParam["Power"]);

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
#ifdef _DEBUG
	// �p�����[�^�̍ēǂݍ���
	m_JumpParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\jump_param.json");
	AMOUNT_JUMPACCEL = static_cast<float>(m_JumpParam["Power"]);
#endif // _DEBUG

	// �A�N�V�����f�[�^�̃W�����v�����񐔂��C���N�������g
	CField_Manager::GetInstance()->GetFieldBuilder()->IncrementCntJump();

	// ��ї������[�V�������Đ�
	m_pCharacter->SetNowMotion(7);
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

	// �����G�t�F�N�g�𔭐�
	CConstellation::GenerateSpread(m_pCharacter->GetPos());

	// ��ї������[�V�������I�����Ă�����
	if (m_pCharacter->GetNowMotion() == 7 &&
		m_pCharacter->GetStopState())
	{
		// ��s���[�V�����ɕύX
		m_pCharacter->SetNowMotion(8);
	}

	// Y�������̉����x�������AY�����̈ړ��ڕW�̃m��������������
	if (m_pCharacter->GetVelY() == 0.0f &&
		fabsf(m_pCharacter->GetPosTarget().y - m_pCharacter->GetPos().y) <= 0.3f)
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
		m_pCharacter->SetVelY(1.0f);

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

	if (pMouse->GetTrigger(0))
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
	if (CManager::GetKeyboard()->GetRelease(DIK_SPACE) || CManager::GetKeyboard()->GetRelease(DIK_W) ||
		m_nJumpRemainDuration > static_cast<float>(m_JumpParam["Duration"]))
	{
		m_bEndRemain = true;
	}

	// �d�͉���
	if (!m_bEndRemain)
	{
		// �W�����v�������͂킸���ɏd�͂ɋt�炤
		m_pCharacter->SetVelY(m_pCharacter->GetVelY() + -CField_Manager::FIELD_GRAVITY * static_cast<float>(m_JumpParam["AntiPower"]));
	}
	else
	{
		// �������Ԃ��I������ƒʏ�̏d�͉������s��
		m_pCharacter->SetVelY(m_pCharacter->GetVelY() + CField_Manager::FIELD_GRAVITY);
	}
}