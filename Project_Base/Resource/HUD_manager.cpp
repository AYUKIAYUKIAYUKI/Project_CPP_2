//============================================================================
// 
// HUD�}�l�[�W���[ [HUD_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "HUD_manager.h"
#include "object_HUD.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CHUD_Manager* CHUD_Manager::m_pHUDManager = nullptr;	// HUD�}�l�[�W���[�̖{��

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CHUD_Manager::Update()
{
	// HUD�I�u�W�F�N�g�Ɉُ킪����΍X�V���s��Ȃ�
	if (DetectError())
		return;

	// �Q�[���V�[���Ŗ�����΍X�V���s��Ȃ�
	if (typeid(*CScene_Manager::GetInstance()->GetScene()) != typeid(CGame))
		return;

	// �v���C���[�̏�񂪖�����΍X�V���s��Ȃ�
	if (!m_pSyncPlayer)
		return;

	// �v���C���[�̗̑͗ʂ��R�s�[
	WORD wSyncPlayerLife = static_cast<WORD>(m_pSyncPlayer->GetLife());

	// �̗͗ʕ����`�悳���悤�ɍ��W�𒲐�
	for (WORD wCntLife = CPlayer::MAX_LIFE; wCntLife > 0; --wCntLife)
	{
		// �ڕW���W���R�s�[
		Vec3 NewPosTarget = m_pPlayerLife[wCntLife - 1]->GetPosTarget();

		// �v���C���[�̗̑͂Ɉ�v����ʂłȂ���Ή�ʊO�ֈړ�
		if (wCntLife > wSyncPlayerLife)
			NewPosTarget.y = -100.0f;
		else
			NewPosTarget.y = 75.0f;

		// ���W�𔽉f
		m_pPlayerLife[wCntLife - 1]->SetPosTarget(NewPosTarget);
	}

#if 0	// HUD�����̊m�F
	if (CManager::GetKeyboard()->GetPress(DIK_Z))
		m_pPlayerGaugeWindow->SetVibration();
	else if (CManager::GetKeyboard()->GetTrigger(DIK_X))
		m_pPlayerGaugeWindow->SetWaving();
#endif
}

//============================================================================
// �`�揈��
//============================================================================
void CHUD_Manager::Draw()
{
	/* ���݂͖��� */
}

//============================================================================
// �v���C���[���Z�b�g
//============================================================================
void CHUD_Manager::SetSyncPlayer(const CPlayer* const pPlayer)
{
	m_pSyncPlayer = pPlayer;
}

//============================================================================
// ����
//============================================================================
HRESULT CHUD_Manager::Create()
{
	// ���ɐ�������Ă�����
	if (m_pHUDManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("HUD�}�l�[�W���[�͊��ɐ�������Ă��܂�", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// �C���X�^���X�𐶐�
	m_pHUDManager = DBG_NEW CHUD_Manager();

	// �������s
	if (m_pHUDManager == nullptr)
	{
		return E_FAIL;
	}

	// HUD�}�l�[�W���[�̏����ݒ�
	if (FAILED(m_pHUDManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CHUD_Manager::Release()
{
	if (m_pHUDManager != nullptr)
	{
		// �I������
		m_pHUDManager->Uninit();

		// �����������
		delete m_pHUDManager;

		// �|�C���^��������
		m_pHUDManager = nullptr;
	}
}

//============================================================================
// HUD�}�l�[�W���[���擾
//============================================================================
CHUD_Manager* CHUD_Manager::GetInstance()
{
	// �{�̂����݂��Ȃ����
	if (m_pHUDManager == nullptr)
	{
		// ����
		if (FAILED(Create()))
		{
			assert(false && "HUD�}�l�[�W���[�̎擾�Ɏ��s");
		}
	}

	return m_pHUDManager;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CHUD_Manager::CHUD_Manager() :
	m_pSyncPlayer{ nullptr }, 
	m_pMap{ nullptr },
	m_pPlayerGauge{ nullptr },
	m_pPlayerGaugeWindow{ nullptr }
{
	// �v���C���[�̗͕̑\����������
	for (WORD wCnt = 0; wCnt < CPlayer::MAX_LIFE; ++wCnt)
	{
		m_pPlayerLife[wCnt] = nullptr;
	}
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CHUD_Manager::~CHUD_Manager()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CHUD_Manager::Init()
{
	{ // �}�b�v�\�����쐬
		m_pMap = CObject_HUD::Create("Data\\JSON\\HUD\\map.json");
		m_pMap->BindTex(CTexture_Manager::TYPE::MAP);
	}

	{ // �v���C���[�̃Q�[�W�𐶐�
		m_pPlayerGauge = CObject_HUD::Create("Data\\JSON\\HUD\\playergauge.json");
		m_pPlayerGauge->BindTex(CTexture_Manager::TYPE::PLAYERGAUGE);
	}

	{ // �v���C���[�̃Q�[�W�E�B���h�E�𐶐�
		m_pPlayerGaugeWindow = CObject_HUD::Create("Data\\JSON\\HUD\\playergaugewindow.json");
		m_pPlayerGaugeWindow->BindTex(CTexture_Manager::TYPE::PLAYERGAUGEWINDOW);
	}

	// �v���C���[�̗͕̑\���𐶐�
	for (WORD wCntLife = 0; wCntLife < CPlayer::MAX_LIFE; ++wCntLife)
	{
		std::string FilePath = "Data\\JSON\\HUD\\playerlife\\" + to_string(wCntLife) + ".json";
		m_pPlayerLife[wCntLife] = CObject_HUD::Create(FilePath);
		m_pPlayerLife[wCntLife]->BindTex(CTexture_Manager::TYPE::CIRCLE);
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CHUD_Manager::Uninit()
{

}

//============================================================================
// �G���[���o
//============================================================================
bool CHUD_Manager::DetectError()
{
	// �G���[���o�p
	bool bError = 0;

	if (!m_pMap)
	{
		CRenderer::GetRenderer()->SetDebugString("�}�b�v�\�����o���܂���");
		bError = 1;
	}

	if (!m_pPlayerGauge)
	{
		CRenderer::GetRenderer()->SetDebugString("�v���C���[�̃Q�[�W�\�����o���܂���");
		bError = 1;
	}

	if (!m_pPlayerGaugeWindow)
	{
		CRenderer::GetRenderer()->SetDebugString("�v���C���[�̃Q�[�W�g�\�����o���܂���");
		bError = 1;
	}

	if (m_pPlayerLife == nullptr)
	{
		CRenderer::GetRenderer()->SetDebugString("�v���C���[�̗͕̑\�����o���܂���");
		bError = 1;
	}

	// �G���[���o�󋵂�Ԃ�
	return bError;
}