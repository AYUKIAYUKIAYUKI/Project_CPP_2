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
#include "manager.h"
#include "title.h"
#include "object_HUD.h"
#include "motion_set.h"

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

	// �X�v���C���e�X�g�̍X�V����
	//m_pSpline->Update();

#if 0
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

#if 0
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Text"))
	{
		D3DXVECTOR3 Pos = m_pText->GetPos();
		ImGui::DragFloat("Pos:x", &Pos.x);
		ImGui::DragFloat("Pos:y", &Pos.y);
		ImGui::DragFloat("Pos:z", &Pos.z);
		m_pText->SetPos(Pos);
		m_pText->SetPosTarget(Pos);

		D3DXVECTOR3 Rot = m_pText->GetRot();
		ImGui::DragFloat("Rot:x", &Rot.x);
		ImGui::DragFloat("Rot:y", &Rot.y);
		ImGui::DragFloat("Rot:z", &Rot.z);
		m_pText->SetRot(Rot);
		m_pText->SetRotTarget(Rot);

		D3DXVECTOR3 Size = m_pText->GetSize();
		ImGui::DragFloat("Size:x", &Size.x);
		ImGui::DragFloat("Size:y", &Size.y);
		ImGui::DragFloat("Size:z", &Size.z);
		m_pText->SetSize(Size);
		m_pText->SetSizeTarget(Size);

		ImGui::End();
	}
#endif

	// �t�F�C�N�v���C���[�̍X�V
	UpdateFakePlayer();

	// ���̃V�[����
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
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

	// �X�v���C���̕`�揈��
	//m_pSpline->Draw();
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
	m_pText{ nullptr }
	//m_pSpline{ nullptr }
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
	pCamera->SetRot({ 0.0f, 0.0f, 0.0f });
	pCamera->SetRotTarget({ 0.0f, 0.0f, 0.0f });
	pCamera->SetPos({ 0.0f, 0.0f, 0.0f });
	pCamera->SetPosTarget({ 0.0f, 0.0f, 0.0f });
	pCamera->SetDistance(50.0f);
	pCamera->SetUpAdjust(0.0f);

	// �t�F�C�N�v���C���[�̐���
	m_pFakePlayer = CMotion_Set::Create(CObject::LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\player_motion.json"));
	auto FakePlayerParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\fakeplayer.json");
	m_pFakePlayer->SetNowMotion(1);
	m_pFakePlayer->SetRot(utility::JsonConvertToVec3(FakePlayerParam["Rot"]));
	m_Path = utility::JsonConvertToVec3(FakePlayerParam["Pos"]);
	m_pFakePlayer->SetPos({ 0.0f, -1000.0f, 0.0f });

	// �e�L�X�g�̐���
	m_pText = CObject_HUD::Create(utility::OpenJsonFile("Data\\JSON\\HUD\\result.json"));
	m_pText->BindTex(CTexture_Manager::TYPE::RESULT);

	/* �X�v���C���e�X�g�̏����ݒ� */
	//m_pSpline = DBG_NEW CSpline_Test();
	//m_pSpline->Init();

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

	///* �X�v���C���e�X�g�̏I������ */
	//if (m_pSpline != nullptr)
	//{
	//	m_pSpline->Uninit();
	//	delete m_pSpline;
	//	m_pSpline = nullptr;
	//}
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