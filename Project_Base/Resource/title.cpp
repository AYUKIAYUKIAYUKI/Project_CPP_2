//============================================================================
// 
// �^�C�g�� [title.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "motion_set.h"
#include "object_parts.h"
#include "object_X.h"

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CTitle::Update()
{
	// ���N���X�̍X�V����
	CScene::Update();

	// ���̌����E���W��ݒ�
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Tree Param Edit"))
	{
		D3DXVECTOR3 NewRot = m_pButterfly->GetRot();
		ImGui::SliderFloat("RotX", &NewRot.x, -D3DX_PI, D3DX_PI);
		ImGui::SliderFloat("RotY", &NewRot.y, -D3DX_PI, D3DX_PI);
		ImGui::SliderFloat("RotZ", &NewRot.z, -D3DX_PI, D3DX_PI);
		m_pButterfly->SetRot(NewRot);
		D3DXVECTOR3 NewPos = m_pButterfly->GetPos();
		ImGui::DragFloat("PosX", &NewPos.x);
		ImGui::DragFloat("PosY", &NewPos.y);
		ImGui::DragFloat("PosZ", &NewPos.z);
		m_pButterfly->SetPos(NewPos);
		ImGui::End();
	}

	// ���̃V�[����
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		To_Next();
	}
#ifdef _DEBUG	// �p�����[�^�m�F�p
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F7))
	{
		CObject::ReleaseAll();
		CScene_Manager::ChangeScene(Create());
	}
#endif // _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CTitle::Draw()
{
	// ���N���X�̕`�揈��
	CScene::Draw();
}

//============================================================================
// ���̃V�[����
//============================================================================
void CTitle::To_Next()
{
	// �S�I�u�W�F�N�g�����
	CObject::ReleaseAll();

	// �Q�[���V�[���֕ύX
	CScene_Manager::ChangeScene(CGame::Create());
}

//============================================================================
// �^�C�g���𐶐�
//============================================================================
CTitle* CTitle::Create()
{
	// �^�C�g���𐶐�
	CTitle* pNew = DBG_NEW CTitle();

	if (pNew == nullptr)
	{
		assert(false && "�^�C�g���̐����Ɏ��s");
	}

	// �^�C�g���̏����ݒ�
	if (FAILED(pNew->Init()))
	{
		assert(false && "�^�C�g���̏����ݒ�Ɏ��s");
	}

	return pNew;
}

//============================================================================
// 
// protected�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CTitle::CTitle() :
	CScene{},
	m_pButterfly{ nullptr },
	m_pTree{ nullptr },
	m_pHole{ nullptr }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTitle::~CTitle()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CTitle::Init()
{
	// �������𐶐�
	{
		// ��
		m_pButterfly = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\butterfly_motion.json"));
		auto ButterflyParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\butterfly.json");
		m_pButterfly->SetRot(utility::JsonConvertToVec3(ButterflyParam["Rot"]));
		m_pButterfly->SetPos(utility::JsonConvertToVec3(ButterflyParam["Pos"]));

		// ��
		m_pTree = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\tree_motion.json"));
		auto TreeParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\tree.json");
		m_pTree->SetRot(utility::JsonConvertToVec3(TreeParam["Rot"]));
		m_pTree->SetPos(utility::JsonConvertToVec3(TreeParam["Pos"]));

		// ��
		//m_pHole = CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\hole.json"));
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTitle::Uninit()
{

}