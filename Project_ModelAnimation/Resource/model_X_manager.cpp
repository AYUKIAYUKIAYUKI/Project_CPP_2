//============================================================================
// 
// X���f���}�l�[�W���[ [model_X_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "model_X_manager.h"
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
	m_pXModelManager->Init();

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
	if (m_apModelTemp[static_cast<int>(Type)].pMesh == nullptr)
	{ // ���f���擾�s�\

#ifdef _DEBUG	// �x���\��

		CRenderer::SetTimeString("�y�x���z���f���擾�G���[", 600);

#endif	// _DEBUG
	}

	return &m_apModelTemp[static_cast<int>(Type)];
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
	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		// ���f�����̏�����
		m_apModelTemp[i].Size = { 0.0f, 0.0f, 0.0f };	// �T�C�Y
		m_apModelTemp[i].pMesh = nullptr;				// ���b�V���̃|�C���^
		m_apModelTemp[i].pBuffMat = nullptr;			// �}�e���A���o�b�t�@�̃|�C���^
		m_apModelTemp[i].dwNumMat = 0;					// �}�e���A����
		m_apModelTemp[i].apTex = nullptr;				// �e�N�X�`���̃|�C���^
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
	// ���f�����X�g��W�J
	std::ifstream ModelList("Data\\TXT\\model_path.txt");

	// �W�J�Ɏ��s
	if (!ModelList)
	{
		return E_FAIL;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	for (int nCntModel = 0; nCntModel < static_cast<int>(TYPE::MAX); nCntModel++)
	{
		// ���f�����i�[��
		std::string ModelName;

		// ���f�������擾����
		std::getline(ModelList, ModelName);

		// �p�X���쐬����
		ModelName.insert(0, "Data\\MODEL\\");

		// ���f���t�@�C���̎擾
		HRESULT hr = D3DXLoadMeshFromX(ModelName.c_str(),
			D3DXMESH_SYSTEMMEM,
			pDev,
			nullptr,
			&m_apModelTemp[nCntModel].pBuffMat,
			nullptr,
			&m_apModelTemp[nCntModel].dwNumMat,
			&m_apModelTemp[nCntModel].pMesh);

		if (FAILED(hr))
		{ // �擾���s

#ifdef _DEBUG	// �x���\��

			ModelName = ModelName.substr(ModelName.find_last_of("\\") + 1, ModelName.back());
			CRenderer::SetTimeString("�y�x���z���f��[" + ModelName + "]�͓ǂݍ��݂Ɏ��s���܂���", 300);

#endif	// _DEBUG

			continue;
		}

		// ���f���̃T�C�Y���擾����
		m_apModelTemp[nCntModel].Size = LoadSize(ModelName);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_apModelTemp[nCntModel].pBuffMat->GetBufferPointer();

		// �}�e���A���̐����̃e�N�X�`���|�C���^���m��
		m_apModelTemp[nCntModel].apTex = DBG_NEW LPDIRECT3DTEXTURE9[static_cast<int>(m_apModelTemp[nCntModel].dwNumMat)];

		// �}�e���A�����e�N�X�`���̗L�����m�F
		for (int nCntMat = 0; nCntMat < static_cast<int>(m_apModelTemp[nCntModel].dwNumMat); nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != nullptr)
			{
				// �e�N�X�`����ǂݎ�ꂽ�琶��
				hr = D3DXCreateTextureFromFileA(pDev,
					pMat[nCntMat].pTextureFilename,
					&m_apModelTemp[nCntModel].apTex[nCntMat]);

				if (FAILED(hr))
				{ // �������s

					// �e�N�X�`�������R�s�[
					std::string TextureName{ pMat[nCntMat].pTextureFilename };

#ifdef _DEBUG	// �x���\��

					ModelName = ModelName.substr(ModelName.find_last_of("\\") + 1, ModelName.back());
					CRenderer::SetTimeString("�y�x���z���f��[" + ModelName + "]�ɂ�����e�N�X�`���p�X��[" + TextureName + "]�͐����Ɏ��s���܂���", 600);

#endif	// _DEBUG

					m_apModelTemp[nCntModel].apTex[nCntMat] = nullptr;
				}
			}
			else
			{
				// �ǂݎ��Ȃ���Ώ�����
				m_apModelTemp[nCntModel].apTex[nCntMat] = nullptr;
			}
		}
	}

	// �t�@�C�������
	ModelList.close();

	return S_OK;
}

//============================================================================
// �T�C�Y�ǂݍ���
//============================================================================
D3DXVECTOR3 CX_Manager::LoadSize(std::string filename)
{
	// ��r�����p�ɐ��l�����Ă���
	D3DXVECTOR3 sizeMin = { FLT_MAX, FLT_MAX, FLT_MAX };
	D3DXVECTOR3 sizeMax = { FLT_MIN, FLT_MIN, FLT_MIN };

	// ���f���t�@�C�����̂��̂�W�J
	std::ifstream file(filename);

	if (!file)
	{ // �W�J�Ɏ��s

#ifdef _DEBUG	// �x���\��

		CRenderer::SetTimeString("�y�x���z���f��[" + filename + "]�̓T�C�Y�ǂݍ��݂Ɏ��s���܂���", 600);

#endif	// _DEBUG

		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f, };
	}

	// ������i�[�p
	std::string str{};

	// ���_���̉ӏ��܂ōs��ǂݍ���
	while (std::getline(file, str))
	{
		if (str == "Mesh {")
		{
			// �s�v�ȍs����x�ǂݍ���
			std::getline(file, str);

			break;
		}
	}

	// �z��̗v�f��
	const int nNumArray = 3;

	// ���l���r���Ă���
	while (std::getline(file, str))
	{
		// �I������
		if (str == " ")
		{
			break;
		}

		// ���l�p
		float fNum[nNumArray]{};

		// �ǂ݂Ƃ������l����U�i�[����
		for (int i = 0; i < nNumArray; ++i)
		{
			fNum[i] = std::stof(str);
			str = str.substr(str.find(";") + 1, str.back());
		}

		// �e���̍ő�E�ŏ��n�_���X�V
		if (sizeMin.x > fNum[0])
		{
			sizeMin.x = fNum[0];
		}

		if (sizeMin.y > fNum[1])
		{
			sizeMin.y = fNum[1];
		}

		if (sizeMin.z > fNum[2])
		{
			sizeMin.z = fNum[2];
		}

		if (sizeMax.x < fNum[0])
		{
			sizeMax.x = fNum[0];
		}

		if (sizeMax.y < fNum[1])
		{
			sizeMax.y = fNum[1];
		}

		if (sizeMax.z < fNum[2])
		{
			sizeMax.z = fNum[2];
		}
	}

	// �t�@�C�������
	file.close();

	return sizeMax;
}

//============================================================================
// �I������
//============================================================================
void CX_Manager::Uninit()
{
	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		// �e�N�X�`���|�C���^�̔j��
		if (m_apModelTemp[i].apTex != nullptr)
		{
			// �e�N�X�`���̔j��
			for (int nCntMat = 0; nCntMat < static_cast<int>(m_apModelTemp[i].dwNumMat); nCntMat++)
			{
				if (m_apModelTemp[i].apTex[nCntMat] != nullptr)
				{
					m_apModelTemp[i].apTex[nCntMat]->Release();
					m_apModelTemp[i].apTex[nCntMat] = nullptr;
				}
			}

			delete[] m_apModelTemp[i].apTex;
			m_apModelTemp[i].apTex = nullptr;
		}

		// ���b�V���̔j��
		if (m_apModelTemp[i].pMesh != nullptr)
		{
			m_apModelTemp[i].pMesh->Release();
			m_apModelTemp[i].pMesh = nullptr;
		}

		// �}�e���A���̔j��
		if (m_apModelTemp[i].pBuffMat != nullptr)
		{
			m_apModelTemp[i].pBuffMat->Release();
			m_apModelTemp[i].pBuffMat = nullptr;
		}
	}
}