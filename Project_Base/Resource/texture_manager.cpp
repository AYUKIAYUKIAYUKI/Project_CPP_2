//============================================================================
// 
// �e�N�X�`���}�l�[�W���[ [texture_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "texture_manager.h"
#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CTexture_Manager* CTexture_Manager::m_pTextureManager = nullptr;	// �e�N�X�`���}�l�[�W���[�̖{��

//============================================================================
//
// public�����o
//
//============================================================================

//============================================================================
// ����
//============================================================================
HRESULT CTexture_Manager::Create()
{
	// ���ɐ�������Ă�����
	if (m_pTextureManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("�e�N�X�`���}�l�[�W���[�͊��ɐ�������Ă��܂�", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// �C���X�^���X�𐶐�
	m_pTextureManager = DBG_NEW CTexture_Manager();

	// �������s
	if (m_pTextureManager == nullptr)
	{
		return E_FAIL;
	}

	// �e�N�X�`���}�l�[�W���[�̏����ݒ�
	if (FAILED(m_pTextureManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CTexture_Manager::Release()
{
	if (m_pTextureManager != nullptr)
	{
		// �e�N�X�`���}�l�[�W���[�̏I������
		m_pTextureManager->Uninit();

		// �������̉��
		delete m_pTextureManager;

		// �|�C���^�̏�����
		m_pTextureManager = nullptr;
	}
}

//============================================================================
// �e�N�X�`�����擾
//============================================================================
LPDIRECT3DTEXTURE9 CTexture_Manager::GetTexture(TYPE Type)
{
	// ���f���擾�s�\
	if (m_apTexture[static_cast<WORD>(Type)] == nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("�y�x���z�e�N�X�`���擾���ɃG���[����", 300);
#endif	// _DEBUG

		return nullptr;
	}

	return m_apTexture[static_cast<WORD>(Type)];
}

//============================================================================
// �e�N�X�`���}�l�[�W���[���擾
//============================================================================
CTexture_Manager* CTexture_Manager::GetInstance()
{
	// �{�̂����݂��Ȃ����
	if (m_pTextureManager == nullptr)
	{
		// ����
		if (FAILED(Create()))
		{
			assert(false && "�e�N�X�`���}�l�[�W���[�̎擾�Ɏ��s");
		}
	}

	return m_pTextureManager;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CTexture_Manager::CTexture_Manager()
{
	for (WORD wCntTex = 0; wCntTex < static_cast<int>(TYPE::MAX); ++wCntTex)
	{
		m_apTexture[wCntTex] = nullptr;	// �e�N�X�`�����̏�����
	}
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTexture_Manager::~CTexture_Manager()
{

}

//============================================================================
// �I������
//============================================================================
void CTexture_Manager::Uninit()
{
	for (WORD wCntTex = 0; wCntTex < static_cast<int>(TYPE::MAX); ++wCntTex)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[wCntTex] != nullptr)
		{
			m_apTexture[wCntTex]->Release();
			m_apTexture[wCntTex] = nullptr;
		}
	}
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CTexture_Manager::Init()
{
	// �e�N�X�`�����X�g���擾
	JSON Json = utility::OpenJsonFile("Data\\JSON\\texture_list.json");

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();

	for (WORD wCntTex = 0; wCntTex < static_cast<int>(TYPE::MAX); ++wCntTex)
	{
		// �e�N�X�`���t�@�C���̃p�X���쐬����
		const std::string& TextureFilePath = Json["ModelList"][wCntTex];

		// �e�N�X�`���̐���
		HRESULT hr = D3DXCreateTextureFromFileA(pDev,
			TextureFilePath.c_str(),
			&m_apTexture[wCntTex]);

		if (FAILED(hr))
		{ // �e�N�X�`���������s
#ifdef _DEBUG
			CRenderer::SetTimeString("�y�x���z�e�N�X�`��[" + TextureFilePath + "]�͐����Ɏ��s���܂���", 300);
#endif	// _DEBUG
		}
	}

	return S_OK;
}