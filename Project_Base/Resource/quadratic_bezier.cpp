//============================================================================
//
// �񎟃x�W�F�Ȑ� [quadratic_bezier.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
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
// ���W�n���R���X�g���N�^
//============================================================================
CQuadratic_Bezier::CQuadratic_Bezier(const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Pos3) :
	m_pVtxBuff{ nullptr },
	m_fParameter{ 0.0f }
{
	// ����_�̏�����
	m_ControlPoint[0] = Pos1;
	m_ControlPoint[1] = Pos2;
	m_ControlPoint[2] = Pos3;

	// �����ڂ̏�����
	for (int i = 0; i < NUM_VISUAL; ++i)
	{
		m_pParameterVisual[i] = nullptr;
	}
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CQuadratic_Bezier::~CQuadratic_Bezier()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CQuadratic_Bezier::Init()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

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

	for (int i = 0; i < 2; i++)
	{
		// ���_���W��ݒ�
		pVtx[i].pos = VEC3_INIT;

		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = VEC3_INIT;

		// ���_�F�̐ݒ�
		pVtx[i].col = XCOl_INIT;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = VEC2_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	for (int i = 0; i < NUM_VISUAL; ++i)
	{
		m_pParameterVisual[i] = CObject_X::Create();
		m_pParameterVisual[i]->BindModel(CModel_X_Manager::TYPE::SPHERE);
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CQuadratic_Bezier::Uninit()
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
void CQuadratic_Bezier::Update()
{
	Vec3 MovePos1 = m_ControlPoint[0] + (m_ControlPoint[1] - m_ControlPoint[0]) * m_fParameter;
	m_pParameterVisual[0]->SetPos(MovePos1);

	Vec3 MovePos2 = m_ControlPoint[1] + (m_ControlPoint[2] - m_ControlPoint[1]) * m_fParameter;
	m_pParameterVisual[1]->SetPos(MovePos2);

	Vec3 MovePos3 = MovePos1 + (MovePos2 - MovePos1) * m_fParameter;
	m_pParameterVisual[2]->SetPos(MovePos3);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W��ݒ�
	pVtx[0].pos = MovePos1;
	pVtx[1].pos = MovePos2;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �i�s�x�̕ϓ�
	m_fParameter < 1.0f ? m_fParameter += 0.01f : m_fParameter = 0.0f;
}

//============================================================================
// �`�揈��
//============================================================================
void CQuadratic_Bezier::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetInstance()->GetDeviece();

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