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
	// �t���[�����C���N�������g
	m_nNowFrame++;

	// ���N���X�̍X�V����
	CScene::Update();

	// �������̍X�V
	UpdateEnvironment();

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
	m_Path{},
	m_nNowFrame{ 0 },
	m_nMaxFrame{ 0 },
	m_pTree{ nullptr },
	m_pHole{ nullptr }
{
	m_Path.clear();
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
		m_pButterfly->SetNowMotion(1);
		m_Path.push_back(utility::JsonConvertToVec3(ButterflyParam["Path"]));
		m_nMaxFrame = ButterflyParam["MaxFrame"];

		// �J��������ύX
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetDistance(50.0f);
		pCamera->SetUpAdjust(0.0f);
		pCamera->SetPos(utility::JsonConvertToVec3(ButterflyParam["Pos"]));
		pCamera->SetPosTarget(utility::JsonConvertToVec3(ButterflyParam["Pos"]));

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

//============================================================================
// �������̍X�V
//============================================================================
void CTitle::UpdateEnvironment()
{
	// ���̍X�V
	UpdateButterfly();

	if (m_nNowFrame >= m_nMaxFrame)
	{
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetPosTarget({ 100.0f, -50.0f, 0.0f });
		float fDistance = pCamera->GetDistance();

		if (fDistance < 300.0f)
		{
			pCamera->SetDistance(fDistance += 5.0f);
		}
	}
}

//============================================================================
// ���̍X�V
//============================================================================
void CTitle::UpdateButterfly()
{
	if (m_nNowFrame < m_nMaxFrame)
	{
		// �ڕW�n�_�ֈړ�
		D3DXVECTOR3 NewPos = m_pButterfly->GetPos();
		int nFrameCoef = m_nMaxFrame - m_nNowFrame;
		NewPos += (m_Path[0] - NewPos) / nFrameCoef;
		m_pButterfly->SetPos(NewPos);

		// �J�����Ǐ]
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetPos(NewPos);
		pCamera->SetPosTarget(NewPos);
	}
	else if (m_nNowFrame == m_nMaxFrame)
	{
		m_pButterfly->SetNowMotion(0);
	}
}