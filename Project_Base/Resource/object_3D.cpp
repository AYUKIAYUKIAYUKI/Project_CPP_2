//============================================================================
// 
// 3D�I�u�W�F�N�g�Ǘ� [object_3D.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_3D.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//============================================================================
// �R���X�g���N�^
//============================================================================
CObject_3D::CObject_3D(int nPriority) :
	CObject{ nPriority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_Pos{ 0.0f, 0.0f, 0.0f },			// ���W
	m_Rot{ 0.0f, 0.0f, 0.0f },			// ����
	m_Size{ 0.0f, 0.0f, 0.0f },			// �T�C�Y
	m_Col{ 1.0f, 1.0f, 1.0f, 1.0f },	// �F
	m_fLength{ 0.0f },					// �W�J�p�Ίp��
	m_fAngle{ 0.0f }					// �Ίp���p�p�x
{
	D3DXMatrixIdentity(&m_MtxWorld);	// ���[���h�s��
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_3D::~CObject_3D()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_3D::Init()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev{ CRenderer::GetInstance()->GetDeviece() };

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (m_pVtxBuff == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx{ nullptr };

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu�̐ݒ�
	pVtx[0].pos = { 0.0f, 0.0f, 0.0f };
	pVtx[1].pos = { 0.0f, 0.0f, 0.0f };
	pVtx[2].pos = { 0.0f, 0.0f, 0.0f };
	pVtx[3].pos = { 0.0f, 0.0f, 0.0f };

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = { 0.0f, 1.0f, 0.0f };
	pVtx[1].nor = { 0.0f, 1.0f, 0.0f };
	pVtx[2].nor = { 0.0f, 1.0f, 0.0f };
	pVtx[3].nor = { 0.0f, 1.0f, 0.0f };

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
void CObject_3D::Uninit()
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
void CObject_3D::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // ���_�o�b�t�@������
		assert(false);
	}

	// �K�v�Ȑ��l���v�Z
	m_fAngle = atan2f(m_Size.x, m_Size.y);
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx{ nullptr };

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// �ʒu�̐ݒ�
	pVtx[0].pos = {
		sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[1].pos = {
		sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[2].pos = {
		sinf(m_Rot.z - m_fAngle) * m_fLength,
		-cosf(m_Rot.z - m_fAngle) * m_fLength,
		0.0f
	};

	pVtx[3].pos = {
		sinf(m_Rot.z + m_fAngle) * m_fLength,
		-cosf(m_Rot.z + m_fAngle) * m_fLength,
		0.0f
	};

	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���[���h�s��̐ݒ�
	SetMtxWorld();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_3D::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev{ CRenderer::GetInstance()->GetDeviece() };

	// ���[���h�s��̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_3D);

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
void CObject_3D::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// �����ƃe�N�X�`������
//============================================================================
void CObject_3D::BindTex(CTexture_Manager::TYPE Type)
{
	m_pTex = CTexture_Manager::GetInstance()->GetTexture(Type);
}

//============================================================================
// ���W�擾
//============================================================================
D3DXVECTOR3 CObject_3D::GetPos()
{
	return m_Pos;
}

//============================================================================
// ���W�ݒ�
//============================================================================
void CObject_3D::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �����擾
//============================================================================
D3DXVECTOR3 CObject_3D::GetRot()
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_3D::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// �T�C�Y�擾
//============================================================================
D3DXVECTOR3 CObject_3D::GetSize()
{
	return m_Size;
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CObject_3D::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �A���t�@�l���擾
//============================================================================
float& CObject_3D::GetAlpha()
{
	return m_Col.a;
}

//============================================================================
// �A���t�@�l��ݒ�
//============================================================================
void CObject_3D::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// �W�J�p�Ίp���擾
//============================================================================
float CObject_3D::GetLength()
{
	return m_fLength;
}

//============================================================================
// ����
//============================================================================
CObject_3D* CObject_3D::Create()
{
	CObject_3D* pObject3D = DBG_NEW CObject_3D{};

	// �����o���Ă����珉���ݒ�
	if (pObject3D != nullptr)
	{
		pObject3D->Init();
	}

	return pObject3D;
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CObject_3D::SetMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxRot{}, mtxTrans{};

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�s��쐬
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y,
		m_Rot.x,
		m_Rot.z);

	// ��]�s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxRot);

	// ���s�ړ��s��쐬
	D3DXMatrixTranslation(&mtxTrans,
		m_Pos.x,
		m_Pos.y,
		m_Pos.z);

	// ���s�ړ��s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);
}