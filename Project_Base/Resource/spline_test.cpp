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
// �f�t�H���g�R���X�g���N�^
//============================================================================
CSpline_Test::CSpline_Test() :
	m_pVtxBuff{ nullptr },
	m_nNumVtx{ 0 },
	m_nNumPrim{ 0 },
	m_pIdxBuff{ nullptr },
	m_nNumIdx{ 0 },
	m_Pos{ VEC3_INIT },
	m_QuadraticBezier{ nullptr, nullptr, {VEC3_INIT, VEC3_INIT}, VEC3_INIT, 0.0f }
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
		m_nNumIdx = m_Json["Idx"];
		m_nNumPrim = m_Json["Prim"];
	}
	else
	{
		assert(false && "spline_test.json�̓ǂݎ��Ɏ��s���܂���");
	}

	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

#if 0
	// �����𐶐�
	m_QuadraticBezier.pObject = CObject_X::Create();
	const auto& Pos_List = m_Json["Pos_List"];
	const float& X = Pos_List[m_nMoveIdx][0], Y = Pos_List[m_nMoveIdx][1], Z = Pos_List[m_nMoveIdx][2];
	m_pMoving->SetPos(D3DXVECTOR3(X, Y, Z));
	m_pMoving->BindModel(CModel_X_Manager::TYPE::SAMUS);
#else
	// �x�W�F�Ȑ��p�̕��̂𐶐�
	m_QuadraticBezier.pObject = CObject_X::Create();
	m_QuadraticBezier.pObject->SetPos(VEC3_INIT);
	m_QuadraticBezier.pObject->BindModel(CModel_X_Manager::TYPE::SAMUS);

	// ���W�����f�V���A���C�Y
	const auto& Pos_List = m_Json["Pos_List"];
	
	{
		const auto& Pos = Pos_List[0];
		m_QuadraticBezier.PinPos[0] = Vec3(Pos[0], Pos[1], Pos[2]);
	}

	{
		const auto& Pos = Pos_List[1];
		m_QuadraticBezier.MagPos = Vec3(Pos[0], Pos[1], Pos[2]);
	}

	{
		const auto& Pos = Pos_List[2];
		m_QuadraticBezier.PinPos[1] = Vec3(Pos[0], Pos[1], Pos[2]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_QuadraticBezier.pCoefObj[i] = CObject_X::Create();
		m_QuadraticBezier.pCoefObj[i]->SetPos(VEC3_INIT);
		m_QuadraticBezier.pCoefObj[i]->BindModel(CModel_X_Manager::TYPE::SPHERE);
	}
#endif
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
void CSpline_Test::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// ���_�o�b�t�@�̔j��
	if (m_QuadraticBezier.pVtxBuff != nullptr)
	{
		m_QuadraticBezier.pVtxBuff->Release();
		m_QuadraticBezier.pVtxBuff = nullptr;
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
void CSpline_Test::Update()
{

#if 0
	// ���W�����f�V���A���C�Y
	const auto& Pos_List = m_Json["Pos_List"];

	// �ߋ��̃C���f�b�N�X�A�ڕW�̃C���f�b�N�X����n�_�𔲂��o��
	const auto& OldPos_Param = Pos_List[m_nMoveIdx],
		Dest_Param = Pos_List[m_nMoveIdx + 1];
	Vec3 DestPos  = D3DXVECTOR3(Dest_Param[0], Dest_Param[1], Dest_Param[2]),
		OldPos = D3DXVECTOR3(OldPos_Param[0], OldPos_Param[1], OldPos_Param[2]),
		NewPos = m_pMoving->GetPos();

	// �ڕW�n�_�Ɉړ�
	NewPos += (DestPos - OldPos) / MAX_MOVE_COUNT;
	m_pMoving->SetPos(NewPos);

	// �����Ԍo�߂ŖڕW���X�V
	m_nMoveCnt++;
	if (m_nMoveCnt > MAX_MOVE_COUNT) {
		m_nMoveIdx < (m_nNumVtx - 1) - 1 ? m_nMoveIdx++ : m_nMoveIdx = 0;
		m_nMoveCnt = 0;
	}

#ifdef _DEBUG
	CRenderer::GetInstance()->SetDebugString("�ړ����� : " + to_string(m_nMoveIdx) + " -> " + to_string(m_nMoveIdx + 1));
	CRenderer::GetInstance()->SetDebugString("���ݒn �@: " + to_string(NewPos.x) + to_string(NewPos.y) + to_string(NewPos.z));
	CRenderer::GetInstance()->SetDebugString("�ړI�n �@: " + to_string(DestPos.x) + to_string(DestPos.y) + to_string(DestPos.z));
#endif // _DEBUG
#else 

	// �x�W�F�Ȑ����v�Z
	CalcBezier();

	CRenderer::GetInstance()->SetDebugString("�s�����W1�F" + to_string(m_QuadraticBezier.PinPos[0].x) + " : " + to_string(m_QuadraticBezier.PinPos[0].y) + " : " + to_string(m_QuadraticBezier.PinPos[0].z));
	CRenderer::GetInstance()->SetDebugString("���΍��W �F" + to_string(m_QuadraticBezier.MagPos.x) + " : " + to_string(m_QuadraticBezier.MagPos.y) + " : " + to_string(m_QuadraticBezier.MagPos.z));
	CRenderer::GetInstance()->SetDebugString("�s�����W2�F" + to_string(m_QuadraticBezier.PinPos[1].x) + " : " + to_string(m_QuadraticBezier.PinPos[1].y) + " : " + to_string(m_QuadraticBezier.PinPos[1].z));
	CRenderer::GetInstance()->SetDebugString("�ړ����� �F" + to_string(m_QuadraticBezier.fCoef));

#endif
}

//============================================================================
// �`�揈��
//============================================================================
void CSpline_Test::Draw()
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
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, nullptr);

#if 0

	// �C���f�b�N�X������̕`��
	pDev->DrawIndexedPrimitive(D3DPT_LINESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		m_nNumVtx,								// ���_��
		0,
		m_nNumPrim);							// �v���~�e�B�u��
#else

	// ���̕`��
	pDev->DrawPrimitive(D3DPT_LINESTRIP,	// �v���~�e�B�u�̎��
		0,									// ���_���̐擪�A�h���X
		m_nNumPrim);						// �v���~�e�B�u��

#endif

	// Bezier
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDev->SetStreamSource(0, m_QuadraticBezier.pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���̕`��
		pDev->DrawPrimitive(D3DPT_LINESTRIP,	// �v���~�e�B�u�̎��
			0,									// ���_���̐擪�A�h���X
			1);									// �v���~�e�B�u��
	}

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
HRESULT CSpline_Test::CreateVtxBuff()
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
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���W�����f�V���A���C�Y
	const auto& Pos_List = m_Json["Pos_List"];

	for (int i = 0; i < m_nNumVtx; i++)
	{
#if 0
		// ���_���W�̐ݒ�
		const float& X = Pos_List[i][0], Y = Pos_List[i][1], Z = Pos_List[i][2];	// �v�f�𔲂��o����
		pVtx[i].pos = D3DXVECTOR3(X, Y, Z);											// ���W���쐬�����
#elif 1
		const auto& Pos = Pos_List[i];				// �v�f�𔲂��o����
		pVtx[i].pos = Vec3(Pos[0], Pos[1], Pos[2]);	// ���W���쐬�����
#endif

		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = VEC3_INIT;

		// ���_�F�̐ݒ�
		pVtx[i].col = XCOl_INIT;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = VEC2_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// Bezier
	{
		// ���_�o�b�t�@�̐���
		pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_QuadraticBezier.pVtxBuff,
			nullptr);

		if (m_QuadraticBezier.pVtxBuff == nullptr)
		{ // �������s
			return E_FAIL;
		}

		// ���_�o�b�t�@�����b�N
		m_QuadraticBezier.pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

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
		m_QuadraticBezier.pVtxBuff->Unlock();
	}

	return S_OK;
}

//============================================================================
// �C���f�b�N�X�o�b�t�@�𐶐�
//============================================================================
HRESULT CSpline_Test::CreateIdxBuff()
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
	WORD* pIdx = nullptr;

	// �C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, reinterpret_cast<void**>(&pIdx), 0);

	// �C���f�b�N�X��ݒ�
	for (int i = 0; i < m_nNumIdx; i++)
	{
		pIdx[i] = static_cast<WORD>(i);
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//============================================================================
// �x�W�F�Ȑ����v�Z
//============================================================================
void CSpline_Test::CalcBezier()
{
	// �s�� -> ���ΊԂ̋������ړ���������
	m_QuadraticBezier.fCoef += 0.01f;

	Vec3 MovePos1 = m_QuadraticBezier.PinPos[0] + (m_QuadraticBezier.MagPos - m_QuadraticBezier.PinPos[0]) * m_QuadraticBezier.fCoef;
	m_QuadraticBezier.pCoefObj[0]->SetPos(MovePos1);

	Vec3 MovePos2 = m_QuadraticBezier.MagPos + (m_QuadraticBezier.PinPos[1] - m_QuadraticBezier.MagPos) * m_QuadraticBezier.fCoef;
	m_QuadraticBezier.pCoefObj[1]->SetPos(MovePos2);

	Vec3 MovePos3 = MovePos1 + (MovePos2 - MovePos1) * m_QuadraticBezier.fCoef;
	m_QuadraticBezier.pCoefObj[2]->SetPos(MovePos3);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_QuadraticBezier.pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W��ݒ�
	pVtx[0].pos = MovePos1;
	pVtx[1].pos = MovePos2;

	// ���_�o�b�t�@���A�����b�N����
	m_QuadraticBezier.pVtxBuff->Unlock();

	if (m_QuadraticBezier.fCoef > 1.0f)
	{
		m_QuadraticBezier.fCoef = 0.0f;
	}
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