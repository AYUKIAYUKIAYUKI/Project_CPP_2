//============================================================================
// 
// ���U���g [result.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "result.h"
#include "motion_set.h"
#include "object_PopUp.h"

#include "manager.h"
#include "title.h"

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CResult::Update()
{
	// ���N���X�̍X�V����
	CScene::Update();

	// �t�F�C�N�v���C���[�̒���
#if 0
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Playerfake"))
	{
		D3DXVECTOR3 Pos = m_pFakePlayer->GetPos();
		ImGui::DragFloat("Pos:x", &Pos.x);
		ImGui::DragFloat("Pos:y", &Pos.y);
		ImGui::DragFloat("Pos:z", &Pos.z);
		m_pFakePlayer->SetPos(Pos);

		D3DXVECTOR3 Rot = m_pFakePlayer->GetRot();
		ImGui::DragFloat("Rot:x", &Rot.x);
		ImGui::DragFloat("Rot:y", &Rot.y);
		ImGui::DragFloat("Rot:z", &Rot.z);
		m_pFakePlayer->SetRot(Rot);

		ImGui::End();
	}
#endif

	// �|�b�v�A�b�v�E�B���h�E�̒���
#if 0
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("PopUpWindow"))
	{
		// �T�C�Y
		D3DXVECTOR3 Size = m_pPopUpWindow->GetSize();
		ImGui::DragFloat("Size:x", &Size.x);
		ImGui::DragFloat("Size:y", &Size.y);
		ImGui::DragFloat("Size:z", &Size.z);
		m_pPopUpWindow->SetSize(Size);
	
		// ���W
		D3DXVECTOR3 Pos = m_pPopUpWindow->GetPos();
		ImGui::DragFloat("Pos:x", &Pos.x);
		ImGui::DragFloat("Pos:y", &Pos.y);
		ImGui::DragFloat("Pos:z", &Pos.z);
		m_pPopUpWindow->SetPos(Pos);

		ImGui::End();
	}
#endif

	// �t�F�C�N�v���C���[�̍X�V
	UpdateFakePlayer();

	// �R���g���[���[���擾
	CInputPad* pPad = CManager::GetPad();

	// ���̃V�[����
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputPad::JOYKEY::A) ||
		pPad->GetTrigger(CInputPad::JOYKEY::START))
	{
		To_Next();
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CResult::Draw()
{
	// ���N���X�̕`�揈��
	CScene::Draw();
}

//============================================================================
// ���̃V�[����
//============================================================================
void CResult::To_Next()
{
	// �S�I�u�W�F�N�g�����
	CObject::ReleaseAll();

	// �^�C�g���V�[���֕ύX
	CScene_Manager::ChangeScene(CTitle::Create());
}

//============================================================================
// ���U���g�𐶐�
//============================================================================
CResult* CResult::Create()
{
	// ���U���g�𐶐�
	CResult* pNew = DBG_NEW CResult();

	if (pNew == nullptr)
	{
		assert(false && "���U���g�̐����Ɏ��s");
	}

	// ���U���g�̏����ݒ�
	if (FAILED(pNew->Init()))
	{
		assert(false && "���U���g�̏����ݒ�Ɏ��s");
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
CResult::CResult() :
	m_Path{ 0.0f, 0.0f, 0.0f },
	m_pFakePlayer{ nullptr },
	m_pPopUpWindow{ nullptr }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CResult::~CResult()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CResult::Init()
{
	// �J�����̃p�����[�^��ݒ�
	CCamera* pCamera = CManager::GetManager()->GetCamera();
	pCamera->ChangeTrackPlayer(false);
	pCamera->SetRot({ 0.0f, 0.0f, 0.0f });
	pCamera->SetRotTarget({ 0.0f, 0.0f, 0.0f });
	pCamera->SetPos({ 0.0f, 0.0f, 0.0f });
	pCamera->SetPosTarget({ 0.0f, 0.0f, 0.0f });
	pCamera->SetDistance(50.0f);
	pCamera->SetUpAdjust(0.0f);

	// �t�F�C�N�v���C���[�̐���
	m_pFakePlayer = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\player_motion.json"));
	auto FakePlayerParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\fakeplayer.json");
	m_pFakePlayer->SetNowMotion(1);
	m_pFakePlayer->SetRot(utility::JsonConvertToVec3(FakePlayerParam["Rot"]));
	m_Path = utility::JsonConvertToVec3(FakePlayerParam["Pos"]);
	m_pFakePlayer->SetPos({ 0.0f, -1024.0f, 0.0f });

	// �|�b�v�A�b�v�E�B���h�E�̐���
	m_pPopUpWindow = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\resultwindow.json"));

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CResult::Uninit()
{
	// �t�F�C�N�v���C���[�̔j��
	if (m_pFakePlayer != nullptr)
	{
		m_pFakePlayer->SetRelease();
		m_pFakePlayer = nullptr;
	}

	// �|�b�v�A�b�v�E�B���h�E�̔j��
	if (m_pPopUpWindow != nullptr)
	{
		m_pPopUpWindow->SetRelease();
		m_pPopUpWindow = nullptr;
	}
}

//============================================================================
// �t�F�C�N�v���C���[�̍X�V����
//============================================================================
void CResult::UpdateFakePlayer()
{
	D3DXVECTOR3 NewPos = m_pFakePlayer->GetPos();
	NewPos += (m_Path - NewPos) * 0.1f;
	m_pFakePlayer->SetPos(NewPos);
}