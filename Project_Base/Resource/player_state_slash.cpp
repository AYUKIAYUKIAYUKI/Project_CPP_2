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
#include "object_X.h"
#include "player_state_default.h"
#include "player_state_damage.h"
#include "collision.h"
#include "constellation.h"

// �t�B�[���h�T�C�Y�擾�p
#include "field_manager.h"

// �C���v�b�g�擾�p
#include "manager.h"

#include "boss.h"

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
	m_pSlashModel{ CObject_X::Create(CX_Manager::TYPE::PLAHEAD) },
	m_pBndSlash{ std::make_unique<CBounding_Sphere>() }
{
	// �a�����f���̍��W��ݒ� (��������̍��W��1f�`�悳��Ă��܂�����)
	m_pSlashModel->SetPos({ 0.0f, -1000.0f, 0.0f });

	// �a���̃o�E���f�B���O�̃T�C�Y��ݒ�
	m_pBndSlash->SetRadius(8.0f);

	// �a�����[�V�������Đ�
	m_pCharacter->SetNowMotion(5);
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

	// �a�����f����j��
	m_pSlashModel->SetRelease();
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer_State_Slash::Update()
{
	// �v���C���[�X�e�[�g�N���X�̍X�V����
	CPlayer_State::Update();

	// �v���C���[�̌����Ă�������̃x�N�g�����쐬
	Vec3 PlayerFacing =
	{
		m_pCharacter->GetPos().x + -sinf(m_pCharacter->GetRot().y) * 15.0f,
		m_pCharacter->GetPos().y,
		m_pCharacter->GetPos().z + -cosf(m_pCharacter->GetRot().y) * 15.0f
	};

	// �a�����f������]
	Vec3 NewRot = m_pSlashModel->GetRot();
	NewRot.y += 1.0f;
	m_pSlashModel->SetRot(NewRot);

	// �a�����f���̍��W��ݒ�
	m_pSlashModel->SetPos(PlayerFacing);

	// �a���o�E���f�B���O�̒��S�_��ݒ�
	m_pBndSlash->SetCenterPos(PlayerFacing);

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
#if 0
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	while (pObj != nullptr)
	{
		// �L�����N�^�[�N���X�Ƀ_�E���L���X�g
		CCharacter* pCharacter = CObject::DownCast<CCharacter>(pObj);

		// �L���X�g���s��
		if (!pCharacter)
		{
			pObj = pObj->GetNext();
			continue;
		}

		// �U���ƓG�L�����N�^�[�̓����蔻��

		// �_���[�W��^����
		pCharacter->SetDamage(-1);

		pObj = pObj->GetNext();
	}
#else // ���i�K�̂�

	// �{�X�����݂��Ă��Ȃ���΂��̃��\�b�h�𖳎�
	if (!CObject::FindSpecificObject(CObject::TYPE::BOSS))
		return false;

	// �{�X�̉~���o�E���f�B���O���擾
	CBoss* pBoss = utility::DownCast<CBoss, CObject>(CObject::FindSpecificObject(CObject::TYPE::BOSS));

	// �a���o�E���f�B���O���R�s�[
	const CBounding_Sphere* pOther = m_pBndSlash.get();

	// �a���o�E���f�B���O�ƃ{�X�̉~���o�E���f�B���O�œ����蔻��
	if (collision::HitCylinderToSphere(pBoss->GetBndCylinder(), pOther))
		pBoss->SetDamage(-1);

#endif

	return false;
}