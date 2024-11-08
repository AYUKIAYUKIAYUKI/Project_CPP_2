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
#include "object_X.h"

// �f�o�C�X�擾�p
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
// �`��D��x�w��R���X�g���N�^
//============================================================================
CRender_Collision::CRender_Collision(LAYER Priority) :
	CObject{ Priority },
	m_pRef{ nullptr },
	m_pVtxBuff{ nullptr },
	m_pIdxBuff{ nullptr },
	m_Col{ XCOL_INIT }
{
	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CRender_Collision::~CRender_Collision()
{

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
}

//============================================================================
// �`�揈��
//============================================================================
void CRender_Collision::Draw()
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
// �ΏۃI�u�W�F�N�g�̐ݒ�
//============================================================================
void CRender_Collision::SetRefObj(CObject_X* pRef)
{
	m_pRef = pRef;
}

//============================================================================
// ����
//============================================================================
CRender_Collision* CRender_Collision::Create(CObject_X* pRef)
{
	// ����\���𐶐�
	CRender_Collision* pRender_Collision = DBG_NEW CRender_Collision(LAYER::FRONT);

	// �������s
	if (pRender_Collision == nullptr)
	{
		assert(false && "����\���̐����Ɏ��s���܂���");
	}

	// ����\���̏����ݒ�
	pRender_Collision->Init();

	// �ΏۃI�u�W�F�N�g�̐ݒ�
	pRender_Collision->SetRefObj(pRef);

	return pRender_Collision;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�o�b�t�@�̐���
//============================================================================
HRESULT CRender_Collision::CreateVtxBuff()
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
HRESULT CRender_Collision::CreateIdxBuff()
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
void CRender_Collision::SetVtx()
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N�����_��񎞂ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// �T�C�Y���R�s�[
	const Vec3& Size = m_pRef->GetSize();

	// �������R�s�[
	const Vec3& Rot = m_pRef->GetRot();

	// ��]�s����`
	struct Mtx_Rot
	{
		float _11, _13, _31, _33;
	};

	// ��]�s����쐬
	Mtx_Rot MtxRot = 
	{
		0.0f, 0.0f, 0.0f, 0.0f
	};

	// ���������]�s����쐬
	MtxRot._11 = cosf(Rot.y);
	MtxRot._13 = sinf(Rot.y);
	MtxRot._31 = -sinf(Rot.y);
	MtxRot._33 = cosf(Rot.y);

	// ��]���X�EZ���_���W�̍쐬
	float VtxPosX = MtxRot._11 + MtxRot._31;
	float VtxPosZ = MtxRot._31 + MtxRot._33;

	VtxPosX *= Size.x;
	VtxPosZ *= Size.z;

	// ���_���W��ݒ�
#if 0
	pVtx[0].pos = { VtxPosX, +Size.y, -Size.z };
	pVtx[1].pos = { VtxPosX, +Size.y, -Size.z };
	pVtx[2].pos = { VtxPosX, -Size.y, -Size.z };
	pVtx[3].pos = { VtxPosX, -Size.y, -Size.z };
	pVtx[4].pos = { VtxPosX, +Size.y, +Size.z };
	pVtx[5].pos = { VtxPosX, +Size.y, +Size.z };
	pVtx[6].pos = { VtxPosX, -Size.y, +Size.z };
	pVtx[7].pos = { VtxPosX, -Size.y, +Size.z };
#elif 0
	pVtx[0].pos = { -VtxPosX, +Size.y, -VtxPosZ };
	pVtx[1].pos = { +VtxPosX, +Size.y, -VtxPosZ };
	pVtx[2].pos = { -VtxPosX, -Size.y, -VtxPosZ };
	pVtx[3].pos = { +VtxPosX, -Size.y, -VtxPosZ };
	pVtx[4].pos = { -VtxPosX, +Size.y, +VtxPosZ };
	pVtx[5].pos = { +VtxPosX, +Size.y, +VtxPosZ };
	pVtx[6].pos = { -VtxPosX, -Size.y, +VtxPosZ };
	pVtx[7].pos = { +VtxPosX, -Size.y, +VtxPosZ };
#elif 1
	// ��]�p�x�Ɋ�Â� cos �� sin ���v�Z
	float cosY = cosf(-Rot.y);
	float sinY = sinf(-Rot.y);

	// �e���_�̈ʒu����]��������ɃT�C�Y���|���Ĕz�u
	pVtx[0].pos = { (-cosY * Size.x - sinY * Size.z), +Size.y, (-sinY * Size.x + cosY * Size.z) };
	pVtx[1].pos = { (cosY * Size.x - sinY * Size.z), +Size.y, (sinY * Size.x + cosY * Size.z) };
	pVtx[2].pos = { (-cosY * Size.x - sinY * Size.z), -Size.y, (-sinY * Size.x + cosY * Size.z) };
	pVtx[3].pos = { (cosY * Size.x - sinY * Size.z), -Size.y, (sinY * Size.x + cosY * Size.z) };

	pVtx[4].pos = { (-cosY * Size.x + sinY * Size.z), +Size.y, (-sinY * Size.x - cosY * Size.z) };
	pVtx[5].pos = { (cosY * Size.x + sinY * Size.z), +Size.y, (sinY * Size.x - cosY * Size.z) };
	pVtx[6].pos = { (-cosY * Size.x + sinY * Size.z), -Size.y, (-sinY * Size.x - cosY * Size.z) };
	pVtx[7].pos = { (cosY * Size.x + sinY * Size.z), -Size.y, (sinY * Size.x - cosY * Size.z) };
#endif

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
	D3DXMatrixIdentity(&m_MtxWorld);

	// ���s�ړ��s��쐬
	D3DXMatrixTranslation(&mtxTrans,
		m_pRef->GetPos().x,
		m_pRef->GetPos().y,
		m_pRef->GetPos().z);

	// ���s�ړ��s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);
}