//============================================================================
// 
// �X�t�B�A�\�� [render_sphere.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "render_sphere.h"
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
CRender_Sphere::CRender_Sphere(LAYER Priority) :
	CRender_Collision{ Priority },
	m_pSphere{ CObject_X::Create(Priority, CModel_X_Manager::TYPE::RENDER_SPHERE) }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CRender_Sphere::~CRender_Sphere()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CRender_Sphere::Init()
{
	// ����\���̏����ݒ�
	if (FAILED(CRender_Collision::Init()))
	{
		return E_FAIL;
	}

	// �\���̓����x��ݒ�
	m_pSphere->SetAlpha(0.5f);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CRender_Sphere::Uninit()
{
	// ����\���̏I������
	CRender_Collision::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CRender_Sphere::Update()
{
	// ����̃T�C�Y�ɍ��킹�ăX�P�[�����g��
	m_pSphere->SetScale(m_pRef->GetRadius());

	// �I�u�W�F�N�g�̍��W�ɔ����\��
	m_pSphere->SetPos(m_pRef->GetPos());
}

//============================================================================
// �`�揈��
//============================================================================
void CRender_Sphere::Draw()
{

}

//============================================================================
// �X�t�B�A�\���̐F��ݒ�
//============================================================================
void CRender_Sphere::SetColRenderSphere(D3DXCOLOR Col)
{
	m_pSphere->SetCol(Col);
}

//============================================================================
// ����
//============================================================================
CRender_Sphere* CRender_Sphere::Create(CObject_X* pRef)
{
	// ����\���𐶐�
	CRender_Sphere* pRender_Collision = DBG_NEW CRender_Sphere(LAYER::FRONT);

	// �������s
	if (pRender_Collision == nullptr)
	{
		assert(false && "�{�b�N�X�\���̐����Ɏ��s���܂���");
	}

	// ����\���̏����ݒ�
	pRender_Collision->Init();

	// �ΏۃI�u�W�F�N�g�̐ݒ�
	pRender_Collision->SetRefObj(pRef);

	return pRender_Collision;
}