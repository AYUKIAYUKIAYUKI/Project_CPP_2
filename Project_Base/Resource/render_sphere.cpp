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
	m_fSyncRadius{ 0.0f },
	m_pSphere{ CObject_X::Create(Priority, CX_Manager::TYPE::RENDER_SPHERE) }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CRender_Sphere::~CRender_Sphere()
{
	// �X�t�B�A�\����j���\��
	if (m_pSphere != nullptr)
	{
		m_pSphere->SetRelease();
	}
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
	m_pSphere->SetCol({ 1.0f, 1.0f, 1.0f, 0.5f });
	m_pSphere->SetUseCol(true);

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
	// �E�B���h�E��\��
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Sphere"))
	{
		ImGui::Text("Pos:X %.1f:Y %.1f:Z %.1f", m_pSphere->GetPos().x, m_pSphere->GetPos().y, m_pSphere->GetPos().z);
	}
	ImGui::End();

	if (m_pRef != nullptr)
	{ // �ΏۃI�u�W�F�N�g���ݒ肳��Ă�����

		// ����̃T�C�Y�ɍ��킹�ăX�P�[�����g��
		const float& fRad = m_pRef->GetRadius();
		const Vec3& Scale = { fRad, fRad, fRad };
		m_pSphere->SetScale(Scale);

		// �I�u�W�F�N�g�̍��W�ɔ����\��
		m_pSphere->SetPos(m_pRef->GetPos());
	}
	else
	{ // �ΏۃI�u�W�F�N�g���ݒ肳��Ă��Ȃ�

		// ����̃T�C�Y�ɍ��킹�ăX�P�[�����g��
		const Vec3& Scale = { m_fSyncRadius, m_fSyncRadius, m_fSyncRadius };
		m_pSphere->SetScale(Scale);

		// �I�u�W�F�N�g�̍��W�ɔ����\��
		m_pSphere->SetPos(m_CenterSyncPos);
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CRender_Sphere::Draw()
{

}

//============================================================================
// ���a��ݒ�
//============================================================================
void CRender_Sphere::SetSyncRadius(float fRadius)
{
	m_fSyncRadius = fRadius;
}

//============================================================================
// ���f����ύX
//============================================================================
void CRender_Sphere::ChangeModel(CX_Manager::TYPE Type)
{
	m_pSphere->BindModel(Type);
}

//============================================================================
// ����
//============================================================================
CRender_Sphere* CRender_Sphere::Create()
{
	// ����\���𐶐�
	CRender_Sphere* pRender_Collision = DBG_NEW CRender_Sphere(LAYER::FRONT);

	// �������s
	if (pRender_Collision == nullptr)
	{
		assert(false && "�X�t�B�A�\���̐����Ɏ��s���܂���");
	}

	// ����\���̏����ݒ�
	pRender_Collision->Init();

	return pRender_Collision;
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
		assert(false && "�X�t�B�A�\���̐����Ɏ��s���܂���");
	}

	// ����\���̏����ݒ�
	pRender_Collision->Init();

	// �ΏۃI�u�W�F�N�g�̐ݒ�
	pRender_Collision->SetRefObj(pRef);

	return pRender_Collision;
}