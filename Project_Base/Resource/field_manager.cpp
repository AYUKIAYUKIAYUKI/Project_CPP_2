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

// ���͎擾�p
#include "manager.h"

// �f�o�b�O�\���p
#include "renderer.h"

// �I�u�W�F�N�g�p
#include "player.h"
#include "block.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace field_manager;
using namespace player;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CField_Manager* CField_Manager::m_pInstance = nullptr;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CField_Manager::Init()
{
	// �����_���͈͂̐ݒ�
	m_fCoeffRaondomRange = 0.1f;

	// �~���̔���𐶐�
	m_pCylinderCollider = CObject_X::Create();

	// �����ݒ�
	m_pCylinderCollider->Init();

	// ���f����ݒ�
	m_pCylinderCollider->BindModel(CModel_X_Manager::TYPE::CYLINDERCOLLIDER);

	// �����x��ݒ�
	m_pCylinderCollider->SetAlpha(0.5f);

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CField_Manager::Release()
{
	if (m_pInstance != nullptr)
	{
		// �I������
		m_pInstance->Uninit();

		// �����������
		delete m_pInstance;

		// �|�C���^��������
		m_pInstance = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CField_Manager::Update()
{
#ifdef _DEBUG
	CRenderer::GetInstance()->SetDebugString(to_string(m_fCoeffRaondomRange));
#endif

	// ���̐������\�b�h
	TestCreate();

	// ���̔j�����\�b�h
	TestDelete();

	if (CManager::GetKeyboard()->GetTrigger(DIK_BACK))
	{
		// ���̑S�j�����\�b�h
		TestDeleteAll();
	}
}

//============================================================================
// ���C���X�^���X���擾
//============================================================================
CField_Manager* CField_Manager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		// ����
		m_pInstance->Create();
	}

	return m_pInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CField_Manager::CField_Manager() :
	m_fCoeffRaondomRange(0.0f),
	m_pCylinderCollider(nullptr)
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CField_Manager::~CField_Manager()
{

}

//============================================================================
// ����
//============================================================================
void CField_Manager::Create()
{
	if (m_pInstance != nullptr)
	{ // ��d�����֎~
		assert(false);
	}

	// �C���X�^���X�𐶐�
	m_pInstance = DBG_NEW CField_Manager();
}

//============================================================================
// �I������
//============================================================================
void CField_Manager::Uninit()
{

}

//============================================================================
// ���̐������\�b�h
//============================================================================
void CField_Manager::TestCreate()
{
	// �u���b�N�����J�E���g
	int nCntBlock = 0;

	// �~�h���I�u�W�F�N�g�����擾
	CObject* pObj = CObject::GetObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// �u���b�N�^�O�̐����J�E���g����
	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			nCntBlock++;
		}

		// ���J�E���g�ňȍ~�̏������s��Ȃ�
		if (nCntBlock >= MAX_BLOCK)
		{
			return;
		}

		pObj = pObj->GetNext();
	}

	while (nCntBlock < MAX_BLOCK)
	{
		// �v���C���[�^�O���擾
		if (CObject::FindObject(CObject::TYPE::PLAYER))
		{
			// �I�u�W�F�N�g���v���C���[�^�O�Ƀ_�E���L���X�g
			CPlayer* pPlayer = nullptr;
			pPlayer = CUtility::DownCast(pPlayer, CObject::FindObject(CObject::TYPE::PLAYER));

			// �v���C���[�̕��p���R�s�[
			const float& fDirection = pPlayer->GetDirection();

			// �u���b�N�p�̍��W�E�������쐬
			Vec3 NewPos = VEC3_INIT, NewRot = VEC3_INIT;

			// �����_���ȕ��p�͈�
			float fRandomRange = 0.0f;

			// �j���͈͂ɂ͂ݏo�������������悤�ɒ���
			do
			{
				// �����_���ɕ��p�����炷
				fRandomRange = CUtility::GetRandomValue<float>() * m_fCoeffRaondomRange;

				// �����p�̍��W������
				NewPos.x = cosf(fDirection + fRandomRange) * FIELD_RADIUS;
				NewPos.y = fabsf(CUtility::GetRandomValue<float>());
				NewPos.z = sinf(fDirection + fRandomRange) * FIELD_RADIUS;

			} while (!CUtility::CylinderAndPoint(pPlayer->GetPos(), 250.0f, 250.0f, NewPos));

			// ����������
			NewRot.y = -(fDirection + fRandomRange);

			CBlock::Create(NewPos, NewRot);

			nCntBlock++;
		}
	}
}

//============================================================================
// ���̔j�����\�b�h
//============================================================================
void CField_Manager::TestDelete()
{
	// �~�h���I�u�W�F�N�g�����擾
	CObject* pObj = CObject::GetObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// �u���b�N�^�O�̐����J�E���g����
	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �v���C���[�^�O���擾
			if (CObject::FindObject(CObject::TYPE::PLAYER))
			{
				// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
				CBlock* pBlock = nullptr;
				pBlock = CUtility::DownCast(pBlock, pObj);
				
				// �I�u�W�F�N�g���v���C���[�^�O�Ƀ_�E���L���X�g
				CPlayer* pPlayer = nullptr;
				pPlayer = CUtility::DownCast(pPlayer, CObject::FindObject(CObject::TYPE::PLAYER));

				m_pCylinderCollider->SetPos(pPlayer->GetPos());
				m_pCylinderCollider->SetRot(pPlayer->GetRot());
				m_pCylinderCollider->SetScale(250.0f);

				// �t�ɁA�~���͈͊O�̏ꍇ����
				if (!CUtility::CylinderAndPoint(pPlayer->GetPos(), 250.0f, 250.0f, pBlock->GetPos()))
				{
					pBlock->SetRelease();
				}
			}
		}

		pObj = pObj->GetNext();
	}
}

//============================================================================
// ���̔j�����\�b�h
//============================================================================
void CField_Manager::TestDeleteAll()
{
	// �~�h���I�u�W�F�N�g�����擾
	CObject* pObj = CObject::GetObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// �u���b�N�^�O�̐����J�E���g����
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