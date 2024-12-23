//============================================================================
// 
// �M�� [bright.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bright.h"
#include "enemy.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************

// ���[�V�����f�[�^�̓W�J
const JSON CBright::m_MotionData = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\BRIGHT\\bright_motion.json");

// ��b�p�����[�^�̓W�J
const JSON CBright::m_InitParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\BRIGHT\\bright_param.json");

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CBright::CBright() :
	m_nCntDuration{ 0 },
	m_nMaxDuration{ 0 },
	m_CreateType{ CREATETYPE::NONE },
	CMotion_Set{ LAYER::DEFAULT }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CBright::~CBright()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CBright::Init()
{
	// ���[�V�����I�u�W�F�N�g�̏����ݒ�
	if (FAILED(CMotion_Set::Init()))
	{
		return E_FAIL;
	}

	// �o�����[�V�������Z�b�g
	SetNowMotion(0);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CBright::Uninit()
{
	// ���[�V�����Z�b�g�I������
	CMotion_Set::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CBright::Update()
{
	// ���Ŕ���
	if (Disappear())
		return;	// ���Ŏ��͍X�V���Ȃ�

	// �o�����[�V�������I�����Ă�����
	if (GetStopState() && GetNowMotion() == 0)
	{
		// �ʏ탂�[�V�����ɕύX
		SetNowMotion(1);
	}

	// ���[�V�����Z�b�g�̍X�V����
	CMotion_Set::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CBright::Draw()
{
	// ���[�V�����Z�b�g�̕`�揈��
	CMotion_Set::Draw();
}

//============================================================================
// ����
//============================================================================
void CBright::Generate(D3DXVECTOR3 Pos, CREATETYPE Type)
{
	Create(Pos, Type);
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
void CBright::Create(D3DXVECTOR3 Pos, CREATETYPE Type)
{
	CBright* pNewInstance = DBG_NEW CBright();

	// �������s
	if (!pNewInstance)
	{
		assert(false && "�M���̐����Ɏ��s");
	}

	// �M���̏����ݒ�
	pNewInstance->Init();

	{ // �p�����[�^��ݒ�

		// �f�[�^���L���X�g
		int
			nMaxDuration = static_cast<int>(m_InitParam["MaxDuration"]);

		// �f�[�^���Z�b�g
		pNewInstance->m_nMaxDuration = nMaxDuration;	// �ő�p������
		pNewInstance->m_CreateType = Type;				// ��������G�l�~�[�^�C�v
		pNewInstance->CMotion_Set::SetPos(Pos);			// ���W
	}

	// ���[�V�������Z�b�g
	pNewInstance->CMotion_Set::SetMotion(LAYER::DEFAULT, m_MotionData);
}

//============================================================================
// ����
//============================================================================
bool CBright::Disappear()
{
	// ���Ń��[�V�����Ŗ������
	if (GetNowMotion() != 2)
	{
		// �p�����Ԃ��C���N�������g
		++m_nCntDuration;

		// �ő�p�����ԂɒB������
		if (m_nCntDuration > m_nMaxDuration)
		{
			// ���Ń��[�V�����ɕύX
			SetNowMotion(2);

			// �G�l�~�[����ݒ肷�邽�߂̃|�C���^
			CEnemy* pEnemy = nullptr;

			switch (m_CreateType)
			{
				// ����
			case CREATETYPE::NONE:

				// �I��
				return false;

				break;

				// �����X�^�[
			case CREATETYPE::MONSTER:
				pEnemy = CMonster::Create();
				break;

				// �S�[�X�g
			case CREATETYPE::GHOST:
				pEnemy = CGhost::Create();
				break;

				// �t���C���[
			case CREATETYPE::FLYER:
				pEnemy = CFlyer::Create();
				break;

				// �����ȃ^�C�v�w��
			default:
				throw std::runtime_error("Invalid create enemy type");
				break;
			}

			// �G�l�~�[�͂��̃G�t�F�N�g�̏ꏊ�ɔz�u
			Vec3 BrightPos = GetPos();										// �G�t�F�N�g�̍��W���R�s�[
			pEnemy->SetPos(BrightPos);										// �`�悸��h�~�ɏ������W�����̏ꏊ��
			pEnemy->SetPosTarget(BrightPos);								// �ڕW���W��ύX
			pEnemy->SetDirectionTarget(atan2f(BrightPos.x, BrightPos.z));	// ���W����̕��p�����蓖��
		}

		// �I��
		return false;
	}

	// ���Ń��[�V�����Đ����I��������
	if (GetStopState())
	{
		// �j���\��
		SetRelease();

		return true;
	}

	return false;
}