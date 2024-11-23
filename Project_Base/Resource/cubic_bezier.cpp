//============================================================================
//
// �O���x�W�F�Ȑ� [cubic_bezier.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "cubic_bezier.h"
#include "quadratic_bezier.h"
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
// �񎟋O�Փn���R���X�g���N�^
//============================================================================
CCubic_Bezier::CCubic_Bezier(CObject_X* p1, CObject_X* p2) :
	m_pVtxBuff{ nullptr },
	m_fParameter{ 0.0f },
	m_pQuadratic_Trajectory{ p1, p2 },
	m_pCubic_Trajectory{ nullptr }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CCubic_Bezier::~CCubic_Bezier()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CCubic_Bezier::Init()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// �O���O�Ղ̐���
	m_pCubic_Trajectory = CObject_X::Create();
	m_pCubic_Trajectory->BindModel(CX_Manager::TYPE::SPHERE);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CCubic_Bezier::Uninit()
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
void CCubic_Bezier::Update()
{
	// �񎟋O�Փ��m�����Ԑ���ɁA�O���O�Ղ�\��
	m_pCubic_Trajectory->SetPos(m_pQuadratic_Trajectory[0]->GetPos() + (m_pQuadratic_Trajectory[1]->GetPos() - m_pQuadratic_Trajectory[0]->GetPos()) * m_fParameter);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�𐧌�_�Ԃ̋O�Տ�ɐݒ�
	for (WORD i = 0; i < NUM_CONTROLPOINT - 1; ++i)
	{
		pVtx[i].pos = m_pQuadratic_Trajectory[i]->GetPos();
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �i�s�x�̕ϓ�
	m_fParameter < 1.0f ? m_fParameter += MOVE_SPEED : m_fParameter = 0.0f;
}

//============================================================================
// �`�揈��
//============================================================================
void CCubic_Bezier::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���C�g���I�t
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, nullptr);

	// ���̕`��
	pDev->DrawPrimitive(D3DPT_LINESTRIP,	// �v���~�e�B�u�̎��
		0,									// ���_���̐擪�A�h���X
		1);									// �v���~�e�B�u��

	// ���C�g���I��
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�o�b�t�@�𐶐�
//============================================================================
HRESULT CCubic_Bezier::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,
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

	for (WORD i = 0; i < 2; ++i)
	{
		// ���_���W��ݒ�
		pVtx[i].pos = VEC3_INIT;

		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = VEC3_INIT;

		// ���_�F�̐ݒ�
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = VEC2_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}