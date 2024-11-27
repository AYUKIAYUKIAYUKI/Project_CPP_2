//============================================================================
// 
// ���� [constellation.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "constellation.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
WORD CConstellation::m_nCntGenerateSpan = 0;	// �����X�p���̃J�E���g

// ��b�p�����[�^�̓W�J
JSON CConstellation::n_InitParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\constellation.json");

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CConstellation::CConstellation() :
	CObject_Effect{},
	m_InitPos{ VEC3_INIT }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CConstellation::~CConstellation()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CConstellation::Init()
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
void CConstellation::Uninit()
{
	// �G�t�F�N�g�I�u�W�F�N�g�̏I������
	CObject_Effect::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CConstellation::Update()
{
	// �����̔����ɓ��B��������ł̏����ɓ���
	if (GetDuration() == GetMaxDuration() * 0.5f)
	{
		// �ڕW�T�C�Y���k��
		SetSizeTarget(VEC3_INIT);

		// �J���[�𔖍���
		SetColTarget(XCol(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// �G�t�F�N�g�I�u�W�F�N�g�̍X�V����
	CObject_Effect::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CConstellation::Draw()
{
	// �G�t�F�N�g�I�u�W�F�N�g�̕`�揈��
	CObject_Effect::Draw();
}

//============================================================================
// �g�U����
//============================================================================
void CConstellation::GenerateSpread(D3DXVECTOR3 Pos)
{
	// �����X�p�����J�E���g�A�b�v
	m_nCntGenerateSpan++;

	// �ݒ肳�ꂽ�����X�p���ɓ��B��
	if (m_nCntGenerateSpan > SPREAD_SPAN)
	{
		// �����X�p���̃J�E���g�����Z�b�g
		m_nCntGenerateSpan = 0;

		// �I�t�Z�b�g���R�s�[
		auto Offset = n_InitParam["Offset"];

		// ����������
		for (WORD wCnt = 0; wCnt < 3; ++wCnt)
		{
			// �n���ꂽ���W�������_���ɂ��炷
			Pos += {
				utility::GetRandomValue<float>() * static_cast<float>(Offset),
				utility::GetRandomValue<float>() * static_cast<float>(Offset),
				utility::GetRandomValue<float>() * static_cast<float>(Offset)
			};

			// �����̐���
			Create(Pos);
		}
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
void CConstellation::Create(D3DXVECTOR3 Pos)
{
	CConstellation* pNew = DBG_NEW CConstellation();

	// �������s
	if (!pNew)
	{
		assert(false && "�����̐����Ɏ��s");
	}

	// �����̏����ݒ�
	pNew->Init();

	// ��b�p�����[�^���R�s�[
	auto const& CorrectionCoef = n_InitParam["CorrectionCoef"];
	auto const& SizeTarget = n_InitParam["SizeTarget"];
	auto const& ColTarget = n_InitParam["ColTarget"];
	auto const& MaxDuration = n_InitParam["MaxDuration"];

	// �e��p�����[�^�ݒ�
	CTexture_Manager::TYPE Type = CTexture_Manager::TYPE::CONSTELLATION0;
	pNew->BindTex(Type + rand() % 4);													// �e�N�X�`��
	pNew->SetCorrectionCoef(static_cast<float>(CorrectionCoef));						// �␳�W��
	pNew->SetSizeTarget(Vec3(SizeTarget[0], SizeTarget[1], SizeTarget[2]));				// �ڕW�T�C�Y
	pNew->SetPos(Pos);																	// ���W
	pNew->SetPosTarget(Pos);															// �ڕW���W
	pNew->SetColTarget(XCol(ColTarget[0], ColTarget[1], ColTarget[2], ColTarget[3]));	// �ڕW�F
	pNew->SetMaxDuration(static_cast<int>(MaxDuration));								// �ő����
}