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
#include "motion_set.h"
#include "field_manager.h"
#include "renderer.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
#define DRAW_DEBUG_LINE 1	// �f�o�b�O�p�̃��C����`�悷��

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************

// ��b�p�����[�^�̓W�J
const JSON CFan::m_InitParam = utility::OpenJsonFile("Data\\JSON\\fan_parameter.json");

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
#if DRAW_DEBUG_LINE	// �f�o�b�O�p�̃��C����`�悷��
	// ���_���W�̐ݒ�
	SetVtx();

	// ���[���h�s��ݒ�
	SetMtxWorld();
#endif // DRAW_DEBUG_LINE

	// ���p�ɍ��킹�Ĕ͈͕��̕����x�N�g����2�{�쐬
	m_DirVec[0] = { cosf(m_fDirection + m_fRange), 0, sinf(m_fDirection + m_fRange) };
	m_DirVec[1] = { cosf(m_fDirection - m_fRange), 0, sinf(m_fDirection - m_fRange) };
}

//============================================================================
// �`�揈��
//============================================================================
void CFan::Draw()
{
#if DRAW_DEBUG_LINE	// �f�o�b�O�p�̃��C����`�悷��
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
		NUM_VEC);							// �v���~�e�B�u��

	// ���C�g���I��
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
#endif // �f�o�b�O�p�̃��C����`�悷��
}

//============================================================================
// ��`�͈͓��ɂ��邩���o
//============================================================================
bool CFan::DetectInFanRange(D3DXVECTOR3 Pos)
{
	// �Ώە��ւ̃x�N�g�����o��
	Vec3 OtherVec = Pos - m_Pos;

	// �Ώە��ւ̃x�N�g���𐳋K��
	D3DXVec3Normalize(&OtherVec, &OtherVec);
	
	// �Е��̐�`�����̃x�N�g���ƑΏە��ւ̕����x�N�g���̊O�ς��쐬
	float fCross1 = m_DirVec[0].x * OtherVec.z - m_DirVec[0].z * OtherVec.x;

	// ���̎��_��1�ڂ̕����x�N�g���Ƃ̊O�ό��ʂ����̊p�x�Ȃ�͈͊O
	if (fCross1 > 0.0f)
	{
		return 0;
	}

	// �����Е��̊O�ς��쐬
	float fCross2 = m_DirVec[1].x * OtherVec.z - m_DirVec[1].z * OtherVec.x;

	// 2�ڂ̕����x�N�g���Ƃ̊O�ό��ʂ����̊p�x�Ȃ�͈͊O
	if (fCross2 < 0.0f)
	{
		return 0;
	}

	// �����𖞂����Ă���ΐ�`�͈͓̔�
	return 1;
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

	// �������s
	if (!pNewInstance) throw std::bad_alloc();

	// �����ݒ�
	pNewInstance->Init();
	
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
	m_DirVec{ VEC3_INIT, VEC3_INIT },
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
	// �O�̂��ߏI������
	Uninit();
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CFan::Init()
{
#if DRAW_DEBUG_LINE	// �f�o�b�O�p�̃��C����`��
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}
#endif // DRAW_DEBUG_LINE

	// �f�[�^���L���X�g
	float
		fDirection = static_cast<float>(m_InitParam["Direction"]),
		fRange = static_cast<float>(m_InitParam["Range"]);
		
	// ��`�̃p�����[�^���Z�b�g
	m_fDirection = fDirection;	// ���p
	m_fRange = fRange;			// �͈�

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

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = VEC2_INIT;
	}

	// ���_�F�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CFan::Uninit()
{
#if DRAW_DEBUG_LINE
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
#endif // DRAW_DEBUG_LINE
}

//============================================================================
// ���_���W�̐ݒ�
//============================================================================
void CFan::SetVtx()
{
	// ���p�ɍ��킹�Ĕ͈͕��̕����x�N�g����2�{�쐬
	m_DirVec[0] = { cosf(m_fDirection + m_fRange), 0, sinf(m_fDirection + m_fRange) };
	m_DirVec[1] = { cosf(m_fDirection - m_fRange), 0, sinf(m_fDirection - m_fRange) };

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);
	
	// ���_���W�̐ݒ�
	pVtx[0].pos = m_DirVec[0] * 500.0f;
	pVtx[1].pos = m_Pos;
	pVtx[2].pos = m_DirVec[1] * 500.0f;

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