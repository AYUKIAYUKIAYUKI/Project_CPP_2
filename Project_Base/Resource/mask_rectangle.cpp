//============================================================================
// 
// �l�p�`�}�X�N [mask_rectangle.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "mask_rectangle.h"

#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �e�N�X�`���w��
//============================================================================
CMask_Rectangle::CMask_Rectangle(CTexture_Manager::TYPE Type) :
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_Pos{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Size{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_fLength{ 0.0f },
	m_fAngle{ 0.0f }
{
	// �e�N�X�`������
	BindTex(Type);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CMask_Rectangle::~CMask_Rectangle()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CMask_Rectangle::Init()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// ���W�̐ݒ�
	m_Pos = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };

	// �T�C�Y�̐ݒ�
	m_Size = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CMask_Rectangle::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();	// ���_�o�b�t�@�̉��
		m_pVtxBuff = nullptr;	// �|�C���^�̏�����
	}
}

//============================================================================
// �X�V����
//============================================================================
void CMask_Rectangle::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // ���_�o�b�t�@������
		assert(false && "2D�I�u�W�F�N�g�̒��_�o�b�t�@������");
	}

	// �K�v�Ȑ��l���Z�o
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);
	m_fAngle = atan2f(m_Size.x, m_Size.y);

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = {
		m_Pos.x + sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		m_Pos.y + cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[1].pos = {
		m_Pos.x + sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		m_Pos.y + cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[2].pos = {
		m_Pos.x + sinf(m_Rot.z - m_fAngle) * m_fLength,
		m_Pos.y + cosf(m_Rot.z - m_fAngle) * m_fLength,
		0.0f
	};

	pVtx[3].pos = {
		m_Pos.x + sinf(m_Rot.z + m_fAngle) * m_fLength,
		m_Pos.y + cosf(m_Rot.z + m_fAngle) * m_fLength,
		0.0f
	};

#if 0
#ifdef _DEBUG

	for (int i = 0; i < 4; ++i)
	{
		CRenderer::SetDebugString("���_���W : " + std::to_string(pVtx[i].pos.x) + " :  " + std::to_string(pVtx[i].pos.y) + " : " + std::to_string(pVtx[i].pos.z));
	}

#endif	// _DEBUG
#endif

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================================
// �`�揈��
//============================================================================
void CMask_Rectangle::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, m_pTex);

	// �|���S���̕`��
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,										// ���_���̐擪�A�h���X
		NUM_PRIM);								// �v���~�e�B�u��
}

//============================================================================
// �e�N�X�`������
//============================================================================
void CMask_Rectangle::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// �����ƃe�N�X�`������
//============================================================================
void CMask_Rectangle::BindTex(CTexture_Manager::TYPE Type)
{
	m_pTex = CTexture_Manager::GetInstance()->GetTexture(Type);
}

//============================================================================
// ����
//============================================================================
CMask_Rectangle* CMask_Rectangle::Create(CTexture_Manager::TYPE Type)
{
	// �C���X�^���X�𐶐�
	CMask_Rectangle* pNewInstance = DBG_NEW CMask_Rectangle(Type);

	if (pNewInstance == nullptr)
	{ 
		assert(false && "�l�p�`�}�X�N�̐����Ɏ��s");
	}

	// �����ݒ�
	pNewInstance->Init();

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�o�b�t�@�̐���
//============================================================================
HRESULT CMask_Rectangle::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VTX,
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
	VERTEX_2D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// ���_���W�̐ݒ�
		pVtx[wNumVtx].pos = VEC3_INIT;

		// ���Z���̐ݒ�
		pVtx[wNumVtx].rhw = 1.0f;

		// ���_�F�̐ݒ�
		pVtx[wNumVtx].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[wNumVtx].tex = VEC2_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}