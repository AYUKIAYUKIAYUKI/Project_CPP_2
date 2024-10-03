//============================================================================
// 
// 2D�I�u�W�F�N�g�Ǘ� [object_2D.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_2D.h"

// �f�o�C�X���擾
#include "renderer.h"

//============================================================================
// �R���X�g���N�^
//============================================================================
CObject_2D::CObject_2D(int nPriority) :
	CObject{ nPriority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_Pos{ 0.0f, 0.0f, 0.0f },			// ���W
	m_Rot{ 0.0f, 0.0f, 0.0f },			// ����
	m_Size{ 0.0f, 0.0f, 0.0f },			// �T�C�Y
	m_Col{ 1.0f, 1.0f, 1.0f, 1.0f },	// �F
	m_fLength{ 0.0f },					// �W�J�p�Ίp��
	m_fAngle{ 0.0f },					// �Ίp���p�p�x
	m_fTexWidth{ 1.0f },				// ���e�N�X�`��������
	m_fTexHeight{ 1.0f },				// �c�e�N�X�`�����c��
	m_nNowPatternU{ 0 },				// ���݂̉��e�N�X�`�����
	m_nNowPatternV{ 0 }					// ���݂̏c�e�N�X�`�����
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_2D::~CObject_2D()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_2D::Init()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev{ CRenderer::GetInstance()->GetDeviece() };

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
	VERTEX_2D* pVtx{ nullptr };

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// �ʒu�̐ݒ�
	pVtx[0].pos = { 0.0f, 0.0f, 0.0f };
	pVtx[1].pos = { 0.0f, 0.0f, 0.0f };
	pVtx[2].pos = { 0.0f, 0.0f, 0.0f };
	pVtx[3].pos = { 0.0f, 0.0f, 0.0f };

	// ���Z���̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �F�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̐ݒ�
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_2D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CObject_2D::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // ���_�o�b�t�@������
		assert(false);
	}

	// �K�v�Ȑ��l���Z�o
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);
	m_fAngle = atan2f(m_Size.x, m_Size.y);

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx{ nullptr };

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu�̐ݒ�
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

	// �F�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// �e�N�X�`���̐ݒ�
	pVtx[0].tex = { m_fTexWidth * m_nNowPatternU, m_fTexHeight * m_nNowPatternV };
	pVtx[1].tex = { m_fTexWidth * (m_nNowPatternU + 1), m_fTexHeight * m_nNowPatternV };
	pVtx[2].tex = { m_fTexWidth * (m_nNowPatternU), m_fTexHeight * (m_nNowPatternV + 1) };
	pVtx[3].tex = { m_fTexWidth * (m_nNowPatternU + 1), m_fTexHeight * (m_nNowPatternV + 1) };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_2D::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev{ CRenderer::GetInstance()->GetDeviece() };

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, m_pTex);

	// �|���S���̕`��
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,										// ���_���̐擪�A�h���X
		2);										// �v���~�e�B�u��
}

//============================================================================
// �e�N�X�`������
//============================================================================
void CObject_2D::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// �����ƃe�N�X�`������
//============================================================================
void CObject_2D::BindTex(CTexture_Manager::TYPE Type)
{
	m_pTex = CTexture_Manager::GetInstance()->GetTexture(Type);
}

//============================================================================
// ���W�擾
//============================================================================
D3DXVECTOR3 CObject_2D::GetPos()
{
	return m_Pos;
}

//============================================================================
// ���W�ݒ�
//============================================================================
void CObject_2D::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �����擾
//============================================================================
D3DXVECTOR3 CObject_2D::GetRot()
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_2D::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// �T�C�Y�擾
//============================================================================
D3DXVECTOR3 CObject_2D::GetSize()
{
	return m_Size;
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CObject_2D::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �F���擾
//============================================================================
D3DXCOLOR CObject_2D::GetCol()
{
	return m_Col;
}

//============================================================================
// �F��ݒ�
//============================================================================
void CObject_2D::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// �A���t�@�l���擾
//============================================================================
float& CObject_2D::GetAlpha()
{
	return m_Col.a;
}

//============================================================================
// �A���t�@�l��ݒ�
//============================================================================
void CObject_2D::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// �W�J�p�Ίp���擾
//============================================================================
float CObject_2D::GetLength()
{
	return m_fLength;
}

//============================================================================
// ���e�N�X�`���������ݒ�
//============================================================================
void CObject_2D::SetTexWidth(float fWidth)
{
	m_fTexWidth = fWidth;
}

//============================================================================
// �c�e�N�X�`���������ݒ�
//============================================================================
void CObject_2D::SetTexHeight(float fHeight)
{
	m_fTexHeight = fHeight;
}

//============================================================================
// ���݂̃e�N�X�`�����������擾
//============================================================================
int CObject_2D::GetNowPatternU()
{
	return m_nNowPatternU;
}

//============================================================================
// ���݂̃e�N�X�`�����������ݒ�
//============================================================================
void CObject_2D::SetNowPatternU(int nNowPatternU)
{
	m_nNowPatternU = nNowPatternU;
}

//============================================================================
// ���݂̃e�N�X�`���c�������擾
//============================================================================
int CObject_2D::GetNowPatternV()
{
	return m_nNowPatternV;
}

//============================================================================
// ���݂̃e�N�X�`���c�������ݒ�
//============================================================================
void CObject_2D::SetNowPatternV(int nNowPatternV)
{
	m_nNowPatternV = nNowPatternV;
}

//============================================================================
// ����
//============================================================================
CObject_2D* CObject_2D::Create()
{
	CObject_2D* pObject2D = DBG_NEW CObject_2D{};

	// �����o���Ă����珉���ݒ�
	if (pObject2D != nullptr)
	{
		pObject2D->Init();
	}

	return pObject2D;
}