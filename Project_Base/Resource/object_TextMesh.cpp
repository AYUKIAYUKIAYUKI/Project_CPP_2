//============================================================================
// 
// �e�L�X�g���b�V���I�u�W�F�N�g [object_TextMesh.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_TextMesh.h"
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
// �R���X�g���N�^
//============================================================================
CObject_TextMesh::CObject_TextMesh(LAYER Priority) :
	CObject{ Priority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_pSurface{ nullptr },
	m_Size{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_fLength{ 0.0f },
	m_fAngle{ 0.0f }
{
	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_TextMesh::~CObject_TextMesh()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_TextMesh::Init()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̐���
	if (FAILED(CreateTex()))
	{
		return E_FAIL;
	}

	// �e�L�X�g�^�C�v�ɐݒ�
	SetType(TYPE::TEXT);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_TextMesh::Uninit()
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
void CObject_TextMesh::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // ���_�o�b�t�@������
		assert(false);
	}

	// �K�v�Ȑ��l���v�Z
	m_fAngle = atan2f(m_Size.x, m_Size.y);
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�̐ݒ�
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

	// ���_�F�̐ݒ�
	for (WORD wCntVtx = 0; wCntVtx < NUM_VTX; ++wCntVtx)
	{
		pVtx[wCntVtx].col = m_Col;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���[���h�s��̐ݒ�
	SetMtxWorld();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_TextMesh::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

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
		NUM_PRIM);								// �v���~�e�B�u��

	// ���C�g���f��L���ɂ���
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// �T�[�t�F�C�X���擾
//============================================================================
LPDIRECT3DSURFACE9 CObject_TextMesh::GetSurface()
{
	return m_pSurface;
}

//============================================================================
// �T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CObject_TextMesh::GetSize() const
{
	return m_Size;
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CObject_TextMesh::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �����擾
//============================================================================
const D3DXVECTOR3& CObject_TextMesh::GetRot()const
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_TextMesh::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// ���W�擾
//============================================================================
const D3DXVECTOR3& CObject_TextMesh::GetPos() const
{
	return m_Pos;
}

//============================================================================
// ���W�ݒ�
//============================================================================
void CObject_TextMesh::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �F���擾
//============================================================================
const D3DXCOLOR& CObject_TextMesh::GetCol() const
{
	return m_Col;
}

//============================================================================
// �F��ݒ�
//============================================================================
void CObject_TextMesh::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// �A���t�@�l���擾
//============================================================================
const float& CObject_TextMesh::GetAlpha() const
{
	return m_Col.a;
}

//============================================================================
// �A���t�@�l��ݒ�
//============================================================================
void CObject_TextMesh::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// �W�J�p�Ίp���擾
//============================================================================
const float& CObject_TextMesh::GetLength() const
{
	return m_fLength;
}

//============================================================================
// ����
//============================================================================
CObject_TextMesh* CObject_TextMesh::Create()
{
	// �C���X�^���X�𐶐�
	CObject_TextMesh* pObject3D = DBG_NEW CObject_TextMesh();

	// �����o���Ă����珉���ݒ�
	if (pObject3D != nullptr)
	{
		pObject3D->Init();
	}

	return pObject3D;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�o�b�t�@�̐���
//============================================================================
HRESULT CObject_TextMesh::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VTX,
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
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// ���_���W�̐ݒ�
		pVtx[wNumVtx].pos = { VEC3_INIT };

		// �@���x�N�g���̐ݒ�
		pVtx[wNumVtx].nor = { VEC3_INIT };

		// ���_�F�̐ݒ�
		pVtx[wNumVtx].col = XCOL_INIT;
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// �e�N�X�`���̐���
//============================================================================
HRESULT CObject_TextMesh::CreateTex()
{
	// �f�o�C�X���擾
	auto pDev = CRenderer::GetDeviece();

	JSON Json = utility::OpenJsonFile("Data\\JSON\\debug_param.json");
	m_Size = utility::JsonConvertToVec3(Json["Size"]);

	// �e�N�X�`�����쐬
	HRESULT hr = pDev->CreateTexture(
		static_cast<UINT>(m_Size.x) * 2,	// U
		static_cast<UINT>(m_Size.y) * 2,	// V
		0,								// �~�b�v�}�b�v���x��
		D3DUSAGE_RENDERTARGET,			// �e�N�X�`���̐���
		D3DFMT_A8R8G8B8,				// �s�N�Z���t�H�[�}�b�g
		D3DPOOL_DEFAULT,				// �������Ǘ��t���O
		&m_pTex,						// �e�N�X�`���ۑ���
		nullptr);						// �n���h��

	if (FAILED(hr))
	{ // �������s
		return E_FAIL;
	}

	// �e�N�X�`���̃T�[�t�F�C�X����ێ�
	m_pTex->GetSurfaceLevel(0, &m_pSurface);

	return S_OK;
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CObject_TextMesh::SetMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxRot, mtxTrans;

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