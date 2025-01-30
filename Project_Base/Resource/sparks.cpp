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

namespace {
	float n_fCoef = 0.0f;
	int n_nDuration = 0;
	float n_fWaveCoef = 0.0f;
	float n_fAdderY = 0.0f;
}

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************

// ��b�p�����[�^�̓W�J
JSON CSparks::m_InitParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\SPARKS\\sparks_param.json");

// �����J�E���g
int CSparks::m_nCntGenerate = 0;

// ���������X�p��
const int CSparks::m_nAutoGenerateSpan = static_cast<int>(CSparks::m_InitParam["AutoGenerateSpan"]);

// �Ǐ������X�p��
const int CSparks::m_nAreaGenerateSpan = static_cast<int>(CSparks::m_InitParam["AreaGenerateSpan"]);

// �Ҕ����X�p��
const int CSparks::m_nFuryGenerateSpan = static_cast<int>(CSparks::m_InitParam["FuryGenerateSpan"]);

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
	// �����J�E���g���C���N�������g
	++m_nCntGenerate;

	// �K��̐����X�p���ɓ��B��
	if (m_nCntGenerate > m_nAutoGenerateSpan)
	{
		// �����J�E���g�����Z�b�g
		m_nCntGenerate = 0;

		// ��������
		AutoCreate();
	}
}

//============================================================================
// �Ǐ�����
//============================================================================
void CSparks::AreaGenerate(D3DXVECTOR3 Pos)
{
	// �����J�E���g���C���N�������g
	++m_nCntGenerate;

	// �K��̐����X�p���ɓ��B��
	if (m_nCntGenerate > m_nAreaGenerateSpan)
	{
		// �����J�E���g�����Z�b�g
		m_nCntGenerate = 0;

		// �Ǐ�����
		AreaCreate(Pos);
	}
}

//============================================================================
// �W������
//============================================================================
void CSparks::PointGenerate(D3DXVECTOR3 Pos)
{
	PointCreate(Pos);
}

//============================================================================
// ��ʔ���
//============================================================================
void CSparks::ScreenGenerate(D3DXVECTOR3 Pos)
{
	ScreenCreate(Pos);
}

//============================================================================
// �Ҕ���
//============================================================================
void CSparks::FuryGenerate()
{
	// �����J�E���g���C���N�������g
	++m_nCntGenerate;

	// �K��̐����X�p���ɓ��B��
	if (m_nCntGenerate > m_nFuryGenerateSpan)
	{
		// �����J�E���g�����Z�b�g
		m_nCntGenerate = 0;

		// �Ґ���
		FuryCreate();
	}
}

#ifdef _DEBUG
//============================================================================
#include "flyer.h"
//============================================================================
void CSparks::EditInitParam()
{
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("aaabbbccc")) {
		ImGui::InputFloat("Coef", &n_fCoef);
		ImGui::InputInt("Duration", &n_nDuration);
		ImGui::InputFloat("Wave", &n_fWaveCoef);
		ImGui::InputFloat("Adder", &n_fAdderY);
		if (ImGui::Button("Add Flyer")) { auto p = CFlyer::Create(); p->SetDirection(-D3DX_PI * 0.5f); p->AdjustInitDirection(50.0f); }
		ImGui::End();
	}
}
#endif // _DEBUG

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// ��������
//============================================================================
void CSparks::AutoCreate()
{
	CSparks* pNewInstance = DBG_NEW CSparks();

	// �������s
	if (!pNewInstance)
	{
		assert(false && "�΂̕��̐����Ɏ��s");
	}

	// �΂̕��̏����ݒ�
	pNewInstance->Init();

	// �p�����[�^���R�s�[
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = m_InitParam["CorrectionCoef"];
	auto const& SizeTarget = m_InitParam["SizeTarget"];
	auto const& ColTarget = m_InitParam["ColTarget"];
	auto const& MaxDuration = m_InitParam["MaxDuration"];
	auto const& WaveCoef = m_InitParam["WaveCoef"];
	auto const& AdderY = m_InitParam["AdderY"];

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

//============================================================================
// �Ǐ�����
//============================================================================
void CSparks::AreaCreate(D3DXVECTOR3 Pos)
{
	// �΂̕��𐶐�
	CSparks* pNewInstance = DBG_NEW CSparks();

	// �������s
	if (!pNewInstance)
	{
		assert(false && "�΂̕��̐����Ɏ��s");
	}

	// �΂̕��̏����ݒ�
	pNewInstance->Init();

	// �p�����[�^���R�s�[
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = m_InitParam["CorrectionCoef"];
	auto const& SizeTarget = m_InitParam["SizeTarget"];
	auto const& ColTarget = m_InitParam["ColTarget"];
	auto const& MaxDuration = m_InitParam["MaxDuration"];
	auto const& WaveCoef = m_InitParam["WaveCoef"];
	auto const& AdderY = m_InitParam["AdderY"];

	// �w�肳�ꂽ�ꏊ����Ƀ����_���ȍ��W���쐬
	pNewInstance->m_InitPos = {
		Pos.x + utility::GetRandomValue<float>(),
		Pos.y + fabsf(utility::GetRandomValue<float>()),
		Pos.z + utility::GetRandomValue<float>()
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

//============================================================================
// �W������
//============================================================================
void CSparks::PointCreate(D3DXVECTOR3 Pos)
{
	// �΂̕��𐶐�
	CSparks* pNewInstance = DBG_NEW CSparks();

	// �������s
	if (!pNewInstance)
	{
		assert(false && "�΂̕��̐����Ɏ��s");
	}

	// �΂̕��̏����ݒ�
	pNewInstance->Init();

	// �p�����[�^���R�s�[
#if 1
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = m_InitParam["PointCorrectionCoef"];
	auto const& SizeTarget = m_InitParam["PointSizeTarget"];
	auto const& ColTarget = m_InitParam["PointColTarget"];
	auto const& MaxDuration = m_InitParam["PointMaxDuration"];
	auto const& WaveCoef = m_InitParam["PointWaveCoef"];
	auto const& AdderY = m_InitParam["PointAdderY"];
#else
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = n_fCoef;
	auto const& SizeTarget = m_InitParam["PointSizeTarget"];
	auto const& ColTarget = m_InitParam["PointColTarget"];
	auto const& MaxDuration = n_nDuration;
	auto const& WaveCoef = n_fWaveCoef;
	auto const& AdderY = n_fAdderY;
#endif

	// ��s���ăL���X�g
	float fCoef = static_cast<float>(CorrectionCoef);

	// �w�肳�ꂽ�ꏊ����Ƀ����_���ȍ��W���쐬
	pNewInstance->m_InitPos = {
		Pos.x + utility::GetRandomValue<float>() * fCoef,
		Pos.y + fabsf(utility::GetRandomValue<float>()) * fCoef,
		Pos.z + utility::GetRandomValue<float>() * fCoef
	};

	// �e��p�����[�^�ݒ�
	pNewInstance->BindTex(static_cast<CTexture_Manager::TYPE>(TextureType));					// �e�N�X�`��
	pNewInstance->SetCorrectionCoef(static_cast<float>(fCoef));									// �␳�W��
	pNewInstance->SetSizeTarget(Vec3(SizeTarget[0], SizeTarget[1], SizeTarget[2]));				// �ڕW�T�C�Y
	pNewInstance->SetPos(pNewInstance->m_InitPos);												// ���W
	pNewInstance->SetPosTarget(pNewInstance->m_InitPos);										// �ڕW���W
	pNewInstance->SetColTarget(XCol(ColTarget[0], ColTarget[1], ColTarget[2], ColTarget[3]));	// �ڕW�F
	pNewInstance->SetMaxDuration(static_cast<int>(MaxDuration));								// �ő����
	pNewInstance->m_fWaveCoef = static_cast<float>(WaveCoef);									// �g�ł����x
	pNewInstance->m_fAdderY = static_cast<float>(AdderY);										// �㏸��
}

//============================================================================
// ��ʐ���
//============================================================================
void CSparks::ScreenCreate(D3DXVECTOR3 Pos)
{

}

//============================================================================
// �Ґ���
//============================================================================
void CSparks::FuryCreate()
{
	CSparks* pNewInstance = DBG_NEW CSparks();

	// �������s
	if (!pNewInstance)
	{
		assert(false && "�΂̕��̐����Ɏ��s");
	}

	// �΂̕��̏����ݒ�
	pNewInstance->Init();

	// �p�����[�^���R�s�[
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = m_InitParam["CorrectionCoef"];
	auto const& SizeTarget = m_InitParam["FurySizeTarget"];
	auto const& ColTarget = m_InitParam["FuryColTarget"];
	auto const& MaxDuration = m_InitParam["FuryMaxDuration"];
	auto const& WaveCoef = m_InitParam["FuryWaveCoef"];
	auto const& AdderY = m_InitParam["FuryAdderY"];

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