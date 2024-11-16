//============================================================================
// 
// �΂̕� [sparks.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "sparks.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
WORD CSparks::m_nCntGenerateSpan = 0;	// �����X�p���̃J�E���g

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CSparks::CSparks() :
	CObject_Effect{},
	m_InitPos{ VEC3_INIT },
	m_fWaveCoef{ 0.0f },
	m_fAdderY{ 0.0f }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CSparks::~CSparks()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CSparks::Init()
{
	// �G�t�F�N�g�I�u�W�F�N�g�̏����ݒ�
	if (FAILED(CObject_Effect::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CSparks::Uninit()
{
	// �G�t�F�N�g�I�u�W�F�N�g�̏I������
	CObject_Effect::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CSparks::Update()
{
	// �����̔����ɓ��B��������ł̏����ɓ���
	if (GetDuration() == GetMaxDuration() * 0.5f)
	{
		// �ڕW�T�C�Y���k��
		SetSizeTarget(VEC3_INIT);

		// �J���[�𔖍���
		SetColTarget(XCol(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// ���ݒn����ɁA����p�̖ڕW���l��ݒ肵�Ă���
	Vec3 NewPosTarget = GetPos();
	NewPosTarget += { cosf(NewPosTarget.y) * m_fWaveCoef, m_fAdderY, cosf(NewPosTarget.y) * m_fWaveCoef};
	SetPosTarget(NewPosTarget);

	// �G�t�F�N�g�I�u�W�F�N�g�̍X�V����
	CObject_Effect::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CSparks::Draw()
{
	// �G�t�F�N�g�I�u�W�F�N�g�̕`�揈��
	CObject_Effect::Draw();
}

//============================================================================
// ��������
//============================================================================
void CSparks::AutoGenerate()
{
	// �����X�p�����J�E���g�A�b�v
	m_nCntGenerateSpan++;

	// �ݒ肳�ꂽ�����X�p���ɓ��B��
	if (m_nCntGenerateSpan > NUM_GENERATE_SPAN)
	{
		// �����X�p���̃J�E���g�����Z�b�g
		m_nCntGenerateSpan = 0;

		// ����
		Create();
	}
}

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
void CSparks::Create()
{
	CSparks* pNewInstance = DBG_NEW CSparks();

	// �������s
	if (!pNewInstance)
	{
		assert(false && "�΂̕��̐����Ɏ��s");
	}

	// �΂̕��̏����ݒ�
	pNewInstance->Init();

	// ��b�p�����[�^���擾
	JSON Json = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\sparks.json");

	// �p�����[�^���R�s�[
	auto const& TextureType = Json["TextureType"];
	auto const& CorrectionCoef = Json["CorrectionCoef"];
	auto const& SizeTarget = Json["SizeTarget"];
	auto const& ColTarget = Json["ColTarget"];
	auto const& MaxDuration = Json["MaxDuration"];
	auto const& WaveCoef = Json["WaveCoef"];
	auto const& AdderY = Json["AdderY"];

	// �����_���ȍ��W���쐬
	pNewInstance->m_InitPos = {
		utility::GetRandomValue<float>() * 1.5f,
		fabsf(utility::GetRandomValue<float>()),
		utility::GetRandomValue<float>() * 1.5f
	};

	// �e��p�����[�^�ݒ�
	pNewInstance->BindTex(static_cast<CTexture_Manager::TYPE>(TextureType));					// �e�N�X�`��
	pNewInstance->SetCorrectionCoef(static_cast<float>(CorrectionCoef));						// �␳�W��
	pNewInstance->SetSizeTarget(Vec3(SizeTarget[0], SizeTarget[1], SizeTarget[2]));				// �ڕW�T�C�Y
	pNewInstance->SetPos(pNewInstance->m_InitPos);												// ���W
	pNewInstance->SetPosTarget(pNewInstance->m_InitPos);										// �ڕW���W
	pNewInstance->SetColTarget(XCol(ColTarget[0], ColTarget[1], ColTarget[2], ColTarget[3]));	// �ڕW�F
	pNewInstance->SetMaxDuration(static_cast<int>(MaxDuration));								// �ő����
	pNewInstance->m_fWaveCoef = static_cast<float>(WaveCoef);									// �g�ł����x
	pNewInstance->m_fAdderY = static_cast<float>(AdderY);										// �㏸��
}