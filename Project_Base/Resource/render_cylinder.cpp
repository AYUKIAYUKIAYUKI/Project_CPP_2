//============================================================================
// 
// �V�����_�[�\�� [render_cylinder.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "render_cylinder.h"
#include "object_X.h"

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
// �`��D��x�w��R���X�g���N�^
//============================================================================
CRender_Cylinder::CRender_Cylinder(LAYER Priority) :
	CRender_Collision{ Priority },
	m_fSyncRadius{ 0.0f },
	m_fSyncHeight{ 0.0f },
	m_pCylinderModel{ CObject_X::Create(Priority, CX_Manager::TYPE::RENDER_CYLINDER) }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CRender_Cylinder::~CRender_Cylinder()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CRender_Cylinder::Init()
{
	// ����\���̏����ݒ�
	if (FAILED(CRender_Collision::Init()))
	{
		return E_FAIL;
	}

	// �\���̓����x��ݒ�
	m_pCylinderModel->SetCol({ 1.0f, 1.0f, 1.0f, 0.5f });
	m_pCylinderModel->SetUseCol(true);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CRender_Cylinder::Uninit()
{
	// ����\���̏I������
	CRender_Collision::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CRender_Cylinder::Update()
{
	if (m_pRef != nullptr)
	{ // �ΏۃI�u�W�F�N�g���ݒ肳��Ă�����

		// ����̃T�C�Y�ɍ��킹�ăX�P�[�����g��
		const float& fRad = m_pRef->GetRadius(), fHeight = m_pRef->GetHeight();
		const Vec3& Scale = { fRad, fHeight, fRad };
		m_pCylinderModel->SetScale(Scale);

		// �I�u�W�F�N�g�̍��W�ɔ����\��
		m_pCylinderModel->SetPos(m_pRef->GetPos());
	}
	else
	{ // �ΏۃI�u�W�F�N�g���ݒ肳��Ă��Ȃ�

		// ����̃T�C�Y�ɍ��킹�ăX�P�[�����g��
		const Vec3& Scale = { m_fSyncRadius, m_fSyncHeight, m_fSyncRadius };
		m_pCylinderModel->SetScale(Scale);

		// �I�u�W�F�N�g�̍��W�ɔ����\��
		m_pCylinderModel->SetPos(m_CenterSyncPos);
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CRender_Cylinder::Draw()
{

}

//============================================================================
// �V���N�����a��ݒ�
//============================================================================
void CRender_Cylinder::SetSyncRadius(float fRadius)
{
	m_fSyncRadius = fRadius;
}

//============================================================================
// �V���N��������ݒ�
//============================================================================
void CRender_Cylinder::SetSyncHeight(float fHeight)
{
	m_fSyncHeight = fHeight;
}

//============================================================================
// �F��ݒ�
//============================================================================
void CRender_Cylinder::SetCol(D3DXCOLOR Col)
{
	m_pCylinderModel->SetCol(Col);
}

//============================================================================
// �F���f��ݒ�
//============================================================================
void CRender_Cylinder::SetUseCol(bool bUse)
{
	m_pCylinderModel->SetUseCol(bUse);
}

//============================================================================
// ���f����ύX
//============================================================================
void CRender_Cylinder::ChangeModel(CX_Manager::TYPE Type)
{
	m_pCylinderModel->BindModel(Type);
}

//============================================================================
// ����
//============================================================================
CRender_Cylinder* CRender_Cylinder::Create()
{
	// ����\���𐶐�
	CRender_Cylinder* pRender_Collision = DBG_NEW CRender_Cylinder(LAYER::FRONT);

	// �������s
	if (pRender_Collision == nullptr)
	{
		assert(false && "�V�����_�[�\���̐����Ɏ��s���܂���");
	}

	// ����\���̏����ݒ�
	pRender_Collision->Init();


	return pRender_Collision;
}

//============================================================================
// ����
//============================================================================
CRender_Cylinder* CRender_Cylinder::Create(CObject_X* pRef)
{
	// ����\���𐶐�
	CRender_Cylinder* pRender_Collision = DBG_NEW CRender_Cylinder(LAYER::FRONT);

	// �������s
	if (pRender_Collision == nullptr)
	{
		assert(false && "�V�����_�[�\���̐����Ɏ��s���܂���");
	}

	// ����\���̏����ݒ�
	pRender_Collision->Init();

	// �ΏۃI�u�W�F�N�g�̐ݒ�
	pRender_Collision->SetRefObj(pRef);

	return pRender_Collision;
}