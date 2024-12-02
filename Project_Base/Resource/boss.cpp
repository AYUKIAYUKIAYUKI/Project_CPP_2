//============================================================================
// 
// �{�X [boss.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "boss.h"
#include "bounding_cylinder.h"
#include "manager.h"
#include "renderer.h"
#include "field_manager.h"
#include "object_parts.h"
#include "collision.h"
#include "player.h"
#include "sparks.h"
#include "scene.h"
#include "game.h"

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
// �R���X�g���N�^
//============================================================================
CBoss::CBoss() :
	CCharacter{},
	m_ActionType{ ACTION::HOLDCENTER },
	m_nCntActionCast{ 0 },
	m_nDuration{ 0 },
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CBoss::~CBoss()
{
	// �o�E���f�B���O�V�����_�[�̔j��
	if (m_pBndCylinder != nullptr)
	{
		// �����������
		delete m_pBndCylinder;

		// �|�C���^��������
		m_pBndCylinder = nullptr;
	}
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CBoss::Init()
{
	// �␳���x��ݒ�
	SetCorrectCoef(CORRECT_COEF);

	// �����̗͂�ݒ�
	SetLife(MAX_LIFE);

	// �L�����N�^�[�N���X�̏����ݒ�
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "�{�X�N���X�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CBoss::Uninit()
{
	// �L�����N�^�[�N���X�̏I������
	CCharacter::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CBoss::Update()
{
	// �o�ꃂ�[�V�������I��������
	if (GetNowMotion() == 0 && GetStopState())
	{
		// �ʏ탂�[�V������
		SetNowMotion(1);

		// �s���L���X�g�����Z�b�g
		m_nCntActionCast = 0;

		// �J�����̃v���C���[�Ǐ]�����̎��ɖ߂�
		CManager::GetManager()->GetCamera()->ChangeTrackPlayer(true);
	}

	// �΂̕���Ҕ���������
	CSparks::FuryGenerate();

	// ���̍s��������
	SetNextAction();

	// �s������
	BranchAction();

	// �~���o�E���f�B���O�̒��S�_�����f���̒��S��
	m_pBndCylinder->SetCenterPos(GetPos());

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();

#ifdef _DEBUG
	// �E�B���h�E��\��
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Boss Param"))
	{
		ImGui::Text("ActionType:%d", m_ActionType);
		ImGui::Text("Direction:%.1f", GetDirection() * (180 / D3DX_PI));
		ImGui::Text("DirectionTarget:%.1f", GetDirectionTarget() * (180 / D3DX_PI));
		ImGui::Text("Rot:X %.1f:Y %.1f:Z %.1f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("RotTarget:X %.1f:Y %.1f:Z %.1f", GetRotTarget().x * (180 / D3DX_PI), GetRotTarget().y * (180 / D3DX_PI), GetRotTarget().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.1f:Y %.1f:Z %.1f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::Text("PosTarget:X %.1f:Y %.1f:Z %.1f", GetPosTarget().x, GetPosTarget().y, GetPosTarget().z);
		ImGui::Text("VelY:%.1f", GetVelY());
		ImGui::Text("Life:%d", GetLife());
	}
	ImGui::End();
#endif	// _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CBoss::Draw()
{
	// �L�����N�^�[�N���X�̕`�揈��
	CCharacter::Draw();
}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CBoss::SetDamage(int nDamage)
{
	if (m_ActionType != ACTION::DAMAGEBACK && m_ActionType != ACTION::DEADEND)
	{
		// �_���[�W�ʕ��A�̗͂�ϓ�
		int nNewLife = GetLife();
		nNewLife += nDamage;
		SetLife(nNewLife);

		// �_���[�W��炢�s������������
		m_ActionType = ACTION::DAMAGEBACK;

		// �̗͂��[���ȉ��Ȃ玀�S�s������������
		if (nNewLife <= 0)
			m_ActionType = ACTION::DEADEND;
	}
}

//============================================================================
// ���a���擾
//============================================================================
float CBoss::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// �������擾
//============================================================================
float CBoss::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// �o�E���f�B���O�V�����_�[���擾
//============================================================================
const CBounding_Cylinder* const CBoss::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// ����
//============================================================================
CBoss* CBoss::Create()
{
	// �C���X�^���X�𐶐�
	CBoss* pNewInstance = DBG_NEW CBoss();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::BOSS);

	// �����ݒ�
	pNewInstance->Init();

	// ���[�V�������Z�b�g
	pNewInstance->SetMotion(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\pumpkin_motion.json"));

	// ���a��ݒ�
	pNewInstance->m_pBndCylinder->SetRadius(100.0f);

	// ������ݒ�
	pNewInstance->m_pBndCylinder->SetHeight(60.0f);

	return pNewInstance;
}

//============================================================================
// 
// private�����o
//  
//============================================================================

//============================================================================
// ���̍s��������
//============================================================================
void CBoss::SetNextAction()
{
	// �L���X�g�����l�ɒB�����
	if (m_nCntActionCast > 240)
	{
		// �s���L���X�g�����Z�b�g
		m_nCntActionCast = 0;

		// �����_���ɍs�����Z�b�g
		switch (1)
		{
		case 1:
			m_ActionType = ACTION::DIRECTATTACK;
			break;

		default:	// ��O
#ifdef _DEBUG
			assert(false && "�{�X�̍s������ɗ�O");
#endif // _DEBUG
			break;
		}
	}
}

//============================================================================
// �s������
//============================================================================
void CBoss::BranchAction()
{
	if (m_ActionType == ACTION::HOLDCENTER)
	{ // �s�����ݒ肳��Ă��Ȃ���

		// �s���L���X�g���C���N�������g
		++m_nCntActionCast;

		// �����ɑҋ@
		HoldCenter();
	}
	else
	{ // �s���^�C�v���������Ă��鎞

		// �^�C�v�ɉ����ď�����ύX
		switch (m_ActionType)
		{
			// �̓�����
		case ACTION::DIRECTATTACK:
			DirectAttack();
			break;

			// �_���[�W��炢
		case ACTION::DAMAGEBACK:
			DamageBack();
			break;

			// ���S
		case ACTION::DEADEND:
			DeadEnd();
			break;

			// ��O
		case ACTION::HOLDCENTER:
		default:
#ifdef _DEBUG
			assert(false && "�{�X�̍s���ɗ�O����");
#else
			m_ActionType = ACTION::HOLDCENTER;
#endif // _DEBUG
			break;
		}
	}
}

//============================================================================
// ���S�őҋ@
//============================================================================
void CBoss::HoldCenter()
{
	// �v���C���[���擾
	CPlayer* pPlayer = nullptr;
	pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

	// �v���C���[�̕���������
	Vec3 Rot = VEC3_INIT, Pos = pPlayer->GetPos();
	Rot.y = atan2f(-Pos.x, -Pos.z);
	SetRotTarget(Rot);	

	// �����ɕ�����
	Pos = VEC3_INIT;
	Pos.y = 100.0f;
	SetPosTarget(Pos);
}

//============================================================================
// �̓�����
//============================================================================
void CBoss::DirectAttack()
{
	if (GetNowMotion() != 2)
	{
		// �{�X���u���u�����[�V�����ɕύX
		SetNowMotion(2);

		// ���̏u�Ԃ̃v�����[�̕��p���R�s�[
		CPlayer* pPlayer = nullptr;
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
		SetDirectionTarget(pPlayer->GetDirection());
	}

	// �p�����Ԃ��C���N�������g
	++m_nDuration;

	// �J���������E���Փx�����������ύX
	{
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
		fDinstance += (450.0f - fDinstance) * 0.1f;
		fUpAdjust += (150.0f - fUpAdjust) * 0.05f;
		pCamera->SetDistance(fDinstance);
		pCamera->SetUpAdjust(fUpAdjust);
	}

	// 1.5�b���_��
	if (m_nDuration == 90)
	{
		// �ːi�ڕW�̍��W���쐬
		Vec3 PosTarget = {
			cosf(GetDirection()) * CField_Manager::FIELD_RADIUS,
			30.0f,
			sinf(GetDirection()) * CField_Manager::FIELD_RADIUS
		};

		// �ڕW���W�ɔ��f
		SetPosTarget(PosTarget);
	}

	// 3�b���_��
	if (m_nDuration == 180)
	{
		// �p�����Ԃ����Z�b�g
		m_nDuration = 0;

		// �����ҋ@�ɖ߂�
		m_ActionType = ACTION::HOLDCENTER;

		// �ҋ@���[�V�����ɖ߂�
		SetNowMotion(1);
	}

	// �Փ˔���ɐ���������
	if (HitCheck())
	{
		// �v���C���[��1�_���[�W
		CPlayer* pPlayer = nullptr;
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
		pPlayer->SetDamage(-1);
	}
}

//============================================================================
// �_���[�W��炢
//============================================================================
void CBoss::DamageBack()
{
	if (GetNowMotion() != 3)
	{ // �_���[�W���[�V�������Đ����Ă��Ȃ����

		// �{�X���_���[�W���[�V�����ɕύX
		SetNowMotion(3);
	}
	else if (GetStopState() == true)
	{ // �_���[�W���[�V�������Đ��I��������

		// �s���L���X�g�J�E���g�����Z�b�g
		m_nCntActionCast = 0;

		// �p���J�E���g�����ɖ߂�
		m_nDuration = 0;

		// �ڕW���W�𒆉��֖߂�
		Vec3 Pos = VEC3_INIT;
		Pos.y = 150.0f;
		SetPosTarget(Pos);

		// �����ҋ@�ɖ߂�
		m_ActionType = ACTION::HOLDCENTER;

		// �ҋ@���[�V�����ɖ߂�
		SetNowMotion(1);
	}
}

//============================================================================
// �Փˌ��o
//============================================================================
bool CBoss::HitCheck()
{
	// �v���C���[�̃p�����[�^���R�s�[
	CPlayer* pPlayer = nullptr;
	pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	const CBounding_Cylinder* Other = pPlayer->GetBndCylinder();

	// �����ƃv���C���[�̉~���o�E���f�B���O�Ŕ�ׂ�
	if (collision::HitCylinderToCylinder(m_pBndCylinder, Other))
		return true;

	return false;
}

//============================================================================
// ���S
//============================================================================
void CBoss::DeadEnd()
{
	if (GetNowMotion() != 4)
	{ // ���S���[�V�������Đ����Ă��Ȃ����

		// �{�X�����S���[�V�����ɕύX
		SetNowMotion(4);

		// �����ֈړ�
		Vec3 Pos = VEC3_INIT;
		Pos.y = 150.0f;
		SetPosTarget(Pos);
	}
	else
	{ // ���S���[�V�����Đ���

		// �΂̕���Ҕ���
		for (int nCnt = 0; nCnt < 2; ++nCnt)
			CSparks::FuryGenerate();

		// �J���������E���Փx�����������ύX
		{
			CCamera* pCamera = CManager::GetManager()->GetCamera();
			float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
			fDinstance += (800.0f - fDinstance) * 0.1f;
			fUpAdjust += (300.0f - fUpAdjust) * 0.05f;
			pCamera->SetDistance(fDinstance);
			pCamera->SetUpAdjust(fUpAdjust);

			// �J������U��
			pCamera->SetVibration(0.05f);
		}

		// ���[�V�����̍Đ����I��������Q�[���V�[���ɃQ�[���I����ʒm����
		if (GetStopState())
		{
			// �Q�[���V�[���擾
			CGame* const pScene = dynamic_cast<CGame*>(CScene_Manager::GetInstance()->GetScene());

			// �V�[���J�ڊJ�n
			pScene->SetTransition();
		}
	}
}