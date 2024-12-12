//============================================================================
// 
// �t���C���[ [flyer.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "flyer.h"

#include "player.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************

// ��b�p�����[�^�̓W�J
JSON CFlyer::m_InitParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\flyer_param.json");

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CFlyer::CFlyer() :
	CEnemy{ LAYER::DEFAULT },
	m_ActionType{ ACTION::UPDOWN },
	m_bAdder{ false },
	m_fDiffFromInitY{ 0.0f },
	m_fMinRangeDiff{ 0.0f },
	m_fMaxRangeDiff{ 0.0f }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CFlyer::~CFlyer()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CFlyer::Init()
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
void CFlyer::Uninit()
{
	// �G�l�~�[�N���X�̏I������
	CEnemy::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CFlyer::Update()
{
	// �s������
	BranchAction();

	// ���p�ɍ��W�����킹��
	AutoSetPosTarget();

	// �G�l�~�[�N���X�̍X�V����
	CEnemy::Update();

#if 0
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("PosY")) {
		ImGui::Text("%f", GetPos().y);
		ImGui::Text("%f", GetPosTarget().y);
		ImGui::Text("%f", m_fDiffFromInitY);
		ImGui::End();
	}

	if (CManager::GetKeyboard()->GetRelease(DIK_RETURN))
		m_InitParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\flyer_param.json");
#endif
}

//============================================================================
// �`�揈��
//============================================================================
void CFlyer::Draw()
{

}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CFlyer::SetDamage(int nDamage)
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
CFlyer* CFlyer::Create()
{
	// �C���X�^���X�𐶐�
	CFlyer* pNewInstance = DBG_NEW CFlyer();

	// �^�C�v��ݒ�
	pNewInstance->CObject::SetType(TYPE::ENEMY);

	// �����ݒ�
	pNewInstance->Init();

	// ���[�V�������Z�b�g
	pNewInstance->CCharacter::SetMotion(LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\flyer_motion.json"));

	{ // �p�����[�^�ݒ�

		// �f�[�^���L���X�g
		int
			nLife = static_cast<int>(m_InitParam["Life"]);
		float
			fCoef = static_cast<float>(m_InitParam["Coef"]),
			fSpeed = static_cast<float>(m_InitParam["Speed"]),
			fRadius = static_cast<float>(m_InitParam["Radius"]),
			fHeight = static_cast<float>(m_InitParam["Height"]),
			fMin = static_cast<float>(m_InitParam["MinRangeDiff"]),
			fMax = static_cast<float>(m_InitParam["MaxRangeDiff"]);

		// �f�[�^���Z�b�g
		pNewInstance->CCharacter::SetCorrectCoef(fCoef);	// ��ԋ��x
		pNewInstance->CCharacter::SetMoveSpeed(fSpeed);		// �ړ����x
		pNewInstance->CCharacter::SetLife(nLife);			// �̗�
		pNewInstance->CEnemy::SetBndSize(fRadius, fHeight);	// �o�E���f�B���O�T�C�Y
		pNewInstance->m_fMinRangeDiff = fMin;				// �ω��ʂ̍Œ�l
		pNewInstance->m_fMaxRangeDiff = fMax;				// �ω��ʂ̍ő�l
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
void CFlyer::BranchAction()
{
	// �v���C���[�����݂��Ȃ���΃A�N�V���������Ȃ�
	if (!FindPlayer())
		return;

	// �^�C�v�ɉ����ď�����ύX
	switch (m_ActionType)
	{
		// �����~�܂�
	case ACTION::UPDOWN:
		UpDown();
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
		m_ActionType = ACTION::UPDOWN;
#endif // _DEBUG
		break;
	}
}

//============================================================================
// ���~
//============================================================================
void CFlyer::UpDown()
{
	// ��]���[�V�������͏��~���Ȃ�
	if (GetNowMotion() == 1 && !GetStopState())
	{
		return;
	}

	// ���~���[�V�����ɕύX
	if (GetNowMotion() != 0)
	{
		SetNowMotion(0);
	}

	// �ړ����x���R�s�[
	float fSpeed = GetMoveSpeed();

	// �����ƌ�����؂�ւ��ď㉺�ɍ����̕�Ԃ��J��Ԃ�
	if (m_bAdder)
	{
		m_fDiffFromInitY += (m_fMaxRangeDiff - m_fDiffFromInitY) * fSpeed;
	}
	else
	{
		m_fDiffFromInitY += (m_fMinRangeDiff - m_fDiffFromInitY) * fSpeed;
	}

	// �ő�l�E�Œ�l�ɂ�����x�ߎ��ő������X�C�b�`
	if (m_fDiffFromInitY > m_fMaxRangeDiff - 0.1f ||
		m_fDiffFromInitY < m_fMinRangeDiff + 0.1f)
	{
		// ��]���[�V�������Z�b�g
		SetNowMotion(1);

		// �����̕ω��ʂ����Z�b�g
		m_fDiffFromInitY = 0.0f;

		// ���~���X�C�b�`
		m_bAdder = !m_bAdder;
	}

	// �ڕW���W���R�s�[
	Vec3 PosTarget = GetPosTarget();
	PosTarget.y += m_fDiffFromInitY;
	SetPosTarget(PosTarget);

	// �Փˌ��o
	HitCheck();
}

//============================================================================
// ���S
//============================================================================
void CFlyer::DeadEnd()
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