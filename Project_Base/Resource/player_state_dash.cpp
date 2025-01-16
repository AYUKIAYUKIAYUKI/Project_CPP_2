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
#include "object_X.h"
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
	m_bDirection{ false },
	m_nDuration{ 0 }
{
	// �A�N�V�����f�[�^�̃_�b�V�������񐔂��C���N�������g
	CField_Manager::GetInstance()->GetFieldBuilder()->IncrementCntDash();

	// ���E�̓��͂ɍ��킹�Ĉړ������̃t���O��ύX
	if (CManager::GetKeyboard()->GetPress(DIK_A) || CManager::GetKeyboard()->GetRelease(DIK_A) ||
		CManager::GetPad()->GetPress(CInputPad::JOYKEY::LEFT) || CManager::GetPad()->GetRelease(CInputPad::JOYKEY::LEFT) ||
		CManager::GetPad()->GetJoyStickL().X < 0)
		m_bDirection = 0;
	else
		m_bDirection = 1;

	// �L�����N�^�[�̃|�C���^���v���C���[�N���X�Ƀ_�E���L���X�g
	CPlayer* pPlayer = utility::DownCast<CPlayer, CCharacter>(m_pCharacter);

	// �������ʂ��L������
	if (pPlayer->IsEnabledBoots())
	{
		// ���ꑖ�s���[�V�������Đ�
		m_pCharacter->SetNowMotion(11);
	}
	else
	{
		// �_�b�V�����[�V�������Đ�
		m_pCharacter->SetNowMotion(6);
	}

	// �_�b�V����炷
	CSound::GetInstance()->Play(CSound::LABEL::DASH);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer_State_Dash::~CPlayer_State_Dash()
{
	// �S�ẴG�t�F�N�g��
	for (auto it : m_vEffect)
	{
		// �j���ݒ�
		it->SetRelease();
	}
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer_State_Dash::Update()
{
	// �v���C���[�X�e�[�g�N���X�̍X�V����
	CPlayer_State::Update();

	// �p�����Ԃ��C���N�������g
	++m_nDuration;

	// �����G�t�F�N�g�𔭐�
	for (WORD wCnt = 0; wCnt < 2; ++wCnt)
	{
		CConstellation::GenerateSpread(m_pCharacter->GetPos());
		CConstellation::GenerateSpread(m_pCharacter->GetPosTarget());
	}

	// �L�����N�^�[�̃|�C���^���v���C���[�N���X�Ƀ_�E���L���X�g
	CPlayer* pPlayer = utility::DownCast<CPlayer, CCharacter>(m_pCharacter);

	// �������ʂ��L������
	if (pPlayer->IsEnabledBoots())
	{
		// ���[���G�t�F�N�g���쐬
		CObject_X* pObj = CObject_X::Create(CX_Manager::TYPE::SUM0 + (m_nDuration % 12));

		// ���W�E�����𒲐�
		pObj->SetPos(pPlayer->GetPos());
		pObj->SetRot(pPlayer->GetRot());

		// �I�u�W�F�N�g��ێ�
		m_vEffect.push_back(pObj);

		// �k�ڂ��쐬
		float fScale = static_cast<float>(35 - m_nDuration) * 0.075f;

		// �S�ẴG�t�F�N�g��
		for (auto it : m_vEffect)
		{
			// �g��
			it->SetScale({ fScale, fScale, fScale });
		}
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