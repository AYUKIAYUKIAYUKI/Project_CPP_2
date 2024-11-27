//============================================================================
// 
// ���\�� [render_box.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "render_box.h"
#include "renderer.h"
#include "object_X.h"

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
// �`��D��x�w��R���X�g���N�^
//============================================================================
CRender_Box::CRender_Box(LAYER Priority) :
	CRender_Collision{ Priority },
	m_pVtxBuff{ nullptr },
	m_pIdxBuff{ nullptr },
	m_SyncSize{ VEC3_INIT },
	m_SyncRot{ VEC3_INIT }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CRender_Box::~CRender_Box()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CRender_Box::Init()
{
	// ����\���N���X�̏����ݒ�
	if (FAILED(CRender_Collision::Init()))
	{
		return E_FAIL;
	}

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
void CRender_Box::Uninit()
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

	// ����\���N���X�̏I������
	CRender_Collision::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CRender_Box::Update()
{
	// ���_�̐ݒ�
	SetVtx();

	// ����\���N���X�̍X�V����
	CRender_Collision::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CRender_Box::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���C�g���I�t
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, nullptr);

	// ���̕`��
	pDev->DrawIndexedPrimitive(D3DPT_LINELIST,
		0,
		0,
		NUM_VTX,	// ���_��
		0,
		NUM_PRIM);	// �ӂ̐�

	// ���C�g���I��
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================================
// �V���N���T�C�Y��ݒ�
//============================================================================
void CRender_Box::SetSyncSize(D3DXVECTOR3 Size)
{
	m_SyncSize = Size;
}

//============================================================================
// �V���N��������ݒ�
//============================================================================
void CRender_Box::SetSyncRot(D3DXVECTOR3 Rot)
{
	m_SyncRot = Rot;
}

//============================================================================
// ����
//============================================================================
CRender_Box* CRender_Box::Create()
{
	// ����\���𐶐�
	CRender_Box* pNew = DBG_NEW CRender_Box(LAYER::FRONT);

	// �������s
	if (pNew == nullptr)
	{
		assert(false && "���\���̐����Ɏ��s���܂���");
	}

	// ����\���̏����ݒ�
	pNew->Init();

	return pNew;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�o�b�t�@�̐���
//============================================================================
HRESULT CRender_Box::CreateVtxBuff()
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
		pVtx[wNumVtx].pos = VEC3_INIT;

		// �@���x�N�g��
		pVtx[wNumVtx].nor = VEC3_INIT;

		// ���_�F�̐ݒ�
		pVtx[wNumVtx].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[wNumVtx].tex = VEC2_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// �C���f�b�N�X���̐���
//============================================================================
HRESULT CRender_Box::CreateIdxBuff()
{
	// �C���f�b�N�X�o�b�t�@�̐���
	CRenderer::GetRenderer()->GetDeviece()->CreateIndexBuffer(sizeof(WORD) * NUM_IDX,
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
	WORD* pIdx = nullptr;

	// �C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, reinterpret_cast<void**>(&pIdx), 0);

	// X���ɕ��s�Ȑ�������
	for (WORD i = 0; i < 8; ++i, pIdx++)
	{
		pIdx[0] = i;
	}

	// Y���ɕ��s�Ȑ�������
	for (WORD i = 0, j = 0; i < 4; ++i, pIdx += 2)
	{
		if (i == 2) j += 2;	// 0, 1 ,4 ,5��ӂ̎n�_�ɂ��邽��3, 4�̃C���f�b�N�X���΂�

		pIdx[0] = i + j;
		pIdx[1] = i + 2 + j;
	}

	// Z���ɕ��s�Ȑ�������
	for (WORD i = 0; i < 4; ++i, pIdx += 2)
	{
		pIdx[0] = i;
		pIdx[1] = i + 4;
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//============================================================================
// ���_�̐ݒ�
//============================================================================
void CRender_Box::SetVtx()
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N�����_��񎞂ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ��]�s���p���Ē��_���W���Z�b�g
	for (WORD wCntVtx = 0; wCntVtx < NUM_VTX; ++wCntVtx)
	{
		pVtx[wCntVtx].pos = utility::RotateBoxAroundY(wCntVtx, m_SyncRot.y, m_SyncSize);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}