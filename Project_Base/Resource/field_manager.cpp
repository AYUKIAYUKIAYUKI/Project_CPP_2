//============================================================================
// 
// �t�B�[���h�}�l�[�W���[ [field_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "field_manager.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"
#include "motion_set.h"
#include "object_HUD.h"
#include "player.h"
#include "boss.h"
#include "block.h"
#include "sparks.h"

// �f�o�b�O�\���p
#include "fan.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
#define CHANGE_FIELRDCREATE_STYLE 0	// �u���b�N�̐������@�؂�ւ�

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CField_Manager* CField_Manager::m_pFieldManager = nullptr;	// �t�B�[���h�}�l�[�W���[�̖{��

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �^�C�g�������ǉ������ݒ�
//============================================================================
void CField_Manager::InitForTitle()
{
	// �������̏����ݒ�
	InitEnvironment();
}

//============================================================================
// �X�V����
//============================================================================
void CField_Manager::Update()
{
	// �������̍X�V
	UpdateEnvironment();

	if (m_pSyncPlayer != nullptr)
	{
		// �v���C���[�̌��݂̕��p���`�̕��p�ɂ���
		m_pRenderFan->SetDirection(m_pSyncPlayer->GetDirection());

		// ��`�\���̍X�V����
		m_pRenderFan->Update();
	}

	if (typeid(*CScene_Manager::GetInstance()->GetScene()) == typeid(CGame))
	{
		// �{�X�o��C�x���g
		AppearBossEvent();

#if !CHANGE_FIELRDCREATE_STYLE

		// �u���b�N�̎�������
		GenerateBlock();

		// �u���b�N�̎����폜
		DestroyBlock();

#endif	// CHANGE_FIELRDCREATE_STYLE

		// ���̑S�j�����\�b�h
		if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
		{
			DestroyAllBlock();
		}

		// �̗͂������Ȃ�ƃQ�[���V�[���ɃQ�[���I����ʒm����
		if (m_pSyncPlayer->GetLife() <= 0)
		{
			// �Q�[���V�[���擾
			CGame* const pScene = dynamic_cast<CGame*>(CScene_Manager::GetInstance()->GetScene());

			// �V�[���J�ڊJ�n
			pScene->SetTransition();
		}
	}

	// �f�o�b�O�\��
	PrintDebug();
}

//============================================================================
// �`�揈��
//============================================================================
void CField_Manager::Draw()
{
	// ��`�̕`�揈��
	m_pRenderFan->Draw();
}

//============================================================================
// �W�����v�����񐔂̃C���N�������g
//============================================================================
void CField_Manager::IncrementCntJump()
{
	++m_ActionData.nCntJump;
}

//============================================================================
// �_�b�V�������񐔂̃C���N�������g
//============================================================================
void CField_Manager::IncrementCntDash()
{
	++m_ActionData.nCntDash;
}

//============================================================================
// �{�X�o��
//============================================================================
void CField_Manager::AppearBoss()
{
	// �������U�����n�߂�
	m_pStatue->SetNowMotion(0);
}

//============================================================================
// �v���C���[���Z�b�g
//============================================================================
void CField_Manager::SetSyncPlayer(const CPlayer* const pPlayer)
{
	m_pSyncPlayer = pPlayer;
}

//============================================================================
// ����
//============================================================================
HRESULT CField_Manager::Create()
{
	// ���ɐ�������Ă�����
	if (m_pFieldManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("�t�B�[���h�}�l�[�W���[�͊��ɐ�������Ă��܂�", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// �C���X�^���X�𐶐�
	m_pFieldManager = DBG_NEW CField_Manager();

	// �������s
	if (m_pFieldManager == nullptr)
	{
		return E_FAIL;
	}

	// �t�B�[���h�}�l�[�W���[�̏����ݒ�
	if (FAILED(m_pFieldManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CField_Manager::Release()
{
	if (m_pFieldManager != nullptr)
	{
		// �I������
		m_pFieldManager->Uninit();

		// �����������
		delete m_pFieldManager;

		// �|�C���^��������
		m_pFieldManager = nullptr;
	}
}

//============================================================================
// �t�B�[���h�}�l�[�W���[���擾
//============================================================================
CField_Manager* CField_Manager::GetInstance()
{
	// �{�̂����݂��Ȃ����
	if (m_pFieldManager == nullptr)
	{
		// ����
		if (FAILED(Create()))
		{
			assert(false && "�t�B�[���h�}�l�[�W���[�̎擾�Ɏ��s");
		}
	}

	return m_pFieldManager;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CField_Manager::CField_Manager() :
	m_pSyncPlayer{ nullptr },
	m_pDome{ nullptr },
	m_pStatue{ nullptr },
	m_nCntStatueVibration{ 0 },
	m_pRenderFan{ nullptr }
{
	// �A�N�V�����f�[�^�̏�����
	m_ActionData.nCntDash = 0;
	m_ActionData.nCntJump = 0;
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CField_Manager::~CField_Manager()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CField_Manager::Init()
{
	// ��`�\���𐶐�
	m_pRenderFan = CFan::Create();

#if CHANGE_FIELRDCREATE_STYLE
	// �~�̐���
	DEBUG_CIRCLE();
#endif	// CHANGE_FIELRDCREATE_STYLE

	return S_OK;
}

//============================================================================
// �������̏����ݒ�
//============================================================================
void CField_Manager::InitEnvironment()
{
	{ // �����̐���

		// �p�����[�^�𔽉f
		m_pStatue = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\statue_motion.json"));
		auto StatueParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\statue.json");
		m_pStatue->SetRot(utility::JsonConvertToVec3(StatueParam["Rot"]));
		m_pStatue->SetPos(utility::JsonConvertToVec3(StatueParam["Pos"]));
		
		// �������[�V������ݒ�
		m_pStatue->SetNowMotion(2);
	}
}

//============================================================================
// �I������
//============================================================================
void CField_Manager::Uninit()
{
	// ��`�\����j��
	if (m_pRenderFan != nullptr)
	{
		m_pRenderFan->Release();	// ���
		m_pRenderFan = nullptr;		// �|�C���^��������
	}
}

//============================================================================
// �������̍X�V
//============================================================================
void CField_Manager::UpdateEnvironment()
{
	// �΂̕��𐶐�
	CSparks::AutoGenerate();
}

//============================================================================
// �{�X�o��C�x���g
//============================================================================
void CField_Manager::AppearBossEvent()
{
	// �����̃��[�V�������I�����Ă����炱�̃��\�b�h�𖳎�
	if (m_pStatue->GetStopState())
		return;

	// �Đ����̃��[�V�����ɉ����ď����𕪊�
	if (m_pStatue->GetNowMotion() == 0)
	{ // �������U�����[�V�����Đ���

		// �����U���J�E���g���C���N�������g
		++m_nCntStatueVibration;

		// �J������U��������
		CManager::GetManager()->GetCamera()->SetVibration(0.01f);

		// �J�������{�X�o��p�ɃZ�b�g
		CManager::GetManager()->GetCamera()->SetAppearBoss();

		// �J�E���g���ő�l�ɒB������
		if (m_nCntStatueVibration >= MAX_CNT_STATUEVIBERATION)
		{
			// �J�E���g�����Z�b�g
			m_nCntStatueVibration = 0;

			// ������������΂���郂�[�V�������Z�b�g
			m_pStatue->SetNowMotion(1);

			// �{�X�𐶐�����
			CBoss::Create();
		}
	}
	else if (m_pStatue->GetNowMotion() == 1)
	{ // �����������Ƃ΂���郂�[�V�������Đ���

		// �����U���J�E���g���C���N�������g
		++m_nCntStatueVibration;

		// ���[�V�������œ��������󂷂邠����܂ŃJ�E���g���ꂽ��
		if (m_nCntStatueVibration >= 40)
		{
			// �J������U��������
			CManager::GetManager()->GetCamera()->SetVibration(0.05f);
		}
	}
}

//============================================================================
// �u���b�N�̎�������
//============================================================================
void CField_Manager::GenerateBlock()
{
	// �u���b�N�����J�E���g
	int nCntBlock = CObject::CountSpecificObject(CObject::TYPE::BLOCK);

#ifdef _DEBUG	// �u���b�N���̕\��
	CRenderer::SetDebugString("�u���b�N��:" + to_string(nCntBlock));
#endif	// _DEBUG

#if 1
	// �����ɂ���
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Generate");
	static float fRotY = 0.0f;
	if (ImGui::Button("--"))
		fRotY += D3DX_PI * -0.1f;
	ImGui::SameLine();
	if (ImGui::Button("++"))
		fRotY += D3DX_PI * 0.1f;
	ImGui::SameLine();
	ImGui::SliderFloat("Add RotY", &fRotY, -D3DX_PI, D3DX_PI);
	ImGui::End();
#endif

	// �������W�v�Z�p ((���p + ��`���̊p�x)�̏ꏊ�������|�C���g)
	float fDirection = m_pSyncPlayer->GetDirection();	// �v���C���[�̌��݂̕��p���R�s�[
	float fRange = m_pRenderFan->GetRange();			// ��`�͈͂̕����R�s�[
	Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;		// �u���b�N�p�̍��W�E�������쐬

	// �u���b�N��������ɖ����Ȃ����
	while (nCntBlock < MAX_BLOCK)
	{
		// �j���͈͂ɂ͂ݏo�������������悤�ɒ���
		/* �������W�����_�̏ꍇ�A�����͈͂̔��a���t�B�[���h�̔��a�������Ɩ������[�v */
		do
		{
			// �����p�̍��W������
			NewPos.x = cosf(fDirection + fRange) * FIELD_RADIUS;
			NewPos.y = fabsf(utility::GetRandomValue<float>());
			NewPos.z = sinf(fDirection + fRange) * FIELD_RADIUS;

			// �u���b�N���m�̕������o
			if (DetectAdjacentBlock(NewPos))
			{
				NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
			}

		} while (!m_pRenderFan->DetectInFanRange(NewPos));

		// ����������
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// �u���b�N�𐶐�
		CBlock::Create(NewPos, NewRot);

		// �u���b�N�����J�E���g�A�b�v
		nCntBlock++;
	}
}

//============================================================================
// �אڂ������u���b�N�����o
//============================================================================
bool CField_Manager::DetectAdjacentBlock(const D3DXVECTOR3& Pos)
{
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			/* ����͎����I�ɂ��݂��̋����݂̂��l������ */
			const Vec3& Vec = pBlock->GetPos() - Pos;

			/* ������x�ڋ߂��Ă��܂��Ă���u���b�N�����݂���ꍇ */
			if (sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= pBlock->GetSize().x)
			{
				// ���W�̐�������蒼��
				return 1;
			}
		}

		pObj = pObj->GetNext();
	}

	return 0;
}

//============================================================================
// �u���b�N�̎����폜
//============================================================================
void CField_Manager::DestroyBlock()
{
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::MIDDLE);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			// ��`�\���͈͓̔��Ƀu���b�N��������Δj��
			if (!m_pRenderFan->DetectInFanRange(pBlock->GetPos()))
			{
				pBlock->SetRelease();
			}
		}

		pObj = pObj->GetNext();
	}
}

//============================================================================
// �S�u���b�N�̍폜
//============================================================================
void CField_Manager::DestroyAllBlock()
{
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::MIDDLE);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �j���\��
			pObj->SetRelease();
		}

		pObj = pObj->GetNext();
	}
}

//============================================================================
// �f�o�b�O�\��
//============================================================================
void CField_Manager::PrintDebug()
{
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Action Data")) {
		ImGui::Text("CountJump:%d", m_ActionData.nCntJump);
		ImGui::Text("CountDash:%d", m_ActionData.nCntDash);
		ImGui::End();
	}
}

//============================================================================
// �f�o�b�O�T�[�N��
//============================================================================
void CField_Manager::DEBUG_CIRCLE()
{
	float fDirection = 0.0f;						// ���p
	float fAdder = (D3DX_PI * 2.0f) / MAX_BLOCK;	// ������

	for (WORD i = 0; i < MAX_BLOCK; ++i)
	{
		// �����p�̍��W������
		Vec3 NewPos = VEC3_INIT;
		NewPos.x = cosf(fDirection) * FIELD_RADIUS;
		NewPos.y = 40.0f - utility::GetRandomValue<float>() * 0.15f;
		NewPos.z = -sinf(fDirection) * FIELD_RADIUS;

		// �����p�̌���������
		Vec3 NewRot = VEC3_INIT;
		//NewRot.y = utility::GetRandomValue<float>();
		NewRot.y = fDirection + D3DX_PI * 0.5f;

		// �u���b�N�𐶐�
		CBlock::Create(NewPos, NewRot);

		// ���p�𑝉�
		fDirection += fAdder;
	}
}