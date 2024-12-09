//============================================================================
// 
// �G�l�~�[ [enemy.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "enemy.h"
#include "bounding_cylinder.h"

#include "collision.h"
#include "player.h"
#include "player_state_damage.h"
#include "block.h"
#include "renderer.h"

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
CEnemy::CEnemy() :
	CCharacter{},
	m_ActionType{ ACTION::HOLD },
	m_nCntActionCast{ 0 },
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CEnemy::~CEnemy()
{
	// �o�E���f�B���O�V�����_�[�̔j��
	if (m_pBndCylinder != nullptr)
	{
		// �����������
		delete m_pBndCylinder;

		// �|�C���^��������
		m_pBndCylinder = nullptr;
	}
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CEnemy::Init()
{
	// �␳���x��ݒ�
	SetCorrectCoef(CORRECT_COEF);

	// �����ړ����x��ݒ�
	SetMoveSpeed(DEFAULT_MOVE_SPEED);

	// �����̗͂�ݒ�
	SetLife(MAX_LIFE);

	// �L�����N�^�[�N���X�̏����ݒ�
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "�G�l�~�[�N���X�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CEnemy::Uninit()
{
	// �L�����N�^�[�N���X�̏I������
	CCharacter::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CEnemy::Update()
{
	// ���̍s��������
	SetNextAction();

	// �s������
	BranchAction();

	// �����̕␳
	AdjustHeight();

	// ���p�ɍ��W�����킹��
	AutoSetPosTarget();

	// �����蔻��̒��S�_��ݒ�
	m_pBndCylinder->SetCenterPos(GetPos());

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CEnemy::Draw()
{
	// �L�����N�^�[�̕`�揈��
	CCharacter::Draw();
}

//============================================================================
// ���a���擾
//============================================================================
float CEnemy::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// �������擾
//============================================================================
float CEnemy::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// �~���o�E���f�B���O���擾
//============================================================================
const CBounding_Cylinder* const CEnemy::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CEnemy::SetDamage(int nDamage)
{
	// �_���[�W�ʕ��A�̗͂�ϓ�
	int nNewLife = GetLife();
	nNewLife += nDamage;
	SetLife(nNewLife);

	// ���C�t�������Ȃ�����
	if (nNewLife < 0)
	{
		// �A�N�V�����^�C�v�����S�ɕύX
		m_ActionType = ACTION::DEADEND;
	}
}

//============================================================================
// ����
//============================================================================
CEnemy* CEnemy::Create()
{
	// �C���X�^���X�𐶐�
	CEnemy* pNewInstance = DBG_NEW CEnemy();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::ENEMY);

	// �����ݒ�
	pNewInstance->Init();

	// ���[�V�������Z�b�g
	pNewInstance->SetMotion(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\enemy_motion.json"));

	// ���a��ݒ�
	pNewInstance->m_pBndCylinder->SetRadius(3.0f);

	// ������ݒ�
	pNewInstance->m_pBndCylinder->SetHeight(3.0f);

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���̍s��������
//============================================================================
void CEnemy::SetNextAction()
{
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Enemy")) {
		ImGui::Text("%d", m_ActionType);
		ImGui::End();
	}
}

//============================================================================
// �s������
//============================================================================
void CEnemy::BranchAction()
{
	// �^�C�v�ɉ����ď�����ύX
	switch (m_ActionType)
	{
		// �����~�܂�
	case ACTION::HOLD:
		Hold();
		break;

		// �����Ă���
	case ACTION::COMING:
		Coming();
		break;

		// ����������
	case ACTION::GOBACK:
		GoBack();
		break;

		// ���S
	case ACTION::DEADEND:
		DeadEnd();
		break;

		// ��O
	default:
#ifdef _DEBUG
		assert(false && "�G�l�~�[�̍s���ɗ�O����");
#else
		m_ActionType = ACTION::HOLD;
#endif // _DEBUG
		break;
	}
}

//============================================================================
// �����~�܂�
//============================================================================
void CEnemy::Hold()
{
	// �v���C���[�^�C�v�̃I�u�W�F�N�g������
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// �I�u�W�F�N�g�������炯��ΏI��
	if (pObj == nullptr)
		return;

	// �v���C���[�N���X�Ƀ_�E���L���X�g
	CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// �v���C���[�ւ̋������o��
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// �v���C���[�ɋ߂���Ε����Ă���
	if (Norm.x * Norm.x + Norm.y * Norm.y + Norm.z * Norm.z < 2000.0f)
	{
		m_ActionType = ACTION::COMING;
	}

	// �Փˌ��o
	HitCheck();
}

//============================================================================
// �����Ă���
//============================================================================
void CEnemy::Coming()
{
	// �Փˌ��o
	HitCheck();

	// �u���b�N�ɐڋ߂��Ă�����I��
	if (StopBlockSide())
		return;

	// �v���C���[�^�C�v�̃I�u�W�F�N�g������
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// �I�u�W�F�N�g�������炯��ΏI��
	if (pObj == nullptr)
		return;

	// �v���C���[�N���X�Ƀ_�E���L���X�g
	CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// ���g�̖ڕW���p���R�s�[
	float fDirectionTarget = GetDirectionTarget();

	// ���p�̍����o��
	float fDifference = pPlayer->GetDirection() - fDirectionTarget;

	// ���𖄂߂�悤�ɖڕW���p��ϓ�
	if (fDifference > 0.0f)
		SetDirectionTarget(fDirectionTarget + DEFAULT_MOVE_SPEED);
	else
		SetDirectionTarget(fDirectionTarget + -DEFAULT_MOVE_SPEED);

	// �ړ������Ɍ��������킹��
	AutoSetRotTarget();
}

//============================================================================
// �u���b�N�̋߂��Ŏ~�܂�
//============================================================================
bool CEnemy::StopBlockSide()
{
	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// �u���b�N�^�C�v�̃I�u�W�F�N�g���擾
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = utility::DownCast<CBlock, CObject>(pObj);

			// �u���b�N�ƍ��W�̋������o��
			Vec3 Norm = pBlock->GetPos() - GetPos();

			ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Test")) {
				ImGui::Text("Y:%f", Norm.y * Norm.y);
				ImGui::End();
			}

			// �������������炢�A��������x�אڂ��Ă���u���b�N������Έ���������
			if (Norm.y * Norm.y < 200.0f &&
				Norm.x * Norm.x + Norm.z * Norm.z < 1000.0f)
			{
				// �A�N�V�����^�C�v��ύX
				m_ActionType = ACTION::GOBACK;

				return true;
			}
		}

		pObj = pObj->GetNext();
	}

	return false;
}

//============================================================================
// ����������
//============================================================================
void CEnemy::GoBack()
{
	// ���g�̖ڕW���p���R�s�[
	float fDirectionTarget = GetDirectionTarget();

	// �����ɉ����Ĉ��������������ݒ�
	if (!CheckFacingSide())
	{
		fDirectionTarget += DEFAULT_MOVE_SPEED * 0.5f;
	}
	else
	{
		fDirectionTarget += -DEFAULT_MOVE_SPEED * 0.5f;
	}

	// �ڕW���p�𔽉f
	SetDirectionTarget(fDirectionTarget);

	// �s���L���X�g�J�E���g���C���N�������g
	++m_nCntActionCast;

	// 0.5�b�o�߂�����
	if (m_nCntActionCast > 30)
	{
		// �J�E���g���Z�b�g
		m_nCntActionCast = 0;

		// �A�N�V�����^�C�v�𗧂��~�܂�ɕύX
		m_ActionType = ACTION::HOLD;
	}
}

//============================================================================
// ���S
//============================================================================
void CEnemy::DeadEnd()
{

}

//============================================================================
// ������␳
//============================================================================
void CEnemy::AdjustHeight()
{
	// �ڕW���W��Y���̉����x���ϓ�����
	Vec3 NewPosTarget = GetPosTarget();
	NewPosTarget.y += GetVelY();
	SetPosTarget(NewPosTarget);

	// �����̖ڕW�ɉ�����ݒ�
	if (GetPosTarget().y < m_pBndCylinder->GetHeight())
	{
		// �ڕW���W�������ɌŒ�
		NewPosTarget.y = m_pBndCylinder->GetHeight();
		SetPosTarget(NewPosTarget);

		// Y�������̉����x��������
		SetVelY(0.0f);
	}
}

//============================================================================
// �Փˌ��o
//============================================================================
bool CEnemy::HitCheck()
{
	// �v���C���[�^�C�v�̃I�u�W�F�N�g������
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// �I�u�W�F�N�g�������炯��ΏI��
	if (pObj == nullptr)
		return false;

	// �v���C���[�N���X�Ƀ_�E���L���X�g
	CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// �v���C���[�̎��~���͈͓��ɐN�����Ă�����Փ�
	if (collision::HitCylinderToCylinder(m_pBndCylinder, pPlayer->GetBndCylinder()))
	{
		// 1�_���[�W��^����
		pPlayer->SetDamage(-1);

		return true;
	}

	return false;
}