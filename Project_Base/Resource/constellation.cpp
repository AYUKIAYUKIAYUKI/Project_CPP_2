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

// ��b�p�����[�^�̓W�J
JSON CConstellation::m_InitParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\CONSTELLATION\\constellation_param.json");

// �����J�E���g
int CConstellation::m_nCntGenetrate = 0;

// �����X�p��
const int CConstellation::m_nGenerateSpan = static_cast<int>(CConstellation::m_InitParam["GenerateSpan"]);

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
	// �����J�E���g���C���N�������g
	++m_nCntGenetrate;

	// �K��̐����X�p���ɓ��B��
	if (m_nCntGenetrate > m_nGenerateSpan)
	{
		// �����J�E���g�����Z�b�g
		m_nCntGenetrate = 0;

		// �g�U�I�t�Z�b�g���R�s�[
		float fSpreadOffset = static_cast<float>(m_InitParam["Spreadoffset"]);

		// ����������
		for (WORD wCntCreate = 0; wCntCreate < 3; ++wCntCreate)
		{
			// �n���ꂽ���W�������_���ɂ��炷
			Pos += {
				utility::GetRandomValue<float>() * fSpreadOffset,
				utility::GetRandomValue<float>() * fSpreadOffset,
				utility::GetRandomValue<float>() * fSpreadOffset
			};

			// �����̐���
			Create(Pos);
		}
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
void CConstellation::Create(D3DXVECTOR3 Pos)
{
	CConstellation* pNewInstance = DBG_NEW CConstellation();

	// �������s
	if (!pNewInstance)
	{
		assert(false && "�����̐����Ɏ��s");
	}

	// �����̏����ݒ�
	pNewInstance->Init();

	{ // �p�����[�^�ݒ�

		// �f�[�^���L���X�g
		float
			CorrectCoef = m_InitParam["CorrectCoef"];
		Vec3
			SizeTarget = utility::JsonConvertToVec3(m_InitParam["SizeTarget"]);
		XCol
			ColTarget = utility::JsonConvertToXCol(m_InitParam["ColTarget"]);
		int
			nMaxDuration = static_cast<int>(m_InitParam["MaxDuration"]);

		// �f�[�^���Z�b�g
		pNewInstance->CObject_Effect::SetCorrectionCoef(CorrectCoef);	// ��ԋ��x
		pNewInstance->CObject_Effect::SetSizeTarget(SizeTarget);		// �ڕW�T�C�Y
		pNewInstance->CObject_Effect::SetPos(Pos);						// ���W
		pNewInstance->CObject_Effect::SetPosTarget(Pos);				// �ڕW���W
		pNewInstance->CObject_Effect::SetColTarget(ColTarget);			// �ڕW�F
		pNewInstance->CObject_Effect::SetMaxDuration(nMaxDuration);		// �ő����

		// �e�N�X�`����ݒ�
		CTexture_Manager::TYPE Type = CTexture_Manager::TYPE::CONSTELLATION0;

		// �����_���Ȑ����e�N�X�`���ɕ���
		pNewInstance->BindTex(Type + rand() % 4);
	}
}