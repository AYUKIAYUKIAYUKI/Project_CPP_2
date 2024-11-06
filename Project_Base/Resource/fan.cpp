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

	CRenderer::SetDebugString("��������������������������������������������������������������");
	CRenderer::SetDebugString("��`�̕��p : " + to_string(m_fDirection));
	CRenderer::SetDebugString("���p�̃x�N : " + to_string(sinf(m_fDirection)) + " : " + to_string(cosf(m_fDirection)));
	CRenderer::SetDebugString("��`�̒��� : " + to_string(m_fLength));
	CRenderer::SetDebugString("��`�͈̔� : " + to_string(m_fRange));
	CRenderer::SetDebugString("�͈͂̃x�N : " + to_string(cosf(m_fRange)));
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
		1);									// �v���~�e�B�u��

	// ���C�g���I��
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================================
// ��`�͈͓��ɂ��邩���o
//============================================================================
bool CFan::DetectInFanRange(D3DXVECTOR3 Pos)
{
	// ��`�̎n�_���������W�ւ̃x�N�g�����v�Z
	Vec3 VecPoint = Pos - m_Pos;

	// �͈͓��ɑ��݂��Ă��邩�A�����l��؂�l�߂Đ����Ȕ�����s��
	if (static_cast<int>(sqrtf(VecPoint.x * VecPoint.x + VecPoint.z * VecPoint.z)) <= m_fLength)
	{
		// ���W�x�N�g���𐳋K��
		D3DXVec3Normalize(&VecPoint, &VecPoint);

		// ���p��������x�N�g�����v�Z
		Vec3 VecDir = Vec3(sinf(m_fDirection), 0.0f, cosf(m_fDirection));

		// �����x�N�g���𐳋K��
		D3DXVec3Normalize(&VecDir, &VecDir);

		// �����x�N�g���ƍ��W�x�N�g���̓��ς��s���A��͈̔͂�cos�ŋ��߂�
		float f�� = D3DXVec3Dot(&VecDir, &VecPoint);

		// ��͈͓̔��ɑ��݂��Ă����
		if (cosf(m_fRange) <= f�� ||
			cosf(m_fRange) >= f��)
		{
			CRenderer::SetDebugString("���ς̂Ȃ��p : " + to_string(f��));

			return 1;
		}
	}

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
// �������擾
//============================================================================
const float& CFan::GetLength() const
{
	return m_fLength;
}

//============================================================================
// ������ݒ�
//============================================================================
void CFan::SetLength(float fLength)
{
	m_fLength = fLength;
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
		pNewInstance->SetLength(Json["Length"]);
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
CFan* CFan::Create(D3DXVECTOR3 Pos, float fDirection, float fLength, float fRange)
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
	pNewInstance->SetLength(fLength);
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
	m_fLength{ 0.0f },
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
	// ���p��������x�N�g�����v�Z
	Vec3 VecDir = Vec3(sinf(m_fDirection), 0.0f, cosf(m_fDirection));

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = m_Pos;
	pVtx[1].pos = VecDir * 350.0f;

	//for (WORD i = 0; i < NUM_VTX; ++i)
	//{
	//	CRenderer::SetDebugString("�I�I�I�I�I : " + to_string(pVtx[i].pos.x) + " : " + to_string(pVtx[i].pos.y) + " : " + to_string(pVtx[i].pos.z));
	//}

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