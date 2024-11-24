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
#include "scene.h"

// �`�揈���p
#include "manager.h"

// �I�u�W�F�N�g�擾
#include "object.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
#define ENABLE_STENCIL_BUFFER 1	// �X�e���V���o�b�t�@�̗L����

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CRenderer* CRenderer::m_pRenderer = nullptr;	// �����_���[�̖{��

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CRenderer::Update()
{
	// ������N���A
	m_DebugStr = {};

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
#if ENABLE_STENCIL_BUFFER

	// ��ʃo�b�t�@�N���A
	m_pD3DDevice->Clear(0,											// �N���A�������l�p�`�̐���ݒ� (�r���[�|�[�g�S�̂̏ꍇ��0)
		nullptr,													// �l�p�`�\���̂̃|�C���^��ݒ� (nullptr��n�����ƂŃr���[�|�[�g�S�͈̂̔�)
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// �N���A����T�[�t�F�X���w�肷��
		D3DCOLOR_RGBA(0, 0, 0, 0),									// ���̃J���[�Ń^�[�Q�b�g���N���A
		1.0f,														// ���̒l�ɑ債�ăf�v�X�o�b�t�@���N���A
		0);															// ���̒l�ŃX�e���V���o�b�t�@���N���A

	// �X�e���V���o�b�t�@�ւ̏������݂�L����
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

#else

	// ��ʃo�b�t�@�N���A
	m_pD3DDevice->Clear(0,
		nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

#endif

	// �^���X�N���[���̃e�N�X�`�����֕`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// �J�������Z�b�g
		CManager::GetManager()->GetCamera()->SetCamera();

		// �S�I�u�W�F�N�g�̕`��
		CObject::DrawAll();

		// �V�[���̐�p�`��
		CScene_Manager::GetInstance()->GetScene()->Draw();

		// ImGui�̕`��
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

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
	// �\���͈͒����p
	static WORD wAdjustHeight = 0;

	// �\���͈͕ϓ�
	if (CManager::GetKeyboard()->GetPress(DIK_NUMPAD2))
	{
		wAdjustHeight > 0 ? wAdjustHeight -= 5 : wAdjustHeight = 0;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_NUMPAD8))
	{
		wAdjustHeight += 5;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_NUMPAD0))
	{
		wAdjustHeight = 0;
	}

	// �\���ʒu
	RECT Rect = { 0, 0 - wAdjustHeight, WSCREEN_WIDTH, WSCREEN_HEIGHT - wAdjustHeight };

	for (WORD i = 0; i < m_TimeStr.size(); ++i)
	{
		if (m_TimeStr[i].second > 0)
		{
			// �\�����Ԃ��J�E���g�_�E��
			m_TimeStr[i].second--;

			// ���͂��Ȃ���
			m_DebugStr += m_TimeStr[i].first + "\n";
		}
		else
		{
			// ���͂�����
			m_TimeStr.erase(m_TimeStr.begin() + i);
		}
	}

	// �e�L�X�g�̕`��
	m_pFont->DrawText(NULL, m_DebugStr.c_str(), -1, &Rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//============================================================================
// ����
//============================================================================
HRESULT CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer != nullptr)
	{
		assert(false && "�����_���[�͊��ɍ쐬����Ă��邩�A�_���O�����O���Ă��܂�");
	}

	// �����_���[�𐶐�
	m_pRenderer = DBG_NEW CRenderer();

	if (m_pRenderer == nullptr)
	{
		assert(false && "�����_���[�̐����Ɏ��s");
	}

	// �����_���[�̏����ݒ�
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
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
	// �����_���[�̔j��
	if (m_pRenderer != nullptr)
	{
		// �����_���[�I������
		m_pRenderer->Uninit();

		// �����������
		delete m_pRenderer;

		// �|�C���^��������
		m_pRenderer = nullptr;
	}
}

//============================================================================
// �����_���[�̎擾
//============================================================================
CRenderer* CRenderer::GetRenderer()
{
	return m_pRenderer;
}

//============================================================================
// �f�o�C�X�̎擾
//============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDeviece()
{
	return m_pRenderer->m_pD3DDevice;
}

//============================================================================
// �f�o�b�O�p������ɒǉ�
//============================================================================
void CRenderer::SetDebugString(std::string Str)
{
	m_pRenderer->m_DebugStr += Str + "\n";
}

//============================================================================
// �������f�o�b�O������ɒǉ�
//============================================================================
void CRenderer::SetTimeString(std::string Str, int nCnt)
{
	m_pRenderer->m_TimeStr.push_back({ Str, nCnt });
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CRenderer::CRenderer() :
	m_pD3D{ nullptr },
	m_pD3DDevice{ nullptr },
	m_pFont{ nullptr },
	m_DebugStr{},
	m_TimeStr{}
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
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// �p�����[�^�̃[���N���A
#if 0
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// �Q�[���T�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		// �Q�[���T�C�Y(����)
#else
	d3dpp.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);	// �Q�[���T�C�Y(��)
	d3dpp.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);	// �Q�[���T�C�Y(����)
#endif
	d3dpp.BackBufferFormat = d3ddm.Format;		// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;					// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;		// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
#if ENABLE_STENCIL_BUFFER
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// �f�v�X�o�b�t�@�Ƃ���24bit�A�X�e���V���o�b�t�@�Ƃ���8bit���g�p����
#else
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// �f�v�X�o�b�t�@�Ƃ���16bit���g��
#endif
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

	// �����_�[�X�e�[�g�̏����ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̏����ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ImGui�̃R���e�L�X�g���쐬
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ImGui�̓��o�͐ݒ�
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.Fonts->AddFontFromFileTTF("Data\\FONT\\meiryo.ttc", 18.0f, nullptr);

	// ImGui�̕\���X�^�C����ݒ�
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// �o�b�N�G���h�̏����ݒ�
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(CRenderer::GetDeviece());

	// �t�H���g�𐶐�
	D3DXCreateFont(m_pD3DDevice,
		18,
		0,
		FW_HEAVY,
		1,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pFont);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CRenderer::Uninit()
{
	// �S�I�u�W�F�N�g�������
	CObject::ReleaseAll();

	// �t�H���g�̔j��
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}

	// ImGUi�̏I������
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

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