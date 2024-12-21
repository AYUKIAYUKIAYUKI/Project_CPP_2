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
#include "sound.h"

#include "player.h"
#include "block.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************

// ��b�p�����[�^�̓W�J
const JSON CMonster::m_InitParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\monster_param.json");

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CMonster::CMonster() :
	CEnemy{ LAYER::DEFAULT },
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

	// ���p�ɍ��W�����킹��
	AutoSetPosTarget();

	// �G�l�~�[�N���X�̍X�V����
	CEnemy::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CMonster::Draw()
{

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
	pNewInstance->CObject::SetType(TYPE::ENEMY);

	// �����ݒ�
	pNewInstance->Init();

	// ���[�V�������Z�b�g
	pNewInstance->CCharacter::SetMotion(LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\monster_motion.json"));

	{ // �p�����[�^�ݒ�

		// �f�[�^���L���X�g
		int
			nLife = static_cast<int>(m_InitParam["Life"]);
		float
			fCoef = static_cast<float>(m_InitParam["Coef"]),
			fSpeed = static_cast<float>(m_InitParam["Speed"]),
			fRadius = static_cast<float>(m_InitParam["Radius"]),
			fHeight = static_cast<float>(m_InitParam["Height"]);

		// �f�[�^���Z�b�g
		pNewInstance->CCharacter::SetCorrectCoef(fCoef);	// ��ԋ��x
		pNewInstance->CCharacter::SetMoveSpeed(fSpeed);		// �ړ����x
		pNewInstance->CCharacter::SetLife(nLife);			// �̗�
		pNewInstance->CEnemy::SetBndSize(fRadius, fHeight);	// �o�E���f�B���O�T�C�Y
	}

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
	// �v���C���[�����݂��Ȃ���΃A�N�V���������Ȃ�
	if (!FindPlayer())
		return;

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
	// �ҋ@���[�V�����ɕύX
	if (GetNowMotion() != 0)
	{
		SetNowMotion(0);
	}

	// �v���C���[���擾
	CPlayer* const pPlayer = FindPlayer();

	// �v���C���[�ւ̋������o��
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// �v���C���[�ɋ߂���Ε����Ă���
	if (Norm.y * Norm.y <  GetHeight() * GetHeight() &&
		Norm.x * Norm.x + Norm.z * Norm.z < 2000.0f)
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
	// �����Ă��郂�[�V�����ɕύX
	if (GetNowMotion() != 1)
	{
		SetNowMotion(1);
	}

	// �Փˌ��o
	HitCheck();

	// �u���b�N�ɐڋ߂��Ă�����I��
	if (StopBlockSide())
		return;

	// �v���C���[���擾
	CPlayer* const pPlayer = FindPlayer();

	// �v���C���[�Ƃ̋����̍����o��
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// �v���C���[���牓�����
	if (Norm.x * Norm.x + Norm.z * Norm.z > 4500.0f)
	{
		// �A�N�V�����^�C�v�𗧂��~�܂�ɕύX
		m_ActionType = ACTION::HOLD;

		// �ȍ~�̏����͍s��Ȃ�
		return;
	}

	// ���g�̖ڕW���p���R�s�[
	float fDirectionTarget = GetDirectionTarget();

	// ���p�̍����o��
	float fDifference = pPlayer->GetDirection() - fDirectionTarget;

	// ���𖄂߂�悤�ɖڕW���p��ϓ�
	if (fDifference > 0.0f)
	{
		SetDirectionTarget(fDirectionTarget + static_cast<float>(m_InitParam["Speed"]));
	}
	else
	{
		SetDirectionTarget(fDirectionTarget + -static_cast<float>(m_InitParam["Speed"]));
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

			// �u���b�N�̔����Ǝ����̔��a�E�����ň��������钷�����o��
			float
				fWidth = pBlock->GetModel()->Size.x + GetRadius(),
				fHeight = pBlock->GetModel()->Size.y + GetHeight();

			// �������������炢�A��������x�אڂ��Ă���u���b�N������Έ���������
			if (Norm.y * Norm.y < fHeight * fHeight &&
				Norm.x * Norm.x + Norm.z * Norm.z < fWidth * fWidth)
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
		fDirectionTarget += static_cast<float>(m_InitParam["Speed"]) * 0.5f;
	}
	else
	{
		fDirectionTarget += -static_cast<float>(m_InitParam["Speed"]) * 0.5f;
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
	// ���S���[�V�����ɕύX
	if (GetNowMotion() != 2)
	{
		SetNowMotion(2);

		// ����SE��炷
		CSound::GetInstance()->Play(CSound::LABEL::DISAPPEAR);
	}

	// ���S���[�V�����̍Đ����I��������
	if (GetStopState())
	{
		// �j��
		SetRelease();
	}
}