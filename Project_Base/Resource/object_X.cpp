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
// �ÓI�����o�ϐ��̏�����
//****************************************************
const float CObject_X::DEFAULT_SCALE_VALUE = 1.0f;	// �f�t�H���g�X�P�[���l
const float CObject_X::DEFAULT_ALPHA_VALUE = 1.0f;	// �f�t�H���g�A���t�@�l

//============================================================================
// �R���X�g���N�^
//============================================================================
CObject_X::CObject_X(int nPriority) :
	CObject{ nPriority },
	m_pModel{ nullptr },
	m_Pos{ 0.0f, 0.0f, 0.0f },
	m_Rot{ 0.0f, 0.0f, 0.0f },
	m_Size{ 0.0f, 0.0f, 0.0f },
	m_fScale{ DEFAULT_SCALE_VALUE },
	m_fAlpha{ DEFAULT_ALPHA_VALUE }
{
	D3DXMatrixIdentity(&m_MtxWorld);	// ���[���h�s��
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
#ifdef _DEBUG
		CRenderer::GetInstance()->SetDebugString(static_cast<std::string>("�y") + typeid(*this).name() + static_cast<std::string>("�Ƀ��f����񂪂���܂���I�z"));
#endif	// _DEBUG
		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev{ CRenderer::GetInstance()->GetDeviece() };

	// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef{};

	// �}�e���A�����ւ̃|�C���^
	D3DXMATERIAL* pMat{ nullptr };

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
	m_pModel = CModel_X_Manager::GetInstance()->GetModel(Type);
}

//============================================================================
// ���W�擾
//============================================================================
D3DXVECTOR3 CObject_X::GetPos()
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
// �����擾
//============================================================================
D3DXVECTOR3& CObject_X::GetRot()
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
// �T�C�Y�擾
//============================================================================
D3DXVECTOR3 CObject_X::GetSize()
{
	return m_Size;
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CObject_X::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �A���t�@�l�擾
//============================================================================
float& CObject_X::GetAlpha()
{
	return m_fAlpha;
}

//============================================================================
// �A���t�@�l�ݒ�
//============================================================================
void CObject_X::SetAlpha(float fAlpha)
{
	m_fAlpha = fAlpha;
}

//============================================================================
// ����
//============================================================================
CObject_X* CObject_X::Create()
{
	CObject_X* pObjectX = DBG_NEW CObject_X{};

	// �����o���Ă����珉���ݒ�
	if (pObjectX != nullptr)
	{
		pObjectX->Init();
	}

	return pObjectX;
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CObject_X::SetMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxScale{}, mtxRot{}, mtxTrans{};

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