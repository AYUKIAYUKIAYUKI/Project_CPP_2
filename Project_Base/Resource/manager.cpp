//============================================================================
// 
// �}�l�[�W���[ [manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "manager.h"

// �V���O���g���Ǘ��p
#include "fade.h"
#include "renderer.h"
#include "sound.h"

#include "mask_rectangle.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace camera;

//****************************************************
// �ÓI�����o�̏�����
//****************************************************
CMask_Rectangle* CManager::m_pMask_Rectangle= nullptr;
CCamera* CManager::m_pCamera = nullptr;				// �J�����Ǘ�
CLight* CManager::m_pLight = nullptr;				// ���C�g�Ǘ�
CInputKeyboard* CManager::m_pKeyboard = nullptr;	// �L�[�{�[�h�Ǘ�
CInputPad* CManager::m_pPad = nullptr;				// �p�b�h�Ǘ�
CScene* CManager::m_pScene = nullptr;				// �V�[���Ǘ�

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CManager::CManager()
{
	// �O�̂��ߐÓI�����o�̏�����
	m_pCamera = nullptr;
	m_pLight = nullptr;
	m_pKeyboard = nullptr;
	m_pPad = nullptr;
	m_pScene = nullptr;
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CManager::~CManager()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �����_���[�̐���
	if (FAILED(CRenderer::GetInstance()->Init(hWnd, bWindow)))
	{
		return E_FAIL;
	}

	// �l�p�`�}�X�N�̐���
	m_pMask_Rectangle = CMask_Rectangle::Create(CTexture_Manager::TYPE::MAP);

	// �t�F�[�h�̏����ݒ�
	if (FAILED(CFade::GetInstance()->Init()))
	{
		return E_FAIL;
	}

	// �T�E���h�����ݒ�
	if (FAILED(CSound::GetInstance()->Init(hWnd)))
	{
		return E_FAIL;
	}

	// �J�����̐���
	m_pCamera = DBG_NEW CCamera;

	if (m_pCamera == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �J�����̏�����
	m_pCamera->Init();

	// ���C�g�̐���
	m_pLight = DBG_NEW CLight;

	if (m_pLight == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// ���C�g�̏�����
	m_pLight->Init();

	// �L�[�{�[�h�̐���
	m_pKeyboard = DBG_NEW CInputKeyboard;

	if (m_pKeyboard == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �L�[�{�[�h�̏�����
	m_pKeyboard->Init(hInstance, hWnd);

	// �p�b�h�̐���
	m_pPad = DBG_NEW CInputPad;

	if (m_pPad == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �p�b�h�̏�����
	m_pPad->Init();

	// �ŏ��̃V�[���ݒ�
	SetScene(CScene::MODE::TITLE);

	// BGM��������
	CSound::GetInstance()->Play(CSound::LABEL::TEST);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CManager::Uninit()
{
	// �V�[���̔j��
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();	// �I������
		delete m_pScene;	// �����������
		m_pScene = nullptr;	// �|�C���^��������
	}

	// �p�b�h�̔j��
	if (m_pPad != nullptr)
	{
		m_pPad->Uninit();	// �I������
		delete m_pPad;		// �����������
		m_pPad = nullptr;	// �|�C���^��������
	}

	// �L�[�{�[�h�̔j��
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();	// �I������
		delete m_pKeyboard;		// �����������
		m_pKeyboard = nullptr;	// �|�C���^��������
	}

	// ���C�g�̔j��
	if (m_pLight != nullptr)
	{
		delete m_pLight;		// �����������
		m_pLight = nullptr;		// �|�C���^��������
	}

	// �J�����̔j��
	if (m_pCamera != nullptr)
	{
		delete m_pCamera;		// �����������
		m_pCamera = nullptr;	// �|�C���^��������
	}

	// �T�E���h�j��
	CSound::GetInstance()->Release();

	// �t�F�[�h�̔j��
	CFade::GetInstance()->Release();

	// �l�p�`�}�X�N�̔j��
	if (m_pMask_Rectangle != nullptr)
	{
		m_pMask_Rectangle->Uninit();	// �I������
		delete m_pMask_Rectangle;		// �����������
		m_pMask_Rectangle = nullptr;	// �|�C���^��������
	}

	// �����_���[�̔j��
	CRenderer::GetInstance()->Release();
}

//============================================================================
// �X�V����
//============================================================================
void CManager::Update()
{
	// �l�p�`�}�X�N�̍X�V����
	m_pMask_Rectangle->Update();

	// �����_���[�̍X�V
	CRenderer::GetInstance()->Update();

	// �V�[���̍X�V
	m_pScene->Update();

	// ���C�g�̍X�V
	m_pLight->Update();

	// �J�����̍X�V
	m_pCamera->Update();

	// �L�[�{�[�h�̍X�V
	m_pKeyboard->Update();

	// �p�b�h�̍X�V
	m_pPad->Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CManager::Draw()
{
	// �����_���[�̕`��
	CRenderer::GetInstance()->Draw();
}

//============================================================================
// �J�����擾
//============================================================================
CCamera* CManager::GetCamera()
{
	return m_pCamera;
}

//============================================================================
// ���C�g�擾
//============================================================================
CLight* CManager::GetLight()
{
	return m_pLight;
}

//============================================================================
// �L�[�{�[�h�擾
//============================================================================
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pKeyboard;
}

//============================================================================
// �p�b�h�擾
//============================================================================
CInputPad* CManager::GetPad()
{
	return m_pPad;
}

//============================================================================
// �V�[���擾
//============================================================================
CScene* CManager::GetScene()
{
	return m_pScene;
}

//============================================================================
// �V�[���ݒ�
//============================================================================
void CManager::SetScene(CScene::MODE mode)
{
	// ���݂̃V�[����j��
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	// �V���ȃV�[����ݒ�
	m_pScene = CScene::Create(mode);

	// �������s
	if (!m_pScene)
	{
		assert(false);
	}

	// �����ݒ�
	m_pScene->Init();
}