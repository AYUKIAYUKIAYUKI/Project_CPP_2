//============================================================================
// 
// �����_���[ [renderer.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "renderer.h"

// �`�揈���p
#include "manager.h"

// �I�u�W�F�N�g�擾
#include "object.h"

// �V���O���g���Ǘ�
#include "texture_manager.h"
#include "model_X_manager.h"

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CRenderer* CRenderer::m_pRenderer = nullptr;	// �����_���[

//============================================================================
// �R���X�g���N�^
//============================================================================
CRenderer::CRenderer() :
	m_pD3D{ nullptr },				// Direct3D
	m_pD3DDevice{ nullptr },		// �f�o�C�X
	m_pFont{ nullptr },				// �t�H���g
	m_debugStr{},					// �\���p������
	m_timeStr{}						// ������������
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CRenderer::~CRenderer()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindiw)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{ // �����Ɏ��s�����ꍇ
		return E_FAIL;
	}

	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{ // ���݂̃f�B�X�v���C���[�h���擾
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));	// �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[���T�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[���T�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindiw;									// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐����s���@
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		// Direct3D�f�o�C�X�̐����s���A
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// Direct3D�f�o�C�X�̐����s���B
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ?
	//ID3DXAllocateHierarchy;

	// �t�H���g�𐶐�
	D3DXCreateFont(m_pD3DDevice, 22, 0, FW_HEAVY, 1,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &m_pFont);

	// �e�N�X�`���}�l�[�W���[�����ݒ�
	if (FAILED(CTexture_Manager::GetInstance()->Load()))
	{
		return E_FAIL;
	}

	// X���f���}�l�[�W���[�����ݒ�
	if (FAILED(CModel_X_Manager::GetInstance()->Load()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CRenderer::Release()
{
	if (m_pRenderer != nullptr)
	{
		// �I������
		m_pRenderer->Uninit();

		// �����������
		delete m_pRenderer;

		// �|�C���^��������
		m_pRenderer = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CRenderer::Update()
{
	// ������N���A
	m_debugStr = {};

	// �S�I�u�W�F�N�g�X�V����
	CObject::UpdateAll();

	// �S�I�u�W�F�N�g��X�V����
	CObject::LateUpdateAll();
}

//============================================================================
// �`�揈��
//============================================================================
void CRenderer::Draw()
{
	//// ��ʃo�b�t�@�N���A
	//m_pD3DDevice->Clear(0, nullptr,
	//	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
	//	D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// �^���X�N���[���̃e�N�X�`�����֕`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// �J�������Z�b�g
		CManager::GetCamera()->SetCamera();

		// �S�I�u�W�F�N�g�̕`��
		CObject::DrawAll();

		// �V�[���̐�p�`��
		CManager::GetScene()->Draw();

		// �f�o�b�O�\��
		PrintDebug();

		// �`��I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//============================================================================
// �f�o�b�O�\��
//============================================================================
void CRenderer::PrintDebug()
{
	// �\���ʒu
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	for (unsigned int i = 0; i < m_timeStr.size(); i++)
	{
		if (m_timeStr[i].second > 0)
		{
			// �\�����Ԃ��J�E���g�_�E��
			m_timeStr[i].second--;

			// ���͂��Ȃ���
			m_debugStr += m_timeStr[i].first + "\n";
		}
		else
		{
			// ���͂�����
			m_timeStr.erase(m_timeStr.begin() + i);
		}
	}

	//�e�L�X�g�̕`��
	m_pFont->DrawText(NULL, m_debugStr.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//============================================================================
// �����_���[�̎擾
//============================================================================
CRenderer* CRenderer::GetInstance()
{
	if (m_pRenderer == nullptr)
	{
		// ����
		m_pRenderer->Create();
	}

	return m_pRenderer;
}

//============================================================================
// �f�o�C�X�̎擾
//============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDeviece()
{
	return m_pD3DDevice;
}

//============================================================================
// �f�o�b�O�p������ɒǉ�
//============================================================================
void CRenderer::SetDebugString(std::string str)
{
	m_debugStr += str + "\n";
}

//============================================================================
// �������f�o�b�O������ɒǉ�
//============================================================================
void CRenderer::SetTimeString(std::string str, int nCnt)
{
	m_timeStr.push_back({ str, nCnt });
}

//============================================================================
// ����
//============================================================================
void CRenderer::Create()
{
	if (m_pRenderer != nullptr)
	{ // ��d�����֎~
		assert(false);
	}

	// �C���X�^���X�𐶐�
	m_pRenderer = DBG_NEW CRenderer{};
}

//============================================================================
// �I������
//============================================================================
void CRenderer::Uninit()
{
	// �S�I�u�W�F�N�g�������
	CObject::ReleaseAll();

	// X���f���}�l�[�W���[�j��
	CModel_X_Manager::GetInstance()->Release();

	// �e�N�X�`���}�l�[�W���[�j��
	CTexture_Manager::GetInstance()->Release();

	// �t�H���g�̔j��
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}

	// Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}