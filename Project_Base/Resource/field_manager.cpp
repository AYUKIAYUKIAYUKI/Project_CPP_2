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
#include "field_builder.h"
#include "field_type.h"
#include "object_HUD.h"
#include "object_PopUp.h"
#include "motion_set.h"
#include "player.h"

// �V�X�e������
#include "manager.h"
#include "renderer.h"
//#include "scene.h"
#include "sound.h"
#include "game.h"

// �����I�u�W�F�N�g
#include "boss.h"
#include "block.h"
#include "life.h"
#include "bright.h"
#include "sparks.h"

/* �f�o�b�O�p */
namespace
{
	// �����z�u�u���b�N�̒������|�C���^
	CBlock* pAdjusrInitBlock = nullptr;
}

/* �C���p */
#define SAFE 1

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CField_Manager* CField_Manager::m_pFieldManager = nullptr;	// �t�B�[���h�}�l�[�W���[�̖{��
bool			CField_Manager::m_bRetry = false;			// ���񔻒�

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
	if (!m_pFieldBuilder)
	{
		// �t�B�[���h�r���_�[�̐���
		m_pFieldBuilder = CField_Builder::Create();

		// �t�B�[���h�r���_�[�Ƀv���C���[���Z�b�g
		m_pFieldBuilder->SetSyncPlayer(m_pSyncPlayer);
	}

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

	// 5�t�F�[�Y�ȍ~�ɐi�s���Ă�����
	if (m_nPhase >= 5)
	{
		// �t�B�[���h�r���_�[�̍X�V
		m_pFieldBuilder->Update();

		// �{�X�o��C�x���g
		AppearBossEvent();
	}

	// �V�[���J�ڂ�ʒm����
	NotifySceneTransition();

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
	// �t�B�[���h�r���_�[�̕`�揈��
	m_pFieldBuilder->Draw();
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
// �ɂ��ӂ������ӂ�����
//============================================================================
void CField_Manager::AAABBB()
{
	// ������U�����[�V�����ɕύX
	m_pStatue->SetNowMotion(0);

	// ���Őݒ�
	if (m_pPopUp)
	{
		m_pPopUp->SetDisappear();
		m_pPopUp = nullptr;
	}
}

//============================================================================
// �t�B�[���h�r���_�[�̎擾
//============================================================================
CField_Builder* const CField_Manager::GetFieldBuilder() const
{
	return m_pFieldBuilder;
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
	m_pFieldBuilder{ nullptr },
	m_nPhase{ 0 },
	m_pPopUp{ nullptr },
	m_pStatue{ nullptr },
	m_nCntStatueVibration{ 0 },
	m_pSyncPlayer{ nullptr }
{
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
	/* ���݂͖��� */

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
	// �t�B�[���h�r���_�[�̔j��
	if (m_pFieldBuilder)
	{
		// �������
		m_pFieldBuilder->Release();
	
		// �|�C���^��������
		m_pFieldBuilder = nullptr;
	}
}

//============================================================================
// �t�F�[�Y�̍X�V
//============================================================================
void CField_Manager::UpdatePhase()
{
#if 0
	// 2�T�ڂȂ�
	if (m_bRetry && m_nPhase == 0)
	{
		// ����̃t�F�[�Y��ύX
		m_nPhase = 11;
	}
#endif

	/* �u���b�N�̔j��ʂ��擾 */
	const int nCntDestroyBlock = m_pFieldBuilder->GetCntDestroyBlock();

	/* �u���b�N�̔j��ʂ��ő�ŋ����I�ɍŏI�t�F�[�Y�� */
	if (nCntDestroyBlock >= MAX_DESTROY_BLOCK)
	{
		m_nPhase = 10;
	}

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
			// ���Őݒ�
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
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
			// ���Őݒ�
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
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
			// ���Őݒ�
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
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

			/* �G�l�~�[�𐶐����Ă��� */
			CBright::Generate(utility::DirectionConvertVec3(D3DX_PI * -0.5f, 50.0f, CField_Manager::FIELD_RADIUS), CBright::CREATETYPE::GHOST);
			CBright::Generate(utility::DirectionConvertVec3(D3DX_PI * -0.35f, 50.0f, CField_Manager::FIELD_RADIUS), CBright::CREATETYPE::MONSTER);
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
			// ���Őݒ�
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
				m_pPopUp = nullptr;
			}

			// ���̃t�F�[�Y��
			++m_nPhase;
		}

		break;

	case 4:

		{ // �A�C�e���𐶐�
			CItem* pItem = CLife::Create();

			// ���p��ݒ�
			pItem->SetDirection(D3DX_PI * 0.5f);

			// �����ɐݒ�
			pItem->SetPosY(100.0f);

			// �`��O�Ɉ�x�X�V
			pItem->Update();
		}

		// ���̃t�F�[�Y��
		++m_nPhase;

		break;

	case 5:

		// �ړ��ɂ��n�`�̍Đ������A�����s���Ă������Ղ������
		if (nCntDestroyBlock >= 10)
		{
			// ���̃t�F�[�Y��
			++m_nPhase;
		}

		break;

	case 6:

		// �|�b�v�A�b�v�𐶐�
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_4.json"));
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

		{ // �A�C�e���I�u�W�F�N�g���擾
			CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

			// �A�C�e���I�u�W�F�N�g�������Ă�����
			if (!pObj)
			{
				// ���Őݒ�
				if (m_pPopUp)
				{
					m_pPopUp->SetDisappear();
					m_pPopUp = nullptr;
				}

				// ���̃t�F�[�Y��
				++m_nPhase;
			}
		}

		break;

	case 7:

		// �|�b�v�A�b�v�𐶐�
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_5.json"));
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

		{ // �A�C�e���I�u�W�F�N�g���擾
			CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

			// �A�C�e���I�u�W�F�N�g�������o������
			if (pObj)
			{
#if SAFE
				// �A�C�e���N���X�Ƀ_�E���L���X�g
				CItem* pItem = utility::DownCast<CItem, CObject>(pObj);

				Vec3 Distance = m_pSyncPlayer->GetPos() - pItem->GetPos();
				float fDistanceNorm = Distance.x * Distance.x + Distance.z * Distance.z;

				// �A�C�e���ɐڋ߂�����
				if (fDistanceNorm <= CField_Type::GetAreaNorm())
				{
					// ���Őݒ�
					if (m_pPopUp)
					{
						m_pPopUp->SetDisappear();
						m_pPopUp = nullptr;
					}

					// ���̃t�F�[�Y��
					++m_nPhase;
				}
#endif // SAFE
			}
		}

		break;

	case 8:

		{ // �A�C�e���I�u�W�F�N�g���擾
			CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

			// �A�C�e���I�u�W�F�N�g�������Ȃ��Ă�����
			if (!pObj)
			{
				// ���̃t�F�[�Y��
				++m_nPhase;
			}
		}

		break;

	case 9:

		// �|�b�v�A�b�v�𐶐�
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_6.json"));
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

		{ // �A�C�e���I�u�W�F�N�g���擾
			CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

			// �A�C�e���I�u�W�F�N�g�������Ȃ��Ă�����
			if (!pObj)
			{
				// ���Őݒ�
				if (m_pPopUp)
				{
					m_pPopUp->SetDisappear();
					m_pPopUp = nullptr;
				}

				// �t�F�[�Y�i�s
				++m_nPhase;
			}
		}

		break;

		// �I�_�ƂȂ�t�F�[�Y
	case 10:

		// ���񔻒���o��
		m_bRetry = true;

		//// �|�b�v�A�b�v�ɏ��Őݒ�
		//if (m_pPopUp)
		//{
		//	m_pPopUp->SetDisappear();
		//	m_pPopUp = nullptr;
		//}

		break;

		// 2�T�ڈȍ~�͂��̃t�F�[�Y
	case 11:

		// �|�b�v�A�b�v�𐶐�
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_ex.json"));
		}

		// ������x�ړ����Ă�����
		if (nCntDestroyBlock > 25)
		{
			// ���Őݒ�
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
				m_pPopUp = nullptr;
			}

			// ���̃t�F�[�Y��
			++m_nPhase;
		}

		break;
	}
}

//============================================================================
// �������̍X�V
//============================================================================
void CField_Manager::UpdateEnvironment()
{
	// �΂̕��𐶐�
	CSparks::AutoGenerate();

#ifdef _DEBUG
	CSparks::EditInitParam();
#endif // _DEBUG
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

	/* �ꎞ�I�Ƀ{�X�o��C�x���g���ɃA�C�e���������폜 */
#if SAFE

	{ // �A�C�e���I�u�W�F�N�g���擾
		CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

		// �A�C�e���I�u�W�F�N�g�������o������
		if (pObj)
		{
			// ���̃I�u�W�F�N�g�ɔj���\��
			pObj->SetRelease();
		}
	}

#endif
}

//============================================================================
// �V�[���J�ڂ�ʒm����
//============================================================================
void CField_Manager::NotifySceneTransition()
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
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Field Manager Data")) {
		ImGui::Text("Phase:%d", m_nPhase);
		ImGui::End();
	}

#if 1	// �t�F�[�Y�X�L�b�v
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Skip Phase")) {
		if (ImGui::Button("Skip to 5")) {
			m_nPhase = 5;
		}
		ImGui::End();
	}
#endif

#if 0	// �����z�u�u���b�N�̒���
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("InitBlock Edit")) {
		if (!pAdjusrInitBlock)
		{

		}
		ImGui::End();
	}
#endif

#if 1	// �{�X���Ăяo��
	if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT) &&
		CManager::GetKeyboard()->GetTrigger(DIK_SPACE))
	{
		m_nPhase = 11;
		m_pStatue->SetNowMotion(0);
	}
#endif
}