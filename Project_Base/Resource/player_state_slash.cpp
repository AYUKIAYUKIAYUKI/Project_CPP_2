//============================================================================
//
// �v���C���[�a���X�e�[�g [player_state_slash.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state_slash.h"
#include "bright.h"
#include "player_state_default.h"
#include "player_state_damage.h"

#include "manager.h"
#include "sound.h"
#include "collision.h"
#include "field_manager.h"

#include "constellation.h"
#include "enemy.h"
#include "monster.h"
#include "boss.h"
#include "stone.h"

// �A�N�V�����f�[�^�p
#include "field_builder.h"

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
CPlayer_State_Slash::CPlayer_State_Slash() :
	CPlayer_State{},
	m_SlashType{ SLASH_TYPE::LEFT },
	m_pBndSlash{ std::make_unique<CBounding_Sphere>() }
{
	// �A�N�V�����f�[�^�̎a�������񐔂��C���N�������g
	CField_Manager::GetInstance()->GetFieldBuilder()->IncrementCntSlash();

	// �a���̃o�E���f�B���O�̃T�C�Y��ݒ�
	m_pBndSlash->SetRadius(6.0f);

	// �a�����[�V�������Đ�
	m_pCharacter->SetNowMotion(5);

	// �a��SE��炷
	CSound::GetInstance()->Play(CSound::LABEL::SLASH);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer_State_Slash::~CPlayer_State_Slash()
{
	// �a���̃o�E���f�B���O��j��
	if (m_pBndSlash != nullptr)
	{
		m_pBndSlash.reset();
	}
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer_State_Slash::Update()
{
	// �v���C���[�X�e�[�g�N���X�̍X�V����
	CPlayer_State::Update();

	// ���͎擾�p
	CInputKeyboard* pKeyBoard = CManager::GetManager()->GetKeyboard();
	CInputPad* pPad = CManager::GetManager()->GetPad();

	// �@�����̏Ռ��ʂ��쐬
	float fImpact = -0.001f;

	// �A���E�ǂ���������Ă��邩����
	bool bFaceSide = m_pCharacter->CheckFacingSide();

	// �B������t�������������
	if (!bFaceSide)
	{
		fImpact *= -1.0f;
	}

	// �C���͕����ɉ����Ĕ����ʂ������ω�
	if (bFaceSide && pKeyBoard->GetPress(DIK_A))
	{ // �E�����ō����͒�

		// ��������
		fImpact *= 1.5f;
	}
	else if (bFaceSide && pKeyBoard->GetPress(DIK_D))
	{ // �E�����ŉE���͒�

		// �����ጸ
		fImpact *= 0.25f;
	}
	else if (!bFaceSide && pKeyBoard->GetPress(DIK_A))
	{ // �������ō����͒�

		// �����ጸ
		fImpact *= 0.25f;
	}
	else if (!bFaceSide && pKeyBoard->GetPress(DIK_D))
	{ // �������ŉE���͒�

		// ��������
		fImpact *= 1.5f;
	}

	// �D�����Ō��
	float fNewDirectionTarget = m_pCharacter->GetDirectionTarget();
	fNewDirectionTarget += fImpact;
	m_pCharacter->SetDirectionTarget(fNewDirectionTarget);

	// �����Ă�������̃x�N�g�����쐬
	Vec3 PlayerFacing =
	{
		m_pCharacter->GetPos().x + -sinf(m_pCharacter->GetRot().y) * 12.0f,
		m_pCharacter->GetPos().y + 3.5f,
		m_pCharacter->GetPos().z + -cosf(m_pCharacter->GetRot().y) * 12.0f
	};

	// �a���o�E���f�B���O�̒��S�_��ݒ�
	m_pBndSlash->SetCenterPos(PlayerFacing);

	// �L�����N�^�[�̃|�C���^���v���C���[�N���X�Ƀ_�E���L���X�g
	CPlayer* pPlayer = utility::DownCast<CPlayer, CCharacter>(m_pCharacter);

	// �e�₪�L��������Ă�����
	if (pPlayer->IsEnabledPowerStone())
	{
		// �����ʒu�����炷
		Vec3 InitPos = m_pCharacter->GetPos();
		InitPos += {
			utility::GetRandomValue<float>() * 0.075f,
			utility::GetRandomValue<float>() * 0.05f,
			utility::GetRandomValue<float>() * 0.075f
		};

		// ��΂��������U�������ɂ���
		Vec3 Way = PlayerFacing - m_pCharacter->GetPos();
		Way.y = 0.0f;

		CStone::Create(InitPos, Way * 0.15f);
	}

	// �����G�t�F�N�g�𔭐�
	CConstellation::GenerateSpread(PlayerFacing);

	// �Փˌ��o
	SlashHitCheck();

	// �d�͉���
	m_pCharacter->SetVelY(m_pCharacter->GetVelY() + CField_Manager::FIELD_GRAVITY * 0.25f);

	// �a�����[�V��������~������
	if (m_pCharacter->GetStopState())
	{
		// �ʏ탂�[�V������
		To_Default();
	}
}

//============================================================================
// �ʏ��Ԃ�
//============================================================================
void CPlayer_State_Slash::To_Default()
{
	if (GetNextState() == nullptr)
	{
		SetNextState(DBG_NEW CPlayer_State_Default());
	}
}

//============================================================================
// �_���[�W��Ԃ�
//============================================================================
void CPlayer_State_Slash::To_Damage()
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
// �d�͂̕␳
//============================================================================
void CPlayer_State_Slash::AdjustGravity()
{

}

//============================================================================
// �a�������蔻��
//============================================================================
bool CPlayer_State_Slash::SlashHitCheck()
{
	// �f�t�H���g�I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// �I�u�W�F�N�g���X�g����
	while (pObj != nullptr)
	{
		// �I�u�W�F�N�g�̃^�C�v�ɉ����ď�������
		if (pObj->GetType() == CObject::TYPE::ENEMY)
		{ // �G�l�~�[�^�C�v�Ȃ�

			// �G�l�~�[�N���X�ɃL���X�g
			CEnemy* pEnemy = CObject::DownCast<CEnemy>(pObj);

			// �a���o�E���f�B���O���R�s�[
			const CBounding_Sphere* pSlash = m_pBndSlash.get();

			// �U���ƃG�l�~�[�ł̓����蔻��
			if (collision::HitCylinderToSphere(pEnemy->CEnemy::GetBndCylinder(), pSlash))
			{
				// �_���[�W��^����
				pEnemy->SetDamage(-1);
			}
		}
		else if (pObj->GetType() == CObject::TYPE::BOSS)
		{ // �{�X�^�C�v�Ȃ�

			// �{�X�N���X�ɃL���X�g
			CBoss* pBoss = CObject::DownCast<CBoss>(pObj);

			// �a���o�E���f�B���O���R�s�[
			const CBounding_Sphere* pSlash = m_pBndSlash.get();

			// �U���ƃ{�X�ł̓����蔻��
			if (collision::HitCylinderToSphere(pBoss->GetBndCylinder(), pSlash))
			{
				// �_���[�W��^����
				pBoss->SetDamage(-1);
			}
		}

		// ���̃I�u�W�F�N�g��
		pObj = pObj->GetNext();
	}

	return false;
}