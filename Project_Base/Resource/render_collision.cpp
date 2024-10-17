//============================================================================
// 
// ����\�� [render_collision.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "render_collision.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
const int CRender_Collision::m_nNumVtx = 8;			// ���_��
const int CRender_Collision::m_nNumPrimitive = 12;	// �Ӑ�
const int CRender_Collision::m_nNumIdx = 24;		// �C���f�b�N�X��

//============================================================================
// �R���X�g���N�^
//============================================================================
CRender_Collision::CRender_Collision(CObject_X* pObj, D3DXVECTOR3& posRef, D3DXVECTOR3& sizeRef, int nPriority) :
	CObject{ nPriority },				// �v���C�I���e�B
	m_pVtxBuff{ nullptr },				// ���_�o�b�t�@�̃|�C���^
	m_pIdxBuff{ nullptr },				// �C���f�b�N�X�o�b�t�@�̃|�C���^
	m_pRefPtr(pObj),					// �Q�Ɛ�
	m_posRef{ posRef },					// �Q�ƈʒu
	m_col{ 1.0f, 0.0f, 0.0f, 1.0f },	// �F
	m_size{ sizeRef }
{
	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CRender_Collision::~CRender_Collision()
{
	// �O�̂��ߏI������
	Uninit();

	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CRender_Collision::Init()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(CreateIdxBuff()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CRender_Collision::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CRender_Collision::Update()
{
	// ���_�̐ݒ�
	SetVtx();

	// ���[���h�s��̐ݒ�
	SetMtxWorld();

	SetRelease();
}

//============================================================================
// �`�揈��
//============================================================================
void CRender_Collision::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

	// ���C�g���I�t
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, nullptr);

	// �{�b�N�X�̕`��
	pDev->DrawIndexedPrimitive(D3DPT_LINELIST,
		0,
		0,
		m_nNumVtx,			// ���_��
		0,
		m_nNumPrimitive);	// �ӂ̐�
	
	// ���C�g���I��
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================================
// ����
//============================================================================
CRender_Collision* CRender_Collision::Create(CObject_X* pObj, D3DXVECTOR3& posRef, D3DXVECTOR3& sizeRef)
{
	// �C���X�^���X�𐶐�
	CRender_Collision* pRender_Collision = DBG_NEW CRender_Collision{ pObj, posRef, sizeRef, static_cast<int>(LAYER::FRONT) };

	// �����o���Ă����珉���ݒ�
	if (pRender_Collision != nullptr)
	{
		pRender_Collision->Init();
	}

	return pRender_Collision;
}

//============================================================================
// ���_�o�b�t�@�̐���
//============================================================================
HRESULT CRender_Collision::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
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
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (int i = 0; i < m_nNumVtx; i++)
	{
		// �ʒu�̐ݒ�
		pVtx[i].pos = { 0.0f, 0.0f, 0.0f };

		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = { 0.0f, 0.0f, 0.0f };

		// �F�̐ݒ�
		pVtx[i].col = m_col;

		// �e�N�X�`���̐ݒ�
		pVtx[i].tex = { 0.0f, 0.0f };
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// �C���f�b�N�X���̐���
//============================================================================
HRESULT CRender_Collision::CreateIdxBuff()
{
	// �C���f�b�N�X�o�b�t�@�̐���
	CRenderer::GetInstance()->GetDeviece()->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	if (m_pIdxBuff == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �C���f�b�N�X���ւ̃|�C���^
	WORD* pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, reinterpret_cast<void**>(&pIdx), 0);

	// X���ɕ��s�Ȑ�������
	for (int i = 0; i < 8; i++, pIdx++)
	{
		pIdx[0] = static_cast<WORD>(i);
	}

	// Y���ɕ��s�Ȑ�������
	for (int i = 0, j = 0; i < 4; i++, pIdx += 2)
	{
		if (i == 2) j += 2;	// 0,1,4,5��ӂ̎n�_�ɂ��邽��3,4�̃C���f�b�N�X���΂�

		pIdx[0] = static_cast<WORD>(i + j);
		pIdx[1] = static_cast<WORD>(i + 2 + j);
	}

	// Z���ɕ��s�Ȑ�������
	for (int i = 0; i < 4; i++, pIdx += 2)
	{
		pIdx[0] = static_cast<WORD>(i);
		pIdx[1] = static_cast<WORD>(i + 4);
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}


//============================================================================
// ���_�̐ݒ�
//============================================================================
void CRender_Collision::SetVtx()
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N�����_��񎞂ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	pVtx[0].pos = { -m_size.x, +m_size.y, -m_size.z };
	pVtx[1].pos = { +m_size.x, +m_size.y, -m_size.z };
	pVtx[2].pos = { -m_size.x, -m_size.y, -m_size.z };
	pVtx[3].pos = { +m_size.x, -m_size.y, -m_size.z };
	pVtx[4].pos = { -m_size.x, +m_size.y, +m_size.z };
	pVtx[5].pos = { +m_size.x, +m_size.y, +m_size.z };
	pVtx[6].pos = { -m_size.x, -m_size.y, +m_size.z };
	pVtx[7].pos = { +m_size.x, -m_size.y, +m_size.z };

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CRender_Collision::SetMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_mtxWorld);

	// ���s�ړ��s��쐬
	D3DXMatrixTranslation(&mtxTrans,
		m_posRef.x,
		m_posRef.y,
		m_posRef.z);

	// ���s�ړ��s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxTrans);
}