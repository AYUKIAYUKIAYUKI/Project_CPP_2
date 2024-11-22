//============================================================================
//
// ��` [fan.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "fan.h"
#include "field_manager.h"

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
// �j��
//============================================================================
void CFan::Release()
{
	// ���g�̔j��
	if (this != nullptr)
	{
		this->Uninit();	// �I������
		delete this;	// �����������
	}
}

//============================================================================
// �X�V����
//============================================================================
void CFan::Update()
{
	// ���_���W�̐ݒ�
	SetVtx();

	// ���[���h�s��ݒ�
	SetMtxWorld();

#ifdef _DEBUG

	CRenderer::SetDebugString("��������������������������������������������������������������");;
	CRenderer::SetDebugString("��`�̕��p�F" + to_string(m_fDirection));
	CRenderer::SetDebugString("��`�͈̔́F" + to_string(m_fRange));
	CRenderer::SetDebugString("��������������������������������������������������������������");

#endif // _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CFan::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

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
		NUM_PRIM);							// �v���~�e�B�u��

	// ���C�g���I��
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================================
// ��`�͈͓��ɂ��邩���o
//============================================================================
bool CFan::DetectInFanRange(D3DXVECTOR3 Pos)
{
	return 0;
}

//============================================================================
// ���W���擾
//============================================================================
const D3DXVECTOR3& CFan::GetPos() const
{
	return m_Pos;
}

//============================================================================
// ���W��ݒ�
//============================================================================
void CFan::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// ���p���擾
//============================================================================
const float& CFan::GetDirection() const
{
	return m_fDirection;
}

//============================================================================
// ���p��ݒ�
//============================================================================
void CFan::SetDirection(float fDirection)
{
	m_fDirection = fDirection;
}

//============================================================================
// �͈͂��擾
//============================================================================
const float& CFan::GetRange() const
{
	return m_fRange;
}

//============================================================================
// �͈͂�ݒ�
//============================================================================
void CFan::SetRange(float fRange)
{
	m_fRange = fRange;
}

//============================================================================
// ����
//============================================================================
CFan* CFan::Create()
{
	CFan* pNewInstance = DBG_NEW CFan();

	if (!pNewInstance)
	{
		assert(false && "��`�C���X�^���X�̐����Ɏ��s");
	}

	// �����ݒ�
	pNewInstance->Init();
	
	// JSON�t�@�C����ǂݎ��W�J
	std::ifstream ifs("Data\\JSON\\fan_parameter.json");

	// �t�@�C�����W�J�o���Ă�����
	if (ifs.good())
	{
		// JSON�f�[�^���p�[�X
		JSON Json;
		ifs >> Json;

		// �e��p�����[�^���f�V���A���C�Y
		pNewInstance->SetDirection(Json["Direction"]);
		pNewInstance->SetRange(Json["Range"]);
	}
	else
	{
		assert(false && "spline_test.json�̓ǂݎ��Ɏ��s���܂���");
	}

	return pNewInstance;
}

//============================================================================
// ����
//============================================================================
CFan* CFan::Create(D3DXVECTOR3 Pos, float fDirection, float fRange)
{
	CFan* pNewInstance = DBG_NEW CFan();

	if (!pNewInstance)
	{
		assert(false && "��`�C���X�^���X�̐����Ɏ��s");
	}

	// �����ݒ�
	pNewInstance->Init();

	// �p�����[�^�𔽉f
	pNewInstance->SetPos(Pos);
	pNewInstance->SetDirection(fDirection);
	pNewInstance->SetRange(fRange);

	return pNewInstance;
}

//============================================================================
//
// private�����o
//
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CFan::CFan() :
	m_pVtxBuff{ nullptr },
	m_Pos{ VEC3_INIT },
	m_fDirection{ 0.0f },
	m_fRange{ 0.0f }
{
	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CFan::~CFan()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CFan::Init()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// ��`�͈̔͂�ݒ�
	m_fRange = D3DX_PI * 0.1f;

	return S_OK;
}

//============================================================================
// ���_�o�b�t�@�̐���
//============================================================================
HRESULT CFan::CreateVtxBuff()
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

	for (WORD i = 0; i < NUM_VTX; ++i)
	{
		// ���_���W��ݒ�
		pVtx[i].pos = VEC3_INIT;

		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = VEC3_INIT;

		// ���_�F�̐ݒ�
		pVtx[i].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = VEC2_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CFan::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//============================================================================
// ���_���W�̐ݒ�
//============================================================================
void CFan::SetVtx()
{
	// ���p�ɍ��킹�Ĕ͈͕��̕����x�N�g����2�{�쐬
	Vec3 Vec[2] = { 
		Vec3(cosf(m_fDirection + m_fRange), 0, sin(m_fDirection + m_fRange)),
		Vec3(cosf(m_fDirection - m_fRange), 0, sin(m_fDirection - m_fRange))
	};

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);
	
	// ���_���W�̐ݒ�
	pVtx[0].pos = Vec[0] * CField_Manager::FIELD_RADIUS;
	pVtx[1].pos = m_Pos;
	pVtx[2].pos = Vec[1] * CField_Manager::FIELD_RADIUS;
	
	// �����ɂ���
	for (int i = 0; i < 2; ++i)
	CRenderer::SetDebugString("Vec" + to_string(i) + "�F" + to_string(Vec[i].x) + "�F" + to_string(Vec[i].z));

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CFan::SetMtxWorld()
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