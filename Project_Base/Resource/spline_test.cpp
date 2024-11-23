//============================================================================
//
// �X�v���C���Ȑ��e�X�g [spline_test.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "spline_test.h"
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
CSpline_Test::CSpline_Test() :
	m_pVtxBuff{ nullptr },
	m_nNumVtx{ 0 },
	m_nNumPrim{ 0 },
	m_Pos{ VEC3_INIT },
	m_pQuadratic_Bezier{ nullptr, nullptr }
{
	// JSON�t�@�C����ǂݎ��W�J
	std::ifstream ifs("Data\\JSON\\spline_test.json");

	// �t�@�C�����W�J�o���Ă�����
	if (ifs.good())
	{
		// JSON�f�[�^���p�[�X
		ifs >> m_Json;

		// �e��p�����[�^���f�V���A���C�Y
		m_nNumVtx = m_Json["Vtx"];
		m_nNumPrim = m_Json["Prim"];
	}
	else
	{
		assert(false && "spline_test.json�̓ǂݎ��Ɏ��s���܂���");
	}

	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CSpline_Test::~CSpline_Test()
{
	// �O�̂��ߏI������
	Uninit();
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CSpline_Test::Init()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// ���W�����f�V���A���C�Y
	const auto& Pos_List = m_Json["Pos_List"];

	for (WORD i = 0; i < NUM_QUADRATIC_BEZIER; ++i)
	{
		// ���W�i�i�[�p�R���e�i
		std::array<Vec3, CQuadratic_Bezier::NUM_CONTROLPOINT> ControlPoint;

		for (WORD j = 0; j < CQuadratic_Bezier::NUM_CONTROLPOINT; ++j)
		{
			// ����_�̍쐬
			const auto& Pos = Pos_List[j + i];				// �v�f�𔲂��o����
			ControlPoint[j] = Vec3(Pos[0], Pos[1], Pos[2]);	// ���W���쐬�����
		}

		// �񎟃x�W�F�Ȑ��p�𐶐�
		m_pQuadratic_Bezier[i] = DBG_NEW CQuadratic_Bezier(ControlPoint);
		m_pQuadratic_Bezier[i]->Init();
	}

	// �O���x�W�F�Ȑ��̐���
	m_pCubic_Bezier = DBG_NEW CCubic_Bezier(m_pQuadratic_Bezier[0]->GetQuadratic_Trajectory(), m_pQuadratic_Bezier[1]->GetQuadratic_Trajectory());
	m_pCubic_Bezier->Init();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CSpline_Test::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �񎟃x�W�F�Ȑ��̔j��
	for (WORD i = 0; i < NUM_QUADRATIC_BEZIER; ++i)
	{
		if (m_pQuadratic_Bezier[i] != nullptr)
		{
			m_pQuadratic_Bezier[i]->Uninit();
			delete m_pQuadratic_Bezier[i];
			m_pQuadratic_Bezier[i] = nullptr;
		}
	}

	// �O���x�W�F�Ȑ��̔j��
	if (m_pCubic_Bezier != nullptr)
	{
		m_pCubic_Bezier->Uninit();
		delete m_pCubic_Bezier;
		m_pCubic_Bezier = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CSpline_Test::Update()
{
	// �񎟃x�W�F�Ȑ��̍X�V
	for (WORD i = 0; i < NUM_QUADRATIC_BEZIER; ++i)
	{
		m_pQuadratic_Bezier[i]->Update();
	}

	// �O���x�W�F�Ȑ��̍X�V
	m_pCubic_Bezier->Update();

	// ���[���h�s��ݒ�
	SetMtxWorld();
}

//============================================================================
// �`�揈��
//============================================================================
void CSpline_Test::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();

	// ���C�g���I�t
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, nullptr);

	// ���̕`��
	pDev->DrawPrimitive(D3DPT_LINESTRIP,	// �v���~�e�B�u�̎��
		0,									// ���_���̐擪�A�h���X
		m_nNumPrim);						// �v���~�e�B�u��

	// ���C�g���I��
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �񎟃x�W�F�Ȑ��̕`��
	for (WORD i = 0; i < NUM_QUADRATIC_BEZIER; ++i)
	{
		m_pQuadratic_Bezier[i]->Draw();
	}

	// �O���x�W�F�Ȑ��̕`��
	m_pCubic_Bezier->Draw();
}

//============================================================================
//
// private�����o
//
//============================================================================

//============================================================================
// ���_�o�b�t�@�𐶐�
//============================================================================
HRESULT CSpline_Test::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();

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
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���W�����f�V���A���C�Y
	const auto& Pos_List = m_Json["Pos_List"];

	for (WORD i = 0; i < m_nNumVtx; ++i)
	{
		// ���_���W�̐ݒ�
		const auto& Pos = Pos_List[i];				// �v�f�𔲂��o����
		pVtx[i].pos = Vec3(Pos[0], Pos[1], Pos[2]);	// ���W���쐬�����

		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = VEC3_INIT;

		// ���_�F�̐ݒ�
		pVtx[i].col = XCOL_INIT;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = VEC2_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CSpline_Test::SetMtxWorld()
{
	// �v�Z�p�s��
	Mtx mtxRot, mtxTrans;

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

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