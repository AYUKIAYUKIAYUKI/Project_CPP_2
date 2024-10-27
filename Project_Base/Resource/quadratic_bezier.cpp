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
	// ����_�̐ݒ�
	m_ControlPoint[0] = Pos1;
	m_ControlPoint[1] = Pos2;
	m_ControlPoint[2] = Pos3;

	// �O�Ղ̏�����
	for (WORD i = 0; i < NUM_TRAJECTORY; ++i)
	{
		m_pTrajectory[i] = nullptr;
	}
}

//============================================================================
// ���W�n���R���X�g���N�^
//============================================================================
CQuadratic_Bezier::CQuadratic_Bezier(const std::array<D3DXVECTOR3, NUM_CONTROLPOINT>& ControlPoint) :
	m_pVtxBuff{ nullptr },
	m_fParameter{ 0.0f }
{
	// ����_�̐ݒ�
	for (WORD i = 0; i < NUM_CONTROLPOINT; ++i)
	{
		m_ControlPoint[i] = ControlPoint[i];
	}

	// �O�Ղ̏�����
	for (WORD i = 0; i < NUM_TRAJECTORY; ++i)
	{
		m_pTrajectory[i] = nullptr;
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
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// �O�Ղ̐���
	for (WORD i = 0; i < NUM_TRAJECTORY; ++i)
	{
		m_pTrajectory[i] = CObject_X::Create();
		m_pTrajectory[i]->BindModel(CModel_X_Manager::TYPE::SPHERE);
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
	for (WORD i = 0; i < NUM_CONTROLPOINT - 1; ++i)
	{
		// �i�s�x�ɍ��킹�āA����_�ԏ�̋O�Ղ̍��W���X�V
		m_pTrajectory[i]->SetPos(m_ControlPoint[i] + (m_ControlPoint[i + 1] - m_ControlPoint[i]) * m_fParameter);
	}

	// �O�Փ��m�����Ԑ���ɁA����Ȃ�O�Ղ�\��
	m_pTrajectory[NUM_CONTROLPOINT - 1]->SetPos(m_pTrajectory[0]->GetPos() + (m_pTrajectory[1]->GetPos() - m_pTrajectory[0]->GetPos()) * m_fParameter);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�𐧌�_�Ԃ̋O�Տ�ɐݒ�
	for (WORD i = 0; i < NUM_CONTROLPOINT - 1; ++i)
	{
		pVtx[i].pos = m_pTrajectory[i]->GetPos();
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �i�s�x�̕ϓ�
	m_fParameter < 1.0f ? m_fParameter += MOVE_SPEED : m_fParameter = 0.0f;

#if 0
	for (WORD i = 0; i < NUM_CONTROLPOINT; ++i)
	{
		CRenderer::GetInstance()->SetDebugString(to_string(m_pTrajectory[i]->GetPos().x) + " : " + to_string(m_pTrajectory[i]->GetPos().y) + " : " + to_string(m_pTrajectory[i]->GetPos().z));
	}

	for (WORD i = 0; i < NUM_CONTROLPOINT; ++i)
	{
		CRenderer::GetInstance()->SetDebugString(to_string(m_ControlPoint[i].x) + " : " + to_string(m_ControlPoint[i].y) + " : " + to_string(m_ControlPoint[i].z));
	}
#endif
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

//============================================================================
// �񎟋O�Ղ��擾
//============================================================================
CObject_X* CQuadratic_Bezier::GetQuadratic_Trajectory() const
{
	return m_pTrajectory[2];
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�o�b�t�@�𐶐�
//============================================================================
HRESULT CQuadratic_Bezier::CreateVtxBuff()
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

	for (WORD i = 0; i < 2; ++i)
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

	return S_OK;
}