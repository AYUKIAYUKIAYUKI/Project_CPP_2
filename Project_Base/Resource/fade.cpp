//============================================================================
// 
// ���t�F�[�h�J�ڏ��� [fade.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "fade.h"

// �f�o�C�X�擾�p
#include "renderer.h"

// �V�[���擾�p
#include "manager.h"

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CFade* CFade::m_pInstnce = nullptr;

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CFade::Init()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (m_pVtxBuff == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx{};

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// �ʒu�̐ݒ�
	pVtx[0].pos = { 0.0f, 0.0f, 0.0f };
	pVtx[1].pos = { SCREEN_WIDTH, 0.0f, 0.0f };
	pVtx[2].pos = { 0.0f, SCREEN_HEIGHT, 0.0f };
	pVtx[3].pos = { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f };

	// ���Z���̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �F�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̐ݒ�
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ���
//=============================================================================
void CFade::Release()
{
	if (m_pInstnce != nullptr)
	{
		// �t�F�[�h�̏I������
		m_pInstnce->Uninit();

		// �����������
		delete m_pInstnce;

		// �|�C���^��������
		m_pInstnce = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CFade::Update()
{
	// �t�F�[�h�֘A�̔��肪�����o�ĂȂ���Ώ������s��Ȃ�
	if (!m_bFadeIn && !m_bFadeOut)
	{
		return;
	}

	// �t�F�[�h�C������
	FadeIn();

	// �t�F�[�h�A�E�g����
	FadeOut();

	// ���_���̐ݒ菈��
	SetVtx();
}

//============================================================================
// �`�揈��
//============================================================================
void CFade::Draw()
{
	// �t�F�[�h�֘A�̔��肪�����o�ĂȂ���Ώ������s��Ȃ�
	if (!m_bFadeIn && !m_bFadeOut)
	{
		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, nullptr);

	// �|���S���̕`��
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,										// ���_���̐擪�A�h���X
		2);										// �v���~�e�B�u��
}

//============================================================================
// �t�F�[�h�ݒ�
//============================================================================
void CFade::SetFade(CScene::MODE Mode)
{
	CFade* pFade{ GetInstance() };

	// �t�F�[�h�֘A�̔��肪�����o�ĂȂ����
	if (!pFade->m_bFadeIn && !pFade->m_bFadeOut)
	{
		// �t�F�[�h�C��������o��
		pFade->m_bFadeIn = true;

		// ���̃��[�h�̏���ێ����Ă���
		pFade->m_NextMode = Mode;
	}
}

//============================================================================
// �t�F�[�h���擾
//============================================================================
CFade* CFade::GetInstance()
{
	if (m_pInstnce == nullptr)
	{
		// ����
		m_pInstnce->Create();
	}

	return m_pInstnce;
}

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CFade::CFade()
{
	m_pVtxBuff = nullptr;				// ���_�o�b�t�@�̃|�C���^
	m_NextMode = CScene::MODE::NONE;	// ���̃��[�h
	m_bFadeIn = false;					// �t�F�[�h�C��
	m_bFadeOut = false;					// �t�F�[�h�A�E�g
	m_fBrightness = 0.0f;				// ���x
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CFade::~CFade()
{

}

//============================================================================
// ����
//============================================================================
void CFade::Create()
{
	if (m_pInstnce != nullptr)
	{ // ��d�����֎~
		assert(false);
	}

	// �C���X�^���X�𐶐�
	m_pInstnce = DBG_NEW CFade{};
}

//============================================================================
// �I������
//============================================================================
void CFade::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//============================================================================
// �t�F�[�h�C��
//============================================================================
void CFade::FadeIn()
{
	if (m_bFadeIn)
	{
		// ���x���オ���Ă���
		m_fBrightness += 0.05f;

		// �ő喾�x���B��
		if (m_fBrightness > 1.0f)
		{
			// ���x���ō��ɌŒ�
			m_fBrightness = 1.0f;

			// �t�F�[�h�C�����I��
			m_bFadeIn = false;

			// ���̃V�[���֕ύX
			CManager::SetScene(m_NextMode);

			// �t�F�[�h�A�E�g���J�n
			m_bFadeOut = true;
		}
	}
}

//============================================================================
// �t�F�[�h�A�E�g
//============================================================================
void CFade::FadeOut()
{
	if (m_bFadeOut)
	{
		// ���x���������Ă���
		m_fBrightness += -0.05f;

		// �Œᖾ�x���B��
		if (m_fBrightness < 0.0f)
		{
			// ���x���Œ�ɌŒ�
			m_fBrightness = 0.0f;

			// �t�F�[�h�A�E�g���I��
			m_bFadeOut = false;
		}
	}
}

//============================================================================
// ���_���̐ݒ�
//============================================================================
void CFade::SetVtx()
{
	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// �F�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fBrightness);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fBrightness);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fBrightness);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fBrightness);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}