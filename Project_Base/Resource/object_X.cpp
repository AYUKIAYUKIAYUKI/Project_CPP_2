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
#include "render_collision.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************

/// <summary> Z�o�b�t�@�ł̕`��؂�ւ� </summary>
#define CHANGE_DRAW_ZBUFFER 0

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
	m_pRender_Collision{ nullptr },
	m_Pos{ 0.0f, 0.0f, 0.0f },
	m_Rot{ 0.0f, 0.0f, 0.0f },
	m_fScale{ SCALE_INIT },
	m_fAlpha{ ALPHA_INIT }
{
	// ����\���𐶐�
	m_pRender_Collision = CRender_Collision::Create(this);

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
	// ����\���̔j��
	if (m_pRender_Collision != nullptr)
	{
		// ����\���̔j���\��
		m_pRender_Collision->SetRelease();

		// �|�C���^��������
		m_pRender_Collision = nullptr;
	}
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
CModel_X_Manager::MODEL* CObject_X::GetModel()
{
	return m_pModel;
}

//============================================================================
// ���f������
//============================================================================
void CObject_X::BindModel(CModel_X_Manager::MODEL* pModel)
{
	m_pModel = pModel;
}

//============================================================================
// �����ƃ��f������
//============================================================================
void CObject_X::BindModel(CModel_X_Manager::TYPE Type)
{
	// ���f�����擾���Đݒ�
	m_pModel = CModel_X_Manager::GetInstance()->GetModel(Type);
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
const float& CObject_X::GetScale() const
{
	return m_fScale;
}

//============================================================================
// �k�ڂ�ݒ�
//============================================================================
void CObject_X::SetScale(float fScale)
{
	m_fScale = fScale;
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
// ���a���擾
//============================================================================
float CObject_X::GetRadius() const
{
	return 0.0f;
}

//============================================================================
// �T�C�Y���擾
//============================================================================
D3DXVECTOR3 CObject_X::GetSize() const
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
		CUtility::AdjustDirection(m_Rot.y);
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
		m_fScale,
		m_fScale,
		m_fScale);

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