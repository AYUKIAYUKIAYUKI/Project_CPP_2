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

		// �̗͂������Ȃ�ƃQ�[���V�[���ɃQ�[���I����ʒm����
		if (m_pSyncPlayer->GetLife() <= 0)
		{
			// �Q�[���V�[���擾
			CGame* const pScene = dynamic_cast<CGame*>(CScene_Manager::GetInstance()->GetScene());

			// �V�[���J�ڊJ�n
			pScene->SetTransition();
		}
	}

	// �t�B�[���h�W�F�l���[�^
	if(m_nCntDestroyBlock < MAX_DESTROY_BLOCK)
		FieldGenerator();

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
// �u���b�N�̔j�󐔂��擾
//============================================================================
int CField_Manager::GetCntDestroyBlock()
{
	return m_nCntDestroyBlock;
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
	m_FiledType{ FIELD_TYPE::NORMAL },
	m_nCntDestroyBlock{ 0 },
	m_pSyncPlayer{ nullptr },
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
// �t�B�[���h�W�F�l���[�^
//============================================================================
void CField_Manager::FieldGenerator()
{
	// �t�B�[���h�^�C�v�̕���
	BranchFieldType();

#if !CHANGE_FIELRDCREATE_STYLE
	// �v���C���[�̖ڕW���W�ւ̃x�N�g�����쐬
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	// �v���C���[�̈ړ����̃m�����ɉ����ău���b�N����
	if (Norm.x * Norm.x + Norm.z * Norm.z > 0.1f)
	{
		// �u���b�N�̎�������
		//AutoCreateBlock(1);
		AutoCreateBlockDash();
	}

	// �u���b�N�̎����폜
	AutoDestroyBlock();
#endif	// CHANGE_FIELRDCREATE_STYLE

#ifdef _DEBUG	// �u���b�N��S�폜
	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
		DestroyAllBlock();
#endif // _DEBUG
}

//============================================================================
// �t�B�[���h�^�C�v�̕���
//============================================================================
void CField_Manager::BranchFieldType()
{
	//if (m_ActionData.nCntJump > m_ActionData.nCntDash)
	if (0)
	{
		m_FiledType = FIELD_TYPE::JUMP;
	}
	else
	{
		m_FiledType = FIELD_TYPE::DASH;
	}
}

//============================================================================
// �u���b�N�̎�������
//============================================================================
void CField_Manager::AutoCreateBlock(int nAmount)
{
	// �������W�v�Z�p ((���p + ��`���̊p�x)�̏ꏊ�������|�C���g)
	float fDirection = m_pSyncPlayer->GetDirection();	// �v���C���[�̌��݂̕��p���R�s�[
	float fRange = m_pRenderFan->GetRange();			// ��`�͈͂̕����R�s�[
	Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;		// �u���b�N�p�̍��W�E�������쐬

	// ���ݍ��W�ƖڕW���W�ɑ΂����_����̕����x�N�g�����쐬
	Vec3 OldVec = m_pSyncPlayer->GetPos() - VEC3_INIT, NewVec = m_pSyncPlayer->GetPosTarget() - VEC3_INIT;
	D3DXVec3Normalize(&OldVec, &OldVec);
	D3DXVec3Normalize(&NewVec, &NewVec);

	// 2�{�̕����x�N�g���̊O�ς��쐬
	float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

	// ���Ɉړ����Ă���ꍇ�p�x�𔽓]������
	if (fCross < 0.0f)
		fRange = -fRange;

	// �u���b�N��������ɖ����Ȃ����
	for (int nCntBlock = 0; nCntBlock < nAmount; nCntBlock++)
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
			if (DetectNearBlock(NewPos))
			{
				return;

				//NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
			}

		} while (!m_pRenderFan->DetectInFanRange(NewPos));

		// ����������
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// �u���b�N�𐶐�
		CBlock::Create(NewPos, NewRot);
	}
}

//============================================================================
// �_�b�V���^�C�v�̎�������
//============================================================================
void CField_Manager::AutoCreateBlockDash()
{
	// �������W�v�Z�p ((���p + ��`���̊p�x)�̏ꏊ�������|�C���g)
	float fDirection = m_pSyncPlayer->GetDirection();	// �v���C���[�̌��݂̕��p���R�s�[
	float fRange = m_pRenderFan->GetRange();			// ��`�͈͂̕����R�s�[
	Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;		// �u���b�N�p�̍��W�E�������쐬

	// ���ݍ��W�ƖڕW���W�ɑ΂����_����̕����x�N�g�����쐬
	Vec3 OldVec = m_pSyncPlayer->GetPos() - VEC3_INIT, NewVec = m_pSyncPlayer->GetPosTarget() - VEC3_INIT;
	D3DXVec3Normalize(&OldVec, &OldVec);
	D3DXVec3Normalize(&NewVec, &NewVec);

	// 2�{�̕����x�N�g���̊O�ς��쐬
	float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

	// ���Ɉړ����Ă���ꍇ�p�x�𔽓]������
	if (fCross < 0.0f)
		fRange = -fRange;

	// �j���͈͂ɂ͂ݏo�������������悤�ɒ���
	/* �������W�����_�̏ꍇ�A�����͈͂̔��a���t�B�[���h�̔��a�������Ɩ������[�v */
	do
	{
		// �����p�̍��W������
		NewPos.x = cosf(fDirection + fRange) * FIELD_RADIUS;
		NewPos.y = fabsf(utility::GetRandomValue<float>());
		NewPos.z = sinf(fDirection + fRange) * FIELD_RADIUS;

		// �u���b�N���m�̕������o
		if (DetectNearBlock(NewPos))
		{
			return;

			//NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
		}

	} while (!m_pRenderFan->DetectInFanRange(NewPos));

	// ����������
	NewRot.y = atan2f(-NewPos.x, -NewPos.z);

	// �u���b�N�𐶐�
	CBlock::Create(NewPos, NewRot);
}

//============================================================================
// �אڂ������u���b�N�����o
//============================================================================
bool CField_Manager::DetectNearBlock(D3DXVECTOR3 Pos)
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

			// ���݂��̋��������߂�
			const Vec3& Vec = pBlock->GetPos() - Pos;

			// �u���b�N�̃T�C�Y���R�s�[
			const Vec3& Size = pBlock->GetSize();

			// �u���b�N�̃T�C�Y���炢�ɋ߂Â��Ă�����
			if ((Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= (Size.x * Size.x + Size.y * Size.y + Size.z * Size.z) * 10.0f)
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
void CField_Manager::AutoDestroyBlock()
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

			if (!m_pRenderFan->DetectInFanRange(pBlock->GetPos()))
			{ // ��`�͈͓̔��Ƀu���b�N���������

				// �u���b�N��j��
				pBlock->SetRelease();
				
				// �u���b�N�̔j��J�E���g�𑝉�
				++m_nCntDestroyBlock;

				// �u���b�N�̔j�󐔃J�E���g���ő��
				if (m_nCntDestroyBlock >= MAX_DESTROY_BLOCK)
				{
					// �{�X�o��
					AppearBoss();

					// �S�u���b�N��j��
					DestroyAllBlock();

					// �����������I��
					return;
				}
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
	// �O�ϊm�F�p
	Vec3 OldVec = m_pSyncPlayer->GetPos() - VEC3_INIT, NewVec = m_pSyncPlayer->GetPosTarget() - VEC3_INIT;
	D3DXVec3Normalize(&OldVec, &OldVec);
	D3DXVec3Normalize(&NewVec, &NewVec);
	float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

	// �m�����m�F�p
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Action Data")) {
		ImGui::Text("CountJump:%d", m_ActionData.nCntJump);
		ImGui::Text("CountDash:%d", m_ActionData.nCntDash);
		ImGui::Text("FieldType:%d", m_FiledType);
		ImGui::Text("DestroyBlock:%d", m_nCntDestroyBlock);
		ImGui::Text("Cross:%f", fCross);
		ImGui::Text("Norm:%f", Norm.x * Norm.x + Norm.z * Norm.z);
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