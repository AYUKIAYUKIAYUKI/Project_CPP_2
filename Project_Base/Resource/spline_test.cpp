//============================================================================
//
// �X�v���C���Ȑ��H�̃e�X�g [spline_test.cpp]
// Author : ���c����
//
//============================================================================

#include "spline_test.h"
#include "renderer.h"

// �����ݒ�
void CSpline::Init() {
	// JSON�t�@�C����ǂݎ��W�J
	std::ifstream ifs("Data\\JSON\\spline_test.json");

	// �t�@�C�����W�J�o���Ă�����
	if (ifs.good())
	{
		// �V���A���C�Y��
		ifs >> m_json;

		// �p�����[�^��ǂݍ���
		m_nNumVtx = m_json["Vtx"];
		m_nNumIdx = m_json["Idx"];
		m_nNumPrim = m_json["Prim"];
	}
	else
	{
		assert(false && "spline_test.json�̓ǂݎ��Ɏ��s���܂���");
	}

	// ���_�𐶐�
	CreateVtxBuff();

	// �C���f�b�N�X�𐶐�
	CreateIdxBuff();
}

// �I������
void CSpline::Uninit() {
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

// �X�V����
void CSpline::Update() {

}

// �`�揈��
void CSpline::Draw() {
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

	//// �{�b�N�X�̕`��
	//pDev->DrawIndexedPrimitive(D3DPT_LINELIST,
	//	0,
	//	0,
	//	m_nNumVtx,			// ���_��
	//	0,
	//	m_nNumPrimitive);	// �ӂ̐�

	// �X�v���C���̕`��
	pDev->DrawPrimitive(D3DPT_LINELIST,	// �v���~�e�B�u�̎��
		0,								// ���_���̐擪�A�h���X
		m_nNumPrim);					// �v���~�e�B�u��

	// ���C�g���I��
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

// ���_�𐶐�
HRESULT CSpline::CreateVtxBuff() {
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

	auto Pos_List = m_json["Pos_List"];

	for (int i = 0; i < m_nNumVtx; i++)
	{
		// ���W�̐ݒ�
		float X = Pos_List[i][0], Y = Pos_List[i][1], Z = Pos_List[i][2];
		pVtx[i].pos = D3DXVECTOR3(X, Y, Z);

		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = { 0.0f, 0.0f, 0.0f };

		// �F�̐ݒ�
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`���̐ݒ�
		pVtx[i].tex = { 0.0f, 0.0f };
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

// �C���f�b�N�X�𐶐�
HRESULT CSpline::CreateIdxBuff(){
	return S_OK;
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CSpline::SetMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxRot, mtxTrans;

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