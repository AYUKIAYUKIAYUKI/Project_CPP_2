//============================================================================
// 
// X�I�u�W�F�N�g�Ǘ� [object_X.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************

/// <summary> Z�o�b�t�@�ł̕`��؂�ւ� </summary>
#define CHANGE_DRAW_ZBUFFER 0

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
CObject_X::CObject_X(LAYER Priority) :
	CObject{ Priority },
	m_pModel{ nullptr },
	m_Pos{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Scale{ 1.0f, 1.0f, 1.0f },
	m_fAlpha{ ALPHA_INIT }
{
	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_X::~CObject_X()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_X::Init()
{
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_X::Uninit()
{

}

//============================================================================
// �X�V����
//============================================================================
void CObject_X::Update()
{
	// �����𒲐�����
	AdjustRotAngle();

	// ���[���h�s��̐ݒ�
	SetMtxWorld();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_X::Draw()
{
	// ���f�����ݒ肳��Ă��Ȃ�
	if (m_pModel == nullptr)
	{
#ifdef _DEBUG	// �x���\��

		CRenderer::SetDebugString(static_cast<std::string>("�y") + typeid(*this).name() + static_cast<std::string>("�Ƀ��f����񂪂���܂���I�z"));

#endif	// _DEBUG

		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�@���̎������K����L����
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

#if CHANGE_DRAW_ZBUFFER

		// �[�x�e�X�g�̔�r���@�̕ύX
		pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		// �[�x�o�b�t�@�ɕ`�悵�Ȃ�
		pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

#endif	// CHANGE_DRAW_ZBUFFER

	// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	// �}�e���A�����ւ̃|�C���^
	D3DXMATERIAL* pMat = nullptr;

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// ���݂̃}�e���A�����擾
	pDev->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<int>(m_pModel->dwNumMat); nCntMat++)
	{
		// �A���t�@�l�̐ݒ�
		pMat[nCntMat].MatD3D.Diffuse.a = m_fAlpha;

		// �}�e���A���̐ݒ�
		pDev->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDev->SetTexture(0, m_pModel->apTex[nCntMat]);

		// �I�u�W�F�N�g�p�[�c�̕`��
		m_pModel->pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDev->SetMaterial(&matDef);

	// ���_�@���̎������K���𖳌���
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

#if CHANGE_DRAW_ZBUFFER

		// �[�x�e�X�g�̔�r���@�̕ύX
		pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		// �[�x�o�b�t�@�ɏ�������
		pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

#endif	// CHANGE_DRAW_ZBUFFER
}

//============================================================================
// ���f���擾
//============================================================================
CX_Manager::MODEL* CObject_X::GetModel()
{
	return m_pModel;
}

//============================================================================
// ���f������
//============================================================================
void CObject_X::BindModel(CX_Manager::MODEL* pModel)
{
	m_pModel = pModel;
}

//============================================================================
// �����ƃ��f������
//============================================================================
void CObject_X::BindModel(CX_Manager::TYPE Type)
{
	// ���f�����擾���Đݒ�
	m_pModel = CX_Manager::GetInstance()->GetModel(Type);
}

//============================================================================
// �����擾
//============================================================================
const D3DXVECTOR3& CObject_X::GetRot()const
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_X::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// ���W�擾
//============================================================================
const D3DXVECTOR3& CObject_X::GetPos() const
{
	return m_Pos;
}

//============================================================================
// ���W�ݒ�
//============================================================================
void CObject_X::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �k�ڂ��擾
//============================================================================
const D3DXVECTOR3& CObject_X::GetScale() const
{
	return m_Scale;
}

//============================================================================
// �k�ڂ�ݒ�
//============================================================================
void CObject_X::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//============================================================================
// �A���t�@�l���擾
//============================================================================
const float& CObject_X::GetAlpha() const
{
	return m_fAlpha;
}

//============================================================================
// �A���t�@�l��ݒ�
//============================================================================
void CObject_X::SetAlpha(float fAlpha)
{
	m_fAlpha = fAlpha;
}

//============================================================================
// �T�C�Y���擾
//============================================================================
D3DXVECTOR3 CObject_X::GetSize() const
{
	return VEC3_INIT;
}

//============================================================================
// ���a���擾
//============================================================================
float CObject_X::GetRadius() const
{
	return 0.0f;
}

//============================================================================
// �������擾
//============================================================================
float CObject_X::GetHeight() const
{
	return 0.0f;
}

//============================================================================
// ����
//============================================================================
CObject_X* CObject_X::Create()
{
	// �C���X�^���X�𐶐�
	CObject_X* pObjectX = DBG_NEW CObject_X();

	// �������s
	if (pObjectX == nullptr)
	{
		assert(false && "X�I�u�W�F�N�g�̐����Ɏ��s���܂���");
	}

	// X�I�u�W�F�N�g�̏����ݒ�
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// ����
//============================================================================
CObject_X* CObject_X::Create(LAYER Priority)
{
	// �C���X�^���X�𐶐�
	CObject_X* pObjectX = DBG_NEW CObject_X(Priority);

	// �������s
	if (pObjectX == nullptr)
	{
		assert(false && "X�I�u�W�F�N�g�̐����Ɏ��s���܂���");
	}

	// X�I�u�W�F�N�g�̏����ݒ�
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// ����
//============================================================================
CObject_X* CObject_X::Create(LAYER Priority, CX_Manager::TYPE Type)
{
	// �C���X�^���X�𐶐�
	CObject_X* pObjectX = DBG_NEW CObject_X(Priority);

	// �������s
	if (pObjectX == nullptr)
	{
		assert(false && "X�I�u�W�F�N�g�̐����Ɏ��s���܂���");
	}

	// X�I�u�W�F�N�g�̏����ݒ�
	pObjectX->Init();

	// ���f����ݒ�
	pObjectX->BindModel(Type);

	return pObjectX;
}

//============================================================================
// ����
//============================================================================
CObject_X* CObject_X::Create(JSON Json)
{
	// �p�����[�^���R�s�[���Ă���
	const auto& Priority = Json["Priority"];
	const auto& ModelType = Json["ModelType"];
	const auto& Rot = Json["Rot"];
	const auto& Pos = Json["Pos"];
	const auto& Scale = Json["Scale"];
	const auto& Alpha = Json["Alpha"];

	// �C���X�^���X�𐶐�
	CObject_X* pObjectX = DBG_NEW CObject_X(static_cast<CObject::LAYER>(Priority));

	// �������s
	if (pObjectX == nullptr)
	{
		assert(false && "X�I�u�W�F�N�g�̐����Ɏ��s���܂���");
	}

	// X�I�u�W�F�N�g�̏����ݒ�
	pObjectX->Init();

	// �e��p�����[�^��ݒ�
	pObjectX->BindModel(static_cast<CX_Manager::TYPE>(ModelType));
	pObjectX->SetRot(Vec3(Rot[0], Rot[1], Rot[2]));
	pObjectX->SetPos(Vec3(Pos[0], Pos[1], Pos[2]));
	pObjectX->SetScale(Vec3(Scale[0], Scale[1], Scale[2]));
	pObjectX->SetAlpha(Alpha);

	return pObjectX;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �����𒲐�����
//============================================================================
void CObject_X::AdjustRotAngle()
{
	while (fabsf(m_Rot.y) > D3DX_PI)
	{
		utility::AdjustDirection(m_Rot.y);
	}
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CObject_X::SetMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

	// �g�k�s��쐬
	D3DXMatrixScaling(&mtxScale,
		m_Scale.x,
		m_Scale.y,
		m_Scale.z);

	// �g�k�s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxScale);

	// ��]�s��쐬
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y,
		m_Rot.x,
		m_Rot.z);

	// ��]�s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxRot);

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