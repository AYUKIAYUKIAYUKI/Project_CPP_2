//============================================================================
// 
// �����X�^�[ [monster.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "monster.h"

#include "player.h"
#include "block.h"

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
CMonster::CMonster() :
	CEnemy{},
	m_ActionType{ ACTION::HOLD },
	m_nCntActionCast{ 0 }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CMonster::~CMonster()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CMonster::Init()
{
	// �␳���x��ݒ�
	SetCorrectCoef(CORRECT_COEF);

	// �����ړ����x��ݒ�
	SetMoveSpeed(DEFAULT_MOVE_SPEED);

	// �����̗͂�ݒ�
	SetLife(MAX_LIFE);

	// �G�l�~�[�N���X�̏����ݒ�
	if (FAILED(CEnemy::Init()))
	{
		assert(false && "�����X�^�[�N���X�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CMonster::Uninit()
{
	// �G�l�~�[�N���X�̏I������
	CEnemy::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CMonster::Update()
{
	// �s������
	BranchAction();

	// �G�l�~�[�N���X�̍X�V����
	CEnemy::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CMonster::Draw()
{
	// �G�l�~�[�N���X�̕`�揈��
	CEnemy::Draw();
}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CMonster::SetDamage(int nDamage)
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
CMonster* CMonster::Create()
{
	// �C���X�^���X�𐶐�
	CMonster* pNewInstance = DBG_NEW CMonster();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::ENEMY);

	// �����ݒ�
	pNewInstance->Init();

	// ���[�V�������Z�b�g
	pNewInstance->SetMotion(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\enemy_motion.json"));

	// �o�E���f�B���O�T�C�Y��ݒ�
	pNewInstance->SetBndSize(3.0f, 3.0f);

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �s������
//============================================================================
void CMonster::BranchAction()
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
void CMonster::Hold()
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
void CMonster::Coming()
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
	{
		SetDirectionTarget(fDirectionTarget + DEFAULT_MOVE_SPEED);
	}
	else
	{
		SetDirectionTarget(fDirectionTarget + -DEFAULT_MOVE_SPEED);
	}

	// �ړ������Ɍ��������킹��
	AutoSetRotTarget();
}

//============================================================================
// �u���b�N�̋߂��Ŏ~�܂�
//============================================================================
bool CMonster::StopBlockSide()
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

#ifdef _DEBUG
			ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Test")) {
				ImGui::Text("Y:%f", Norm.y * Norm.y);
				ImGui::End();
			}
#endif // _DEBUG

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
void CMonster::GoBack()
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
void CMonster::DeadEnd()
{

}