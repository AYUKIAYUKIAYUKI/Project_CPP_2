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
#include "field_manager.h"
#include "object_HUD.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"
#include "boss.h"

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

#ifdef _DEBUG
	// ����
	ParamControl();
#endif // _DEBUG

	// �}�b�v�V���{���̍X�V
	UpdateMapSymbol();

	// �{�X�Q�[�W�w�i�̍X�V
	UpdateBossGaugeBack();

	// �{�X�Q�[�W�o�[�̍X�V
	UpdateBossGaugeBar();
}

//============================================================================
// �`�揈��
//============================================================================
void CHUD_Manager::Draw()
{
	/* ���݂͖��� */
}

//============================================================================
// �}�b�v�V���{���̓��ꋓ��
//============================================================================
void CHUD_Manager::SpecialMapSymbol(float fMove)
{
	// �v���C���[�̕��p���^���쐬
	float fAngle = -D3DX_PI * 0.5f, fCoef = 56.0f;

	// �ڕW���W���R�s�[
	Vec3 AdjustPos = m_pMapSymbol->GetPosTarget();

	// �v���C���[�̕��p�ɍ��킹�ăV���{���̍��W����]������
	AdjustPos += { cosf(fAngle)* fCoef, -sinf(fAngle) * fCoef - fMove, 0.0f };
	m_pMapSymbol->SetPos(AdjustPos);
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
	m_pMapBase{ nullptr },
	m_pMapRing{ nullptr },
	m_pPlayerGauge{ nullptr },
	m_pPlayerGaugeWindow{ nullptr },
	m_pBossGaugeBack{ nullptr },
	m_pBossGaugeBar{ nullptr },
	m_BossGaugeBarParam{},
	m_bSwitchBar{ false },
	m_pBossGaugeBase{ nullptr }
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
	{ // �}�b�v�g���쐬
		m_pMapBase = CObject_HUD::Create("Data\\JSON\\HUD\\mapbase.json");
		m_pMapBase->BindTex(CTexture_Manager::TYPE::MAPBASE);
	}

	{ // �}�b�v�ւ��쐬
		m_pMapRing = CObject_HUD::Create("Data\\JSON\\HUD\\mapring.json");
		m_pMapRing->BindTex(CTexture_Manager::TYPE::MAPRING);
	}

	{ // �}�b�v�V���{�����쐬
		m_pMapSymbol = CObject_HUD::Create("Data\\JSON\\HUD\\mapsymbol.json");
		m_pMapSymbol->BindTex(CTexture_Manager::TYPE::MAPSYMBOL);
	}

	{ // �v���C���[�Q�[�W�o�[�𐶐�
		m_pPlayerGauge = CObject_HUD::Create("Data\\JSON\\HUD\\playergauge.json");
		m_pPlayerGauge->BindTex(CTexture_Manager::TYPE::PLAGAUBAR);
	}

	{ // �v���C���[�Q�[�W�g�𐶐�
		m_pPlayerGaugeWindow = CObject_HUD::Create("Data\\JSON\\HUD\\playergaugewindow.json");
		m_pPlayerGaugeWindow->BindTex(CTexture_Manager::TYPE::PLAGAUBASE);
	}

	// �v���C���[�̗͕̑\���𐶐�
	for (WORD wCntLife = 0; wCntLife < CPlayer::MAX_LIFE; ++wCntLife)
	{
		std::string FilePath = "Data\\JSON\\HUD\\playerlife\\" + to_string(wCntLife) + ".json";
		m_pPlayerLife[wCntLife] = CObject_HUD::Create(FilePath);
		m_pPlayerLife[wCntLife]->BindTex(CTexture_Manager::TYPE::PLALIFE);
	}

	{ // �{�X�Q�[�W�w�i�𐶐�
		m_pBossGaugeBack = CObject_HUD::Create("Data\\JSON\\HUD\\bossgaugeback.json");
		m_pBossGaugeBack->BindTex(CTexture_Manager::TYPE::BOSSGAUBACK);
	}

	{ // �{�X�Q�[�W�o�[�𐶐�
		m_pBossGaugeBar = CObject_HUD::Create("Data\\JSON\\HUD\\bossgaugebar.json");
		m_pBossGaugeBar->BindTex(CTexture_Manager::TYPE::BOSSGAUBAR);

		// X�����ڕW�T�C�Y��0�ɂ��Ă���
		Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget();
		SizeTarget.x = 0.0f;
		m_pBossGaugeBar->SetSizeTarget(SizeTarget);

		// �{�X�Q�[�W�o�[�p�̃p�����[�^���R�s�[���Ă���
		m_BossGaugeBarParam = utility::OpenJsonFile("Data\\JSON\\HUD\\bossgaugebar.json");
	}

	{ // �{�X�Q�[�W�g�𐶐�
		m_pBossGaugeBase = CObject_HUD::Create("Data\\JSON\\HUD\\bossgaugebase.json");
		m_pBossGaugeBase->BindTex(CTexture_Manager::TYPE::BOSSGAUBASE);
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CHUD_Manager::Uninit()
{
	/* ���݂͖��� */
}

//============================================================================
// ����
//============================================================================
void CHUD_Manager::ParamControl()
{
#if 0 // ���W�m�F
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Pos Edit")) {
		Vec3 Pos = m_pMapSymbol->GetPos();
		ImGui::DragFloat("Pos:X", &Pos.x);
		ImGui::DragFloat("Pos:Y", &Pos.y);
		ImGui::DragFloat("Pos:Z", &Pos.z);
		m_pMapSymbol->SetPos(Pos);
		ImGui::End();
	}
#endif

#if 0 // �J���[�m�F
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Color Edit")) {
		XCol ColTarget = m_pMapSymbol->GetColTarget();
		ImGui::ColorEdit4("ColTarget", &ColTarget.r);
		m_pMapSymbol->SetColTarget(ColTarget);
		ImGui::End();
	}
#endif

#if 0 // HUD�����̊m�F
	if (CManager::GetKeyboard()->GetPress(DIK_Z))
		m_pPlayerGaugeWindow->SetVibration();
	else if (CManager::GetKeyboard()->GetTrigger(DIK_X))
		m_pPlayerGaugeWindow->SetWaving();
#endif
}

//============================================================================
// �}�b�v�V���{���̍X�V
//============================================================================
void CHUD_Manager::UpdateMapSymbol()
{
	// �v���C���[�̕��p���R�s�[
	float fAngle = m_pSyncPlayer->GetDirection(), fCoef = 56.0f;

	// �ڕW���W���R�s�[
	Vec3 AdjustPos = m_pMapSymbol->GetPosTarget();

	// �v���C���[�̕��p�ɍ��킹�ăV���{���̍��W����]������
	AdjustPos += { cosf(fAngle)* fCoef, -sinf(fAngle) * fCoef, 0.0f };
	m_pMapSymbol->SetPos(AdjustPos);
}

//============================================================================
// �{�X�Q�[�W�w�i�̍X�V
//============================================================================
void CHUD_Manager::UpdateBossGaugeBack()
{

}

//============================================================================
// �{�X�Q�[�W�o�[�̍X�V
//============================================================================
void CHUD_Manager::UpdateBossGaugeBar()
{
	// �{�X�^�C�v�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::BOSS);

	// �{�X�o��ŃQ�[�W�̖�����ύX
	if (!m_bSwitchBar && pObj)
	{
		m_bSwitchBar = true;

		// �o�[�̐F��Ԃ����߂�
		m_pBossGaugeBar->SetColTarget({ 0.75f, 0.0, 0.0f, 0.75f });
	}

	// �����ύX��̃Q�[�W�o�[�̍X�V
	if (m_bSwitchBar)
	{
		// �{�X�̃��C�t�����R�s�[
		CBoss* pBoss = utility::DownCast<CBoss, CObject>(pObj);
		int nLife = pBoss->GetLife();

		{ // �ڕW�T�C�Y��ݒ�

			// ��b�T�C�Y���ő�J�E���g���Ŋ��遨1�J�E���g������̃T�C�Y���o�������݂̃J�E���g�����̃T�C�Y�ɐݒ�
			Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget();
			SizeTarget.x = nLife * (utility::JsonConvertToVec3(m_BossGaugeBarParam["SizeTarget"]).x / CBoss::MAX_LIFE);
			m_pBossGaugeBar->SetSizeTarget(SizeTarget);
		}

		{ // �ڕW���W��ݒ�

			// ( ��b���W - ��b�T�C�Y )�Ń|���S���̍��[�ƂȂ�̍��W���o��
			float BasePosX = utility::JsonConvertToVec3(m_BossGaugeBarParam["PosTarget"]).x - utility::JsonConvertToVec3(m_BossGaugeBarParam["SizeTarget"]).x;

			// �����𑫂����ƂŁA�Q�[�W�������E�ɂ��炷���Q�[�W�Ƙg�̍��[������
			Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget(), PosTarget = m_pBossGaugeBar->GetPosTarget();
			PosTarget.x = BasePosX + SizeTarget.x;
			m_pBossGaugeBar->SetPosTarget(PosTarget);
		}

		{ // �e�N�X�`���T�C�Y��ݒ�

			// ���l�𒼐ڕύX����̂ŁA�ڕW�l�ւ̕�Ԃ͂����ōs��
			Vec2 TexSize = m_pBossGaugeBar->GetTexSize();
			TexSize.x += (1.0f - (nLife * (1.0f / CBoss::MAX_LIFE)) - TexSize.x) * static_cast<float>(m_BossGaugeBarParam["CorrectionCoef"]);
			m_pBossGaugeBar->SetTexSize(TexSize);
		}

		return;
	}

	// �j�󂵂��u���b�N�����R�s�[
	float nCntDestroyBlock = static_cast<float>(CField_Manager::GetInstance()->GetCntDestroyBlock());

	{ // �ڕW�T�C�Y��ݒ�

		// ��b�T�C�Y���ő�J�E���g���Ŋ��遨1�J�E���g������̃T�C�Y���o�������݂̃J�E���g�����̃T�C�Y�ɐݒ�
		Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget();
		SizeTarget.x = nCntDestroyBlock * (utility::JsonConvertToVec3(m_BossGaugeBarParam["SizeTarget"]).x / CField_Manager::MAX_DESTROY_BLOCK);
		m_pBossGaugeBar->SetSizeTarget(SizeTarget);
	}

	{ // �ڕW���W��ݒ�

		// ( ��b���W - ��b�T�C�Y )�Ń|���S���̍��[�ƂȂ�̍��W���o��
		float BasePosX = utility::JsonConvertToVec3(m_BossGaugeBarParam["PosTarget"]).x - utility::JsonConvertToVec3(m_BossGaugeBarParam["SizeTarget"]).x;

		// �����𑫂����ƂŁA�Q�[�W�������E�ɂ��炷���Q�[�W�Ƙg�̍��[������
		Vec3 SizeTarget = m_pBossGaugeBar->GetSizeTarget(), PosTarget = m_pBossGaugeBar->GetPosTarget();
		PosTarget.x = BasePosX + SizeTarget.x;
		m_pBossGaugeBar->SetPosTarget(PosTarget);
	}

	{ // �e�N�X�`���T�C�Y��ݒ�

		// ���l�𒼐ڕύX����̂ŁA�ڕW�l�ւ̕�Ԃ͂����ōs��
		Vec2 TexSize = m_pBossGaugeBar->GetTexSize();
		TexSize.x += (1.0f - (nCntDestroyBlock * (1.0f / CField_Manager::MAX_DESTROY_BLOCK)) - TexSize.x) * static_cast<float>(m_BossGaugeBarParam["CorrectionCoef"]);
		m_pBossGaugeBar->SetTexSize(TexSize);
	}
}

//============================================================================
// �G���[���o
//============================================================================
bool CHUD_Manager::DetectError()
{
	// �G���[���o�p
	bool bError = 0;

	if (!m_pMapBase)
	{
		CRenderer::GetRenderer()->SetDebugString("�}�b�v�g�\�����o���܂���");
		bError = 1;
	}

	if (!m_pMapRing)
	{
		CRenderer::GetRenderer()->SetDebugString("�}�b�v�֕\�����o���܂���");
		bError = 1;
	}

	if (!m_pMapSymbol)
	{
		CRenderer::GetRenderer()->SetDebugString("�}�b�v�V���{���\�����o���܂���");
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

	if (m_pBossGaugeBack == nullptr)
	{
		CRenderer::GetRenderer()->SetDebugString("�{�X�Q�[�W�w�i�\�����o���܂���");
		bError = 1;
	}

	if (m_pBossGaugeBar == nullptr)
	{
		CRenderer::GetRenderer()->SetDebugString("�{�X�Q�[�W�o�[�\�����o���܂���");
		bError = 1;
	}

	if (m_pBossGaugeBase == nullptr)
	{
		CRenderer::GetRenderer()->SetDebugString("�{�X�Q�[�W�g�\�����o���܂���");
		bError = 1;
	}

	// �G���[���o�󋵂�Ԃ�
	return bError;
}