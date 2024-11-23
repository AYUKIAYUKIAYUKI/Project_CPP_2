//============================================================================
// 
// X���f���}�l�[�W���[ [model_X_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "X_manager.h"
#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�̏�����
//****************************************************
CX_Manager* CX_Manager::m_pXModelManager = nullptr;	// X���f���}�l�[�W���[�̖{��

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
HRESULT CX_Manager::Create()
{
	// ���ɐ�������Ă�����
	if (m_pXModelManager != nullptr)
	{ 
#ifdef _DEBUG
		CRenderer::SetTimeString("X���f���}�l�[�W���[�͊��ɐ�������Ă��܂�", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// �C���X�^���X�𐶐�
	m_pXModelManager = DBG_NEW CX_Manager();

	// �������s
	if (m_pXModelManager == nullptr)
	{
		return E_FAIL;
	}

	// X���f���}�l�[�W���[�̏����ݒ�
	if (FAILED(m_pXModelManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CX_Manager::Release()
{
	if (m_pXModelManager != nullptr)
	{
		// X���f���}�l�[�W���[�̏I������
		m_pXModelManager->Uninit();

		// �������̉��
		delete m_pXModelManager;

		// �|�C���^�̏�����
		m_pXModelManager = nullptr;
	}
}

//============================================================================
// ���f�����擾
//============================================================================
CX_Manager::MODEL* CX_Manager::GetModel(TYPE Type)
{
	// ���f���擾�s�\
	if (m_apModelTemp[static_cast<WORD>(Type)].pMesh == nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("�y�x���z���f���擾���ɃG���[����", 300);
#endif	// _DEBUG

		return nullptr;
	}

	return &m_apModelTemp[static_cast<WORD>(Type)];
}

//============================================================================
// X���f���}�l�[�W���[���擾
//============================================================================
CX_Manager* CX_Manager::GetInstance()
{
	// �{�̂����݂��Ȃ����
	if (m_pXModelManager == nullptr)
	{
		// ����
		if (FAILED(Create()))
		{
			assert(false && "X���f���}�l�[�W���[�̎擾�Ɏ��s");
		}
	}

	return m_pXModelManager;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CX_Manager::CX_Manager()
{
	for (WORD wCntModel = 0; wCntModel < static_cast<WORD>(TYPE::MAX); ++wCntModel)
	{
		// ���f�����̏�����
		m_apModelTemp[wCntModel].pMesh = nullptr;		// ���b�V���̃|�C���^
		m_apModelTemp[wCntModel].Size = { VEC3_INIT };	// �T�C�Y
		m_apModelTemp[wCntModel].pBuffMat = nullptr;	// �}�e���A���̃|�C���^
		m_apModelTemp[wCntModel].dwNumMat = 0;			// �}�e���A����
		m_apModelTemp[wCntModel].apColMat = nullptr;	// �}�e���A���̐F�̃|�C���^
		m_apModelTemp[wCntModel].apTex = nullptr;		// �e�N�X�`���̃|�C���^
	}
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CX_Manager::~CX_Manager()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CX_Manager::Init()
{
	// ���f�����X�g���擾
	JSON Json = utility::OpenJsonFile("Data\\JSON\\model_list.json");

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	for (WORD wCntModel = 0; wCntModel < static_cast<WORD>(TYPE::MAX); ++wCntModel)
	{
		// ���f���t�@�C���̃p�X���쐬����
		const std::string& ModelFilePath = Json["ModelList"][wCntModel];

		// ���f���t�@�C���̎擾
		HRESULT hr = D3DXLoadMeshFromX(ModelFilePath.c_str(),
			D3DXMESH_SYSTEMMEM,
			pDev,
			nullptr,
			&m_apModelTemp[wCntModel].pBuffMat,
			nullptr,
			&m_apModelTemp[wCntModel].dwNumMat,
			&m_apModelTemp[wCntModel].pMesh);

		if (FAILED(hr))
		{ // �擾���s
#ifdef _DEBUG
			CRenderer::SetTimeString("�y�x���z���f��[" + ModelFilePath + "]�͓ǂݍ��݂Ɏ��s���܂���", 300);
#endif	// _DEBUG

			continue;
		}

		// ���f���̃T�C�Y���擾����
		m_apModelTemp[wCntModel].Size = LoadSize(ModelFilePath);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_apModelTemp[wCntModel].pBuffMat->GetBufferPointer();

		// �}�e���A�������A�}�e���A���F���̃|�C���^�z����m��
		m_apModelTemp[wCntModel].apColMat = DBG_NEW D3DXCOLOR[m_apModelTemp[wCntModel].dwNumMat];

		// �}�e���A�������A�e�N�X�`�����̃|�C���^�z����m��
		m_apModelTemp[wCntModel].apTex = DBG_NEW LPDIRECT3DTEXTURE9[m_apModelTemp[wCntModel].dwNumMat];

		// �}�e���A�����ƂɃe�N�X�`���̗L�����m�F
		for (WORD wCntMat = 0; wCntMat < m_apModelTemp[wCntModel].dwNumMat; ++wCntMat)
		{
			// �}�e���A���̐F�����f�������ɕۑ����Ă���
			m_apModelTemp[wCntModel].apColMat[wCntMat] = pMat[wCntMat].MatD3D.Diffuse;

			// �e�N�X�`�������擾���s
			if (pMat[wCntMat].pTextureFilename == nullptr)
			{
				// �e�N�X�`���̃|�C���^�����������Ă���
				m_apModelTemp[wCntModel].apTex[wCntMat] = nullptr;
			
				continue;
			}

			// �e�N�X�`������ǂݎ�ꂽ�琶��
			hr = D3DXCreateTextureFromFileA(pDev,
				pMat[wCntMat].pTextureFilename,
				&m_apModelTemp[wCntModel].apTex[wCntMat]);

			if (FAILED(hr))
			{ // �������s
#ifdef _DEBUG
				CRenderer::SetTimeString("�y�x���z���f��[" + ModelFilePath + "]�ɂ�����e�N�X�`����[" + pMat[wCntMat].pTextureFilename + "]�͐����Ɏ��s���܂���", 600);
#endif	// _DEBUG

				// �e�N�X�`���̃|�C���^�����������Ă���
				m_apModelTemp[wCntModel].apTex[wCntMat] = nullptr;
			}
		}
	}

	return S_OK;
}

//============================================================================
// �T�C�Y�ǂݍ���
//============================================================================
D3DXVECTOR3 CX_Manager::LoadSize(std::string filename)
{
	// �Y�����f���t�@�C����W�J
	std::ifstream ModelFile(filename);

	// �W�J�Ɏ��s
	if (!ModelFile.good())
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("�y�x���z���f��[" + filename + "]�̓T�C�Y�ǂݍ��݂Ɏ��s���܂���", 600);
#endif	// _DEBUG

		return VEC3_INIT;
	}

	// �ėp������i�[�p
	std::string Str;

	// �e�L�X�g���璸�_���̍s������
	while (std::getline(ModelFile, Str))
	{
		// ���_���𔭌������̂Ō����I��
		if (Str == "Mesh {")
		{
			// �s�v�ȍs����x�ǂݍ���
			std::getline(ModelFile, Str);

			break;
		}
	}

	// ��r�����p�ɐ��l�����Ă���
	Vec3 ResultMin = { FLT_MAX, FLT_MAX, FLT_MAX };
	Vec3 ResultMax = { FLT_MIN, FLT_MIN, FLT_MIN };

	// �擾�\��̎�������
	const WORD wAxis = 3;

	// ���l���r���Ă���
	while (std::getline(ModelFile, Str))
	{
		// �I������
		if (Str == " ")
		{
			break;
		}

		// ���l�i�[�p
		float fNum[wAxis] = { 0.0f, 0.0f, 0.0f };

		// �ǂ݂Ƃ������l����U�i�[����
		for (WORD wCntAxis = 0; wCntAxis < wAxis; ++wCntAxis)
		{
			fNum[wCntAxis] = std::stof(Str);
			Str = Str.substr(Str.find(";") + 1, Str.back());
		}

		// �e���̍ő�E�ŏ��n�_���X�V
		if (ResultMin.x > fNum[0])
		{
			ResultMin.x = fNum[0];
		}

		if (ResultMin.y > fNum[1])
		{
			ResultMin.y = fNum[1];
		}

		if (ResultMin.z > fNum[2])
		{
			ResultMin.z = fNum[2];
		}

		if (ResultMax.x < fNum[0])
		{
			ResultMax.x = fNum[0];
		}

		if (ResultMax.y < fNum[1])
		{
			ResultMax.y = fNum[1];
		}

		if (ResultMax.z < fNum[2])
		{
			ResultMax.z = fNum[2];
		}
	}

	// �t�@�C�������
	ModelFile.close();

	return ResultMax;
}

//============================================================================
// �I������
//============================================================================
void CX_Manager::Uninit()
{
	for (WORD wCntModel = 0; wCntModel < static_cast<WORD>(TYPE::MAX); ++wCntModel)
	{
		// �}�e���A���p�̃e�N�X�`����j��
		for (WORD wCntMat = 0; wCntMat < m_apModelTemp[wCntModel].dwNumMat; ++wCntMat)
		{
			if (m_apModelTemp[wCntModel].apTex[wCntMat] != nullptr)
			{
				m_apModelTemp[wCntModel].apTex[wCntMat]->Release();
				m_apModelTemp[wCntModel].apTex[wCntMat] = nullptr;
			}
		}

		// �e�N�X�`���|�C���^�z��̔j��
		if (m_apModelTemp[wCntModel].apTex != nullptr)
		{
			delete[] m_apModelTemp[wCntModel].apTex;
			m_apModelTemp[wCntModel].apTex = nullptr;
		}

		// �}�e���A���F�|�C���^�z��̔j��
		if (m_apModelTemp[wCntModel].apColMat != nullptr)
		{
			delete[] m_apModelTemp[wCntModel].apColMat;
			m_apModelTemp[wCntModel].apColMat = nullptr;
		}

		// �}�e���A���o�b�t�@�̔j��
		if (m_apModelTemp[wCntModel].pBuffMat != nullptr)
		{
			m_apModelTemp[wCntModel].pBuffMat->Release();
			m_apModelTemp[wCntModel].pBuffMat = nullptr;
		}

		// ���b�V���o�b�t�@�̔j��
		if (m_apModelTemp[wCntModel].pMesh != nullptr)
		{
			m_apModelTemp[wCntModel].pMesh->Release();
			m_apModelTemp[wCntModel].pMesh = nullptr;
		}
	}
}