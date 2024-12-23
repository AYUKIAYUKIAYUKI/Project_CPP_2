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
#include "sound.h"
#include "game.h"
#include "object_PopUp.h"
#include "object_HUD.h"
#include "motion_set.h"
#include "fan.h"
#include "player.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"
#include "boss.h"
#include "block.h"
#include "life.h"
#include "bright.h"
#include "sparks.h"

// �u���b�N�����p
namespace
{
	CBlock* pBlock_DBG = nullptr;
}

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
// �Q�[�������ǉ������ݒ�
//============================================================================
void CField_Manager::InitForGame()
{
	// �����u���b�N�̃Z�b�g
	InitBlockSet();
}

//============================================================================
// �X�V����
//============================================================================
void CField_Manager::Update()
{
	// ���̍X�V�������Q�[���V�[���ȊO�ŌĂ΂�Ă��������s��Ȃ�
	if (typeid(*CScene_Manager::GetInstance()->GetScene()) != typeid(CGame))
		return;

	// �t�F�[�Y�̍X�V
	UpdatePhase();

	// �������̍X�V
	UpdateEnvironment();

	// ��`�̍X�V
	UpdateFan();

	// 4�t�F�[�Y�ȍ~�ɐi�s���Ă�����
	if (m_nPhase > 4)
	{
		// �t�B�[���h�X�V
		UpdateField();

		// �{�X�o��C�x���g
		AppearBossEvent();
	}

	// �J�ڂ�ʒm����
	NotifyTransition();

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebug();
#endif // _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CField_Manager::Draw()
{
#ifdef _DEBUG
	// ��`�͈͂̕`��
	m_pFan->Draw();
#endif // _DEBUG
}

//============================================================================
// ���W���t�B�[���h�T�C�Y�ɒ���
//============================================================================
bool CField_Manager::AdjustPosToFieldSize(CObject* pObj)
{
	// ���[�V�����Z�b�g�N���X�Ƀ_�E���L���X�g
	CMotion_Set* pMotion = utility::DownCast<CMotion_Set, CObject>(pObj);

	// ���݂̍��W����
	Vec3 NewPos = pMotion->GetPos();

	// �p�x������o��
	float fDirection = atan2f(-NewPos.z, NewPos.x);

	// �������p�Ńt�B�[���h�T�C�Y���̍��W�֐�����
	NewPos = {
		cosf(fDirection) * CField_Manager::FIELD_RADIUS,
		NewPos.y,
		sinf(fDirection) * CField_Manager::FIELD_RADIUS
	};

	// ���W���Z�b�g
	pMotion->SetPos(NewPos);

	return true;
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
// �U�������񐔂̃C���N�������g
//============================================================================
void CField_Manager::IncrementCntSlash()
{
	++m_ActionData.nCntSlash;
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
	m_nPhase{ 0 },
	m_pPopUp{ nullptr },
	m_FiledType{ FIELD_TYPE::NORMAL },
	m_nCntDestroyBlock{ 0 },
	m_pSyncPlayer{ nullptr },
	m_pStatue{ nullptr },
	m_nCntStatueVibration{ 0 },
	m_pFan{ nullptr }
{
	// �A�N�V�����f�[�^�̏�����
	m_ActionData.nCntDash = 0;
	m_ActionData.nCntJump = 0;
	m_ActionData.nCntSlash = 0;
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
	// ��`�͈͂𐶐�
	m_pFan = CFan::Create();

	return S_OK;
}

//============================================================================
// �������̏����ݒ�
//============================================================================
void CField_Manager::InitEnvironment()
{
	{ // �����̐���

		// �p�����[�^�𔽉f
		m_pStatue = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_motion.json"));
		auto StatueParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_param.json");
		m_pStatue->SetRot(utility::JsonConvertToVec3(StatueParam["Rot"]));
		m_pStatue->SetPos(utility::JsonConvertToVec3(StatueParam["Pos"]));

		// �������[�V������ݒ�
		m_pStatue->SetNowMotion(2);
	}

	{ // �^���̓����𐶐�

		// �p�����[�^�𔽉f
		CMotion_Set* pTrueStatue = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::LATEDRAW, utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_motion.json"));
		auto StatueParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_param.json");
		pTrueStatue->SetRot(utility::JsonConvertToVec3(StatueParam["Rot"]));
		pTrueStatue->SetPos(utility::JsonConvertToVec3(StatueParam["Pos"]));

		// �j�󃂁[�V������ݒ�
		pTrueStatue->SetNowMotion(2);
	}
}

//============================================================================
// �����u���b�N�̃Z�b�g
//============================================================================
void CField_Manager::InitBlockSet()
{
	// ���p�i�[�p
	float fDirection = 0.0f;

	{ // �����̓����ӂ���

		// �����̃v���C���[�ʒu���猩�č����������p
		fDirection = D3DX_PI * -0.6f;

		// �����u���b�N���c�ɕ��ׂ�
		for (WORD wCntBlock = 0; wCntBlock < 2; wCntBlock++)
		{
			// �V�K�����E�V�K���W�i�[
			Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

			// �����p�̍��W������
			NewPos = utility::DirectionConvertVec3(fDirection, 20.0f + (40.0f * wCntBlock), FIELD_RADIUS);

			// ����������
			NewRot.y = atan2f(-NewPos.x, -NewPos.z);

			// �u���b�N�𐶐�
			CBlock* pBlock = CBlock::Create(NewPos, NewRot);

			// �u���b�N�^�C�v���Œ�
			if (pBlock->GetModelType() != CX_Manager::TYPE::BLOTALL)
			{
				// �����u���b�N���f�������蓖�Ă�
				pBlock->BindModel(CX_Manager::TYPE::BLOTALL);

				// �o�E���f�B���O�T�C�Y�����蓖�Ē���
				pBlock->SetSize(pBlock->GetModel()->Size);
			}
		}
	}

	{ // �E���ɍs����悤�Ƀu���b�N�z�u

		// �E���̃u���b�N�̕��p��p��
		fDirection = D3DX_PI * -0.4f;

		// �V�K�����E�V�K���W�i�[
		Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

		// �����p�̍��W������
		NewPos = utility::DirectionConvertVec3(fDirection, 20.0f, FIELD_RADIUS);

		// ����������
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// �u���b�N�𐶐�
		CBlock* pBlock = CBlock::Create(NewPos, NewRot);

		// �u���b�N�^�C�v���Œ�
		if (pBlock->GetModelType() != CX_Manager::TYPE::BLOTALL)
		{
			// �����u���b�N���f�������蓖�Ă�
			pBlock->BindModel(CX_Manager::TYPE::BLOTALL);

			// �o�E���f�B���O�T�C�Y�����蓖�Ē���
			pBlock->SetSize(pBlock->GetModel()->Size);
		}
	}

	{ // �_�b�V�������邽�ߗ��ꂽ�ꏊ�Ƀu���b�N�z�u

		// �u���b�N�̕��p��p��
		fDirection = D3DX_PI * -0.25f;

		// �V�K�����E�V�K���W�i�[
		Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

		// �����p�̍��W������
		NewPos = utility::DirectionConvertVec3(fDirection, 50.0f, FIELD_RADIUS);

		// ����������
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// �u���b�N�𐶐�
		CBlock* pBlock = CBlock::Create(NewPos, NewRot);

		// �u���b�N�^�C�v���Œ�
		if (pBlock->GetModelType() != CX_Manager::TYPE::BLOSIDE)
		{
			// �������u���b�N���f�������蓖�Ă�
			pBlock->BindModel(CX_Manager::TYPE::BLOSIDE);

			// �o�E���f�B���O�T�C�Y�����蓖�Ē���
			pBlock->SetSize(pBlock->GetModel()->Size);
		}
	}

	{ // �������u���b�N�̂̉��ɖW�Q��

		// �E���̃u���b�N�̕��p��p��
		fDirection = -0.736f;

		// �V�K�����E�V�K���W�i�[
		Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

		// �����p�̍��W������
		NewPos = utility::DirectionConvertVec3(fDirection, 20.0f, FIELD_RADIUS);

		// ����������
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// �u���b�N�𐶐�
		CBlock* pBlock = CBlock::Create(NewPos, NewRot);

		pBlock_DBG = pBlock;

		// �u���b�N�^�C�v���Œ�
		if (pBlock->GetModelType() != CX_Manager::TYPE::BLOTALL)
		{
			// �����u���b�N���f�������蓖�Ă�
			pBlock->BindModel(CX_Manager::TYPE::BLOTALL);

			// �o�E���f�B���O�T�C�Y�����蓖�Ē���
			pBlock->SetSize(pBlock->GetModel()->Size);
		}
	}

	{ // �G���瓦���邽�߂̃u���b�N

		// �����̃v���C���[�ʒu���猩�č����������p
		fDirection = D3DX_PI * -0.12f;

		// �V�K�����E�V�K���W�i�[
		Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

		// �����p�̍��W������
		NewPos = utility::DirectionConvertVec3(fDirection, 30.0f, FIELD_RADIUS);

		// ����������
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// �u���b�N�𐶐�
		CBlock* pBlock = CBlock::Create(NewPos, NewRot);

		// �u���b�N�^�C�v���Œ�
		if (pBlock->GetModelType() != CX_Manager::TYPE::BLOSIDE)
		{
			// �������u���b�N���f�������蓖�Ă�
			pBlock->BindModel(CX_Manager::TYPE::BLOSIDE);

			// �o�E���f�B���O�T�C�Y�����蓖�Ē���
			pBlock->SetSize(pBlock->GetModel()->Size);
		}
	}

	{ // �E���̓����ӂ���

		// �E�[�̕��p��ݒ�
		fDirection = D3DX_PI * -0.01f;

		// �����u���b�N���c�ɕ��ׂ�
		for (WORD wCntBlock = 0; wCntBlock < 3; wCntBlock++)
		{
			// �V�K�����E�V�K���W�i�[
			Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

			// �����p�̍��W������
			NewPos = utility::DirectionConvertVec3(fDirection, 20.0f + (40.0f * wCntBlock), FIELD_RADIUS);

			// ����������
			NewRot.y = atan2f(-NewPos.x, -NewPos.z);

			// �u���b�N�𐶐�
			CBlock* pBlock = CBlock::Create(NewPos, NewRot);

			// �u���b�N�^�C�v���Œ�
			if (pBlock->GetModelType() != CX_Manager::TYPE::BLOTALL)
			{
				// �����u���b�N���f�������蓖�Ă�
				pBlock->BindModel(CX_Manager::TYPE::BLOTALL);

				// �o�E���f�B���O�T�C�Y�����蓖�Ē���
				pBlock->SetSize(pBlock->GetModel()->Size);
			}
		}
	}
}

//============================================================================
// �I������
//============================================================================
void CField_Manager::Uninit()
{
	// ��`�͈͂�j��
	if (m_pFan != nullptr)
	{
		m_pFan->Release();	// ���
		m_pFan = nullptr;	// �|�C���^��������
	}
}

//============================================================================
// �t�F�[�Y�̍X�V
//============================================================================
void CField_Manager::UpdatePhase()
{
	switch (m_nPhase)
	{
	case 0:

		// �|�b�v�A�b�v�𐶐�
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_0.json"));
		}

		// �ŏ��̉E�̃u���b�N���z������
		if (m_pSyncPlayer->GetDirection() > D3DX_PI * -0.4f)
		{
			// ����
			if (m_pPopUp)
			{
				m_pPopUp->Disappear();
				m_pPopUp = nullptr;
			}

			// ���̃t�F�[�Y��
			++m_nPhase;
		}

		break;

	case 1:
		
		// �|�b�v�A�b�v�𐶐�
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_1.json"));
		}
		else
		{
			// �v���C���[�ւ̓���
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// �ڕW�����E�ڕW���W���i�[

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y�������փv���C���[�̕��p���R�s�[
			RotTarget.y += D3DX_PI * -0.5f;					// �J�����̐��ʂ������悤�ɒ���
			m_pPopUp->SetRotTarget(RotTarget);				// �ڕW�������Z�b�g

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// �v���C���[�̉��֔z�u
			PosTarget.y += 30.0f;							// ���₷���悤����������t����
			m_pPopUp->SetPosTarget(PosTarget);				// �ڕW���W���Z�b�g
		}

		// �󒆃_�b�V���K�{�̍���𒴂�����
		if (m_pSyncPlayer->GetDirection() > D3DX_PI * -0.235f)
		{
			// ����
			if (m_pPopUp)
			{
				m_pPopUp->Disappear();
				m_pPopUp = nullptr;
			}

			// ���̃t�F�[�Y��
			++m_nPhase;
		}

		break;

	case 2:

		// �|�b�v�A�b�v�𐶐�
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_2.json"));

			// ���̍ۂɃG�l�~�[������
			CBright::Generate(utility::DirectionConvertVec3(D3DX_PI * -0.12f, 95.0f, CField_Manager::FIELD_RADIUS), CBright::CREATETYPE::FLYER);
		}
		else
		{
			// �v���C���[�ւ̓���
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// �ڕW�����E�ڕW���W���i�[

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y�������փv���C���[�̕��p���R�s�[
			RotTarget.y += D3DX_PI * -0.5f;					// �J�����̐��ʂ������悤�ɒ���
			m_pPopUp->SetRotTarget(RotTarget);				// �ڕW�������Z�b�g

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// �v���C���[�̉��֔z�u
			PosTarget.y += 30.0f;							// ���₷���悤����������t����
			m_pPopUp->SetPosTarget(PosTarget);				// �ڕW���W���Z�b�g
		}

		{ // �M���G�t�F�N�g�̎c�����͈ȍ~�̏��������Ȃ�

			// �ʏ�D��x�̃I�u�W�F�N�g���擾
			CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

			while (pObj != nullptr)
			{
				if (typeid(*pObj) == typeid(CBright))
				{
					return;
				}

				pObj = pObj->GetNext();
			}
		}

		// �G�l�~�[���S�ł�����
		if (!CObject::CountSpecificObject(CObject::TYPE::ENEMY))
		{
			// ����
			if (m_pPopUp)
			{
				m_pPopUp->Disappear();
				m_pPopUp = nullptr;
			}

			// ���̃t�F�[�Y��
			++m_nPhase;
		}

		break;

	case 3:

		// �|�b�v�A�b�v�𐶐�
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_3.json"));
		}
		else
		{
			// �v���C���[�ւ̓���
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// �ڕW�����E�ڕW���W���i�[

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y�������փv���C���[�̕��p���R�s�[
			RotTarget.y += D3DX_PI * -0.5f;					// �J�����̐��ʂ������悤�ɒ���
			m_pPopUp->SetRotTarget(RotTarget);				// �ڕW�������Z�b�g

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// �v���C���[�̉��֔z�u
			PosTarget.y += 30.0f;							// ���₷���悤����������t����
			m_pPopUp->SetPosTarget(PosTarget);				// �ڕW���W���Z�b�g
		}

		// �ŏ��̉E�̃u���b�N���߂�����
		if (m_pSyncPlayer->GetDirection() < D3DX_PI * -0.4f)
		{
			// ����
			if (m_pPopUp)
			{
				m_pPopUp->Disappear();
				m_pPopUp = nullptr;
			}

			// ���̃t�F�[�Y��
			++m_nPhase;
		}

		break;

	case 4:

		// �|�b�v�A�b�v�𐶐�
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_4.json"));

			// ����̂݃A�C�e�����ʒu�ɐ���������
			CItem* pItem = CLife::Create();
			
			// ���p��ݒ�
			pItem->SetDirection(D3DX_PI * 0.5f);

			// �����ɐݒ�
			pItem->SetPosY(200.0f);
		}
		else
		{
			// �v���C���[�ւ̓���
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// �ڕW�����E�ڕW���W���i�[

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y�������փv���C���[�̕��p���R�s�[
			RotTarget.y += D3DX_PI * -0.5f;					// �J�����̐��ʂ������悤�ɒ���
			m_pPopUp->SetRotTarget(RotTarget);				// �ڕW�������Z�b�g

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// �v���C���[�̉��֔z�u
			PosTarget.y += 30.0f;							// ���₷���悤����������t����
			m_pPopUp->SetPosTarget(PosTarget);				// �ڕW���W���Z�b�g
		}

		break;
	}

	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Block Edit")) {
		if (!pBlock_DBG)
		{
			
		}
		ImGui::End();
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
// ��`�̍X�V
//============================================================================
void CField_Manager::UpdateFan()
{
	// �v���C���[��񂪃Z�b�g����Ă��Ȃ���Ώ������s��Ȃ�
	if (m_pSyncPlayer == nullptr)
		return;

	// �v���C���[�̌��݂̕��p���`�̕��p�ɂ���
	m_pFan->SetDirection(m_pSyncPlayer->GetDirection());

	// ��`�͈͂̍X�V����
	m_pFan->Update();
}

//============================================================================
// �t�B�[���h�X�V
//============================================================================
void CField_Manager::UpdateField()
{
	// �u���b�N�̔j��J�E���g������ɒB���Ă����珈�����s��Ȃ�
	if (m_nCntDestroyBlock >= MAX_DESTROY_BLOCK)
		return;

	// �t�B�[���h�^�C�v�̕���
	BranchFieldType();

	// �A�C�e���̎�������
	AutoCreateItem();

	// �v���C���[�̖ڕW���W�ւ̃x�N�g�����쐬
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	// �v���C���[�̈ړ����̃m�����ɉ����ău���b�N����
	if (Norm.x * Norm.x + Norm.z * Norm.z > 0.1f)
	{
		// �u���b�N�̎�������
		AutoCreateBlockDash();
	}

	// �u���b�N�̎����폜
	AutoDestroyBlock();

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
	/* �����͉��炩�̕����ݒ�\��ł� */
	m_FiledType = FIELD_TYPE::JUMP;
}

//============================================================================
// �A�C�e���̎�������
//============================================================================
void CField_Manager::AutoCreateItem()
{
	// ���ɃA�C�e����1�ȏ㑶�݂��Ă���Ώ��������Ȃ�
	if (CObject::FindSpecificObject(CObject::TYPE::ITEM))
		return;

	// �A�C�e���p�|�C���^
	CItem* pItem = nullptr;

	// �A�C�e���𐶐�
	/* �����͉��炩�̕����ݒ�\��ł�*/
	pItem = CLife::Create();

	do { // ���̕��p�ɂ�������W���A��`�͈͓��ł���Ε��p���Ē��I����

		// ���p�������_���ɐݒ�
		pItem->SetDirection(fabsf(utility::GetRandomValue<float>()));

	} while (m_pFan->DetectInFanRange(pItem->GetPos()));

	// Y���W�������_���ɐݒ�
	pItem->SetPosY(fabsf(utility::GetRandomValue<float>()));
}

//============================================================================
// �_�b�V���^�C�v�̎�������
//============================================================================
void CField_Manager::AutoCreateBlockDash()
{
	// �������W�v�Z�p ((���p + ��`���̊p�x)�̏ꏊ�������|�C���g)
	float fDirection = m_pSyncPlayer->GetDirection();	// �v���C���[�̌��݂̕��p���R�s�[
	float fRange = m_pFan->GetRange();					// ��`�͈͂̕����R�s�[
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
		}

	} while (!m_pFan->DetectInFanRange(NewPos));

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
	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

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
	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			if (!m_pFan->DetectInFanRange(pBlock->GetPos()))
			{ // ��`�͈͓̔��Ƀu���b�N���������

				// �u���b�N��j��
				pBlock->SetRelease();
				
				// �u���b�N�̔j��J�E���g�𑝉�
				++m_nCntDestroyBlock;

				// �u���b�N�̔j�󐔃J�E���g���ő��
				if (m_nCntDestroyBlock >= MAX_DESTROY_BLOCK)
				{
					// ������U�����[�V�����ɕύX
					m_pStatue->SetNowMotion(0);

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
	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

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
// �{�X�o��C�x���g
//============================================================================
void CField_Manager::AppearBossEvent()
{
	// �����̃��[�V�����Đ����I�����Ă����炱�̃��\�b�h�𖳎�
	if (m_pStatue->GetStopState())
		return;

	// �������ҋ@���[�V�����Ȃ炱�̃��\�b�h�𖳎�
	if (m_pStatue->GetNowMotion() == 2)
		return;

	// �t�H�O�͈͂��g��
	CRenderer::CorrectFogEnd(1350.0f);

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

	// BGM��J��
	CSound::GetInstance()->Transition(CSound::LABEL::FILEDBGM, CSound::LABEL::BOSSBGM);
}

//============================================================================
// �J�ڂ�ʒm����
//============================================================================
void CField_Manager::NotifyTransition()
{
	// �v���C���[��񂪃Z�b�g����Ă��Ȃ���Ώ������s��Ȃ�
	if (m_pSyncPlayer == nullptr)
		return;

	// �̗͂������Ȃ�ƃQ�[���V�[���ɃQ�[���I����ʒm����
	if (m_pSyncPlayer->GetLife() <= 0)
	{
		// �Q�[���V�[���擾
		CGame* const pScene = dynamic_cast<CGame*>(CScene_Manager::GetInstance()->GetScene());

		// �V�[���J�ڊJ�n
		pScene->SetTransition();
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

	// �{�X���Ăяo���f�o�b�O�R�}���h
	if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT) &&
		CManager::GetKeyboard()->GetTrigger(DIK_SPACE))
	{
		m_nCntDestroyBlock = MAX_DESTROY_BLOCK;
		m_pStatue->SetNowMotion(0);
		DestroyAllBlock();
	}
}