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
#include "mask_rectangle.h"

// �V���O���g���N���X�p
#include "renderer.h"
#include "scene.h"
#include "sound.h"
#include "texture_manager.h"
#include "X_manager.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CManager* CManager::m_pManager = nullptr;	// �}�l�[�W���[�̖{��

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CManager::Update()
{
	// Gui�̍X�V���J�n
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �����_���[�̍X�V
	CRenderer::GetRenderer()->Update();

	// �l�p�`�}�X�N�̍X�V����
	m_pMask_Rectangle->Update();

	// ���C�g�̍X�V
	m_pLight->Update();

	// �J�����̍X�V
	m_pCamera->Update();

	// �V�[���̍X�V
	CScene_Manager::GetInstance()->GetScene()->Update();

	// �L�[�{�[�h�̍X�V
	m_pKeyboard->Update();

	// �}�E�X�̍X�V
	m_pMouse->Update();

	// �p�b�h�̍X�V
	m_pPad->Update();

	// Gui�̍X�V���I��
	ImGui::EndFrame();
}

//============================================================================
// �`�揈��
//============================================================================
void CManager::Draw()
{
	// �����_���[�̕`��
	CRenderer::GetRenderer()->Draw();
}

//============================================================================
// �l�p�`�}�X�N���擾
//============================================================================
CMask_Rectangle* CManager::GetMask_Rectangle() const
{
	return m_pMask_Rectangle;
}

//============================================================================
// �J�������擾
//============================================================================
CCamera* CManager::GetCamera() const
{
	return m_pCamera;
}

//============================================================================
// ���C�g���擾
//============================================================================
CLight* CManager::GetLight() const
{
	return m_pLight;
}

//============================================================================
// ����
//============================================================================
HRESULT CManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pManager != nullptr)
	{
		assert(false && "�}�l�[�W���[�͊��ɍ쐬����Ă��邩�A�_���O�����O���Ă��܂�");
	}

	// �}�l�[�W���[�𐶐�
	m_pManager = DBG_NEW CManager();

	if (m_pManager == nullptr)
	{
		assert(false && "�}�l�[�W���[�̐����Ɏ��s");
	}

	// �}�l�[�W���[�̏����ݒ�
	if (FAILED(m_pManager->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CManager::Release()
{
	// �}�l�[�W���[�̔j��
	if (m_pManager != nullptr)
	{
		// �}�l�[�W���[�̏I������
		m_pManager->Uninit();

		// �����������
		delete m_pManager;

		// �|�C���^��������
		m_pManager = nullptr;
	}
}

//============================================================================
// �}�l�[�W���[���擾
//============================================================================
CManager* CManager::GetManager()
{
	return m_pManager;
}

//============================================================================
// �L�[�{�[�h���擾
//============================================================================
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pManager->m_pKeyboard;
}

//============================================================================
// �}�E�X���擾
//============================================================================
CInputMouse* CManager::GetMouse()
{
	return m_pManager->m_pMouse;
}

//============================================================================
// �p�b�h���擾
//============================================================================
CInputPad* CManager::GetPad()
{
	return m_pManager->m_pPad;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CManager::CManager() :
	m_pKeyboard{ nullptr },
	m_pMouse{ nullptr },
	m_pPad{ nullptr },
	m_pMask_Rectangle{ nullptr },
	m_pCamera{ nullptr },
	m_pLight{ nullptr }
{

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
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �����_���[�̐���
	if (FAILED(CRenderer::Create(hWnd, TRUE)))
	{
		return E_FAIL;
	}

	// �J�����̐���
	m_pCamera = DBG_NEW CCamera();

	if (m_pCamera == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �J�����̏�����
	m_pCamera->Init();

	// �V�[���}�l�[�W���[�̐��� (�J����������)
	if (FAILED(CScene_Manager::Create()))
	{
		return E_FAIL;
	}

	// �T�E���h�����ݒ�
	if (FAILED(CSound::GetInstance()->Init(hWnd)))
	{
		return E_FAIL;
	}

	// �e�N�X�`���}�l�[�W���[�𐶐�
	if (FAILED(CTexture_Manager::Create()))
	{
		return E_FAIL;
	}

	// X���f���}�l�[�W���[�𐶐�
	if (FAILED(CX_Manager::Create()))
	{
		return E_FAIL;
	}

	// �l�p�`�}�X�N�̐���
	m_pMask_Rectangle = CMask_Rectangle::Create(CTexture_Manager::TYPE::MAP);

	// ���C�g�̐���
	m_pLight = DBG_NEW CLight();

	if (m_pLight == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// ���C�g�̏�����
	m_pLight->Init();

	// �L�[�{�[�h�̐���
	m_pKeyboard = DBG_NEW CInputKeyboard();

	if (m_pKeyboard == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �L�[�{�[�h�̏�����
	m_pKeyboard->Init(hInstance, hWnd);

	// �}�E�X�̐���
	m_pMouse = DBG_NEW CInputMouse();

	if (m_pMouse == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �}�E�X�̏�����
	m_pMouse->Init(hInstance, hWnd);

	// �p�b�h�̐���
	m_pPad = DBG_NEW CInputPad();

	if (m_pPad == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �p�b�h�̏�����
	m_pPad->Init(hInstance, hWnd);

	// BGM��������
	CSound::GetInstance()->Play(CSound::LABEL::TEST);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CManager::Uninit()
{
	// �p�b�h�̔j��
	if (m_pPad != nullptr)
	{
		m_pPad->Uninit();	// �I������
		delete m_pPad;		// �����������
		m_pPad = nullptr;	// �|�C���^��������
	}

	// �}�E�X�̔j��
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();	// �I������
		delete m_pMouse;	// �����������
		m_pMouse = nullptr;	// �|�C���^��������
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
	
	// �l�p�`�}�X�N�̔j��
	if (m_pMask_Rectangle != nullptr)
	{
		m_pMask_Rectangle->Uninit();	// �I������
		delete m_pMask_Rectangle;		// �����������
		m_pMask_Rectangle = nullptr;	// �|�C���^��������
	}

	// X���f���}�l�[�W���[�j��
	CX_Manager::Release();

	// �e�N�X�`���}�l�[�W���[�j��
	CTexture_Manager::Release();

	// �T�E���h�̔j��
	CSound::GetInstance()->Release();

	// �V�[���}�l�[�W���[�j��
	CScene_Manager::Release();

	// �����_���[�̔j��
	CRenderer::Release();
}