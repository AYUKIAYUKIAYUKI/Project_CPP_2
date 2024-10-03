//============================================================================
// 
// �r���{�[�h�I�u�W�F�N�g�Ǘ� [object_billboard.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_billboard.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//============================================================================
// �R���X�g���N�^
//============================================================================
CObject_billboard::CObject_billboard(int nPriority) :
	CObject{ nPriority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_Pos{ 0.0f, 0.0f, 0.0f },			// ���W
	m_Rot{ 0.0f, 0.0f, 0.0f },			// ����
	m_Size{ 0.0f, 0.0f, 0.0f },			// �T�C�Y
	m_fLength{ 0.0f },					// �W�J�p�Ίp��
	m_fAngle{ 0.0f },					// �Ίp���p�p�x
	m_fTexWidth{ 1.0f },				// ���e�N�X�`��������
	m_fTexHeight{ 1.0f },				// �c�e�N�X�`�����c��
	m_nNowPatternU{ 0 },				// ���݂̉��e�N�X�`�����
	m_nNowPatternV{ 0 }					// ���݂̏c�e�N�X�`�����
{
	D3DXMatrixIdentity(&m_MtxWorld);	// ���[���h�s��
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_billboard::~CObject_billboard()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_billboard::Init()
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
void CObject_billboard::Uninit()
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
void CObject_billboard::Update()
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

	// �e�N�X�`���̐ݒ�
	pVtx[0].tex = { m_fTexWidth * m_nNowPatternU, m_fTexHeight * m_nNowPatternV };
	pVtx[1].tex = { m_fTexWidth * (m_nNowPatternU + 1), m_fTexHeight * m_nNowPatternV };
	pVtx[2].tex = { m_fTexWidth * (m_nNowPatternU), m_fTexHeight * (m_nNowPatternV + 1) };
	pVtx[3].tex = { m_fTexWidth * (m_nNowPatternU + 1), m_fTexHeight * (m_nNowPatternV + 1) };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���[���h�s��̐ݒ�
	SetMtxWorld();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_billboard::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev{ CRenderer::GetInstance()->GetDeviece() };

	//// �[�x�e�X�g�̔�r���@�̕ύX
	//pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//// �[�x�o�b�t�@�ɕ`�悵�Ȃ�
	//pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHAREF, 0);
	pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C�g���f�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	// ���C�g���f��L���ɂ���
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// �[�x�e�X�g�̔�r���@�̕ύX
	//pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//// �[�x�o�b�t�@�ɏ�������
	//pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// �e�N�X�`������
//============================================================================
void CObject_billboard::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// �����ƃe�N�X�`������
//============================================================================
void CObject_billboard::BindTex(CTexture_Manager::TYPE Type)
{
	m_pTex = CTexture_Manager::GetInstance()->GetTexture(Type);
}

//============================================================================
// �ʒu�擾
//============================================================================
D3DXVECTOR3 CObject_billboard::GetPos()
{
	return m_Pos;
}

//============================================================================
// �ʒu�ݒ�
//============================================================================
void CObject_billboard::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �����擾
//============================================================================
D3DXVECTOR3 CObject_billboard::GetRot()
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_billboard::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// �T�C�Y�擾
//============================================================================
D3DXVECTOR3 CObject_billboard::GetSize()
{
	return m_Size;
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CObject_billboard::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �W�J�p�Ίp���擾
//============================================================================
float CObject_billboard::GetLength()
{
	return m_fLength;
}

//============================================================================
// ���e�N�X�`���������ݒ�
//============================================================================
void CObject_billboard::SetTexWidth(float fWidth)
{
	m_fTexWidth = fWidth;
}

//============================================================================
// �c�e�N�X�`���������ݒ�
//============================================================================
void CObject_billboard::SetTexHeight(float fHeight)
{
	m_fTexHeight = fHeight;
}

//============================================================================
// ���݂̃e�N�X�`�����������擾
//============================================================================
int CObject_billboard::GetNowPatternU()
{
	return m_nNowPatternU;
}

//============================================================================
// ���݂̃e�N�X�`�����������ݒ�
//============================================================================
void CObject_billboard::SetNowPatternU(int nNowPatternU)
{
	m_nNowPatternU = nNowPatternU;
}

//============================================================================
// ���݂̃e�N�X�`���c�������擾
//============================================================================
int CObject_billboard::GetNowPatternV()
{
	return m_nNowPatternV;
}

//============================================================================
// ���݂̃e�N�X�`���c�������ݒ�
//============================================================================
void CObject_billboard::SetNowPatternV(int nNowPatternV)
{
	m_nNowPatternV = nNowPatternV;
}

//============================================================================
// ����
//============================================================================
CObject_billboard* CObject_billboard::Create()
{
	CObject_billboard* pObject3D = DBG_NEW CObject_billboard{};

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
void CObject_billboard::SetMtxWorld()
{
	LPDIRECT3DDEVICE9 pDev{ CRenderer::GetInstance()->GetDeviece() };

	// �v�Z�p�s��
	D3DXMATRIX mtxRot{}, mtxTrans{}, mtxView{};

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

	// �r���[�s��擾
	pDev->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����̐��ʂɌ���������
	D3DXMatrixInverse(&m_MtxWorld, nullptr, &mtxView);

	// �t�s������߂�
	m_MtxWorld._41 = 0.0f;
	m_MtxWorld._42 = 0.0f;
	m_MtxWorld._43 = 0.0f;

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