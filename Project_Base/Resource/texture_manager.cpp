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

// �f�o�C�X�擾�p
#include "renderer.h"

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CTexture_Manager* CTexture_Manager::m_pInstance = nullptr;	// �e�N�X�`���}�l�[�W���[

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CTexture_Manager::Load()
{
	// �e�N�X�`�����X�g��W�J
	std::ifstream TextureList{ "Data\\TXT\\texture_path.txt" };

	if (!TextureList)
	{ // �W�J�Ɏ��s
		assert(false);
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev{ CRenderer::GetInstance()->GetDeviece() };

	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		// �e�N�X�`�����i�[��
		std::string TextureName;

		// �e�N�X�`�������擾����
		std::getline(TextureList, TextureName);

		// �p�X���쐬����
		TextureName.insert(0, "Data\\TEXTURE\\");

		// �e�N�X�`���̐���
		HRESULT hr = D3DXCreateTextureFromFileA(pDev,
			TextureName.c_str(),
			&m_apTexTemp[i]);

		if (FAILED(hr))
		{ // �e�N�X�`���������s
#ifdef _DEBUG
			TextureName = TextureName.substr(TextureName.find_last_of("\\") + 1, TextureName.back());
			CRenderer::GetInstance()->SetTimeString("�y�x���z�e�N�X�`��[" + TextureName + "]�͐����Ɏ��s���܂���", 300);
#endif	// _DEBUG
		}
	}

	// �t�@�C�������
	TextureList.close();

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CTexture_Manager::Release()
{
	if (m_pInstance != nullptr)
	{
		// �e�N�X�`����j��
		m_pInstance->Unload();

		// �����������
		delete m_pInstance;
	
		// �|�C���^��������
		m_pInstance = nullptr;
	}
}

//============================================================================
// �e�N�X�`�����擾
//============================================================================
LPDIRECT3DTEXTURE9 CTexture_Manager::GetTexture(TYPE Type)
{
	if (m_apTexTemp[static_cast<int>(Type)] == nullptr)
	{ // �e�N�X�`���擾�s�\
#ifdef _DEBUG
		CRenderer::GetInstance()->SetTimeString("�y�x���z�e�N�X�`���擾�G���[", 600);
#endif	// _DEBUG
	}

	return m_apTexTemp[static_cast<int>(Type)];
}

//============================================================================
// �e�N�X�`���}�l�[�W���[���擾
//============================================================================
CTexture_Manager* CTexture_Manager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		// ����
		m_pInstance->Create();
	}

	return m_pInstance;
}

//============================================================================
// �R���X�g���N�^
//============================================================================
CTexture_Manager::CTexture_Manager()
{
	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		m_apTexTemp[i] = nullptr;	// �e�N�X�`�����̏�����
	}
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTexture_Manager::~CTexture_Manager()
{

}

//============================================================================
// ����
//============================================================================
void CTexture_Manager::Create()
{
	if (m_pInstance != nullptr)
	{ // ��d�����֎~
		assert(false);
	}

	// �C���X�^���X�𐶐�
	m_pInstance = DBG_NEW CTexture_Manager{};
}

//============================================================================
// �I������
//============================================================================
void CTexture_Manager::Unload()
{
	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexTemp[i] != nullptr)
		{
			m_apTexTemp[i]->Release();
			m_apTexTemp[i] = nullptr;
		}
	}
}