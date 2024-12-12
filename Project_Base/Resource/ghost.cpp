//============================================================================
// 
// �S�[�X�g [ghost.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "ghost.h"

#include "player.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************

// ��b�p�����[�^�̓W�J
const JSON CGhost::m_InitParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\ghost_param.json");

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CGhost::CGhost() :
	CEnemy{ LAYER::DEFAULT },
	m_ActionType{ ACTION::HOLD }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CGhost::~CGhost()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CGhost::Init()
{
	// �G�l�~�[�N���X�̏����ݒ�
	if (FAILED(CEnemy::Init()))
	{
		assert(false && "�S�[�X�g�N���X�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CGhost::Uninit()
{
	// �G�l�~�[�N���X�̏I������
	CEnemy::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CGhost::Update()
{
	// �s������
	BranchAction();

	// �G�l�~�[�N���X�̍X�V����
	CEnemy::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CGhost::Draw()
{

}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CGhost::SetDamage(int nDamage)
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
CGhost* CGhost::Create()
{
	// �C���X�^���X�𐶐�
	CGhost* pNewInstance = DBG_NEW CGhost();

	// �^�C�v��ݒ�
	pNewInstance->CObject::SetType(TYPE::ENEMY);

	// �����ݒ�
	pNewInstance->Init();

	// ���[�V�������Z�b�g
	pNewInstance->CCharacter::SetMotion(LAYER::STENCIL, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\ghost_motion.json"));

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
void CGhost::BranchAction()
{
	// �v���C���[�����݂��Ȃ���΃A�N�V���������Ȃ�
	if (!FindPlayer())
		return;

	// �^�C�v�ɉ����ď�����ύX
	switch (m_ActionType)
	{
		// �Y��
	case ACTION::HOLD:
		Hold();
		break;

		// ���ł���
	case ACTION::FLY:
		Fly();
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
// �Y��
//============================================================================
void CGhost::Hold()
{
	// �ҋ@���[�V�����ɕύX
	if (GetNowMotion() != 0)
	{
		SetNowMotion(0);
	}

	// �v���C���[���擾
	CPlayer* const pPlayer = FindPlayer();

	// �v���C���[�Ƃ̋����̍����o��
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// �v���C���[�ɋ߂���Δ��ł���
	if (Norm.x * Norm.x + Norm.y * Norm.y + Norm.z * Norm.z < 5000.0f)
	{
		m_ActionType = ACTION::FLY;
	}

	// �Փˌ��o
	HitCheck();
}

//============================================================================
// ���ł���
//============================================================================
void CGhost::Fly()
{
	// ���ł��郂�[�V�����ɕύX
	if (GetNowMotion() != 1)
	{
		SetNowMotion(1);
	}

	// �Փˌ��o
	HitCheck();

	// �v���C���[���擾
	CPlayer* const pPlayer = FindPlayer();

	// �v���C���[�Ƃ̋����̍����o��
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// �v���C���[���牓�����
	if (Norm.x * Norm.x + Norm.y * Norm.y + Norm.z * Norm.z > 8000.0f)
	{
		// �A�N�V�����^�C�v��Y���ɕύX
		m_ActionType = ACTION::HOLD;

		// �ȍ~�̏����͍s��Ȃ�
		return;
	}

	// ���g�̍��W���R�s�[
	Vec3 Pos = GetPos();

	// �v���C���[�Ƃ̍��W�̍����o��
	Vec3 fDifference = pPlayer->GetPos() - Pos;

	// ���𖄂߂Ă���
	Pos += fDifference * 0.5f;

	// �ڕW���W��ݒ�
	SetPosTarget(Pos);

	// �ړ������Ɍ��������킹��
	AutoSetRotTarget();
}

//============================================================================
// ���S
//============================================================================
void CGhost::DeadEnd()
{
	// ���S���[�V�����ɕύX
	if (GetNowMotion() != 2)
	{
		SetNowMotion(2);
	}

	// ���S���[�V�����̍Đ����I��������
	if (GetStopState())
	{
		// �j��
		SetRelease();
	}
}