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
	m_fCoeffRaondomRange = 0.01f;

	// �~���̔���𐶐�
	m_pCylinderCollider = CObject_X::Create(static_cast<int>(CObject::LAYER::BACK));

	// �����ݒ�
	m_pCylinderCollider->Init();

	// �v���C���[���i�[
	CPlayer* pPlayer = nullptr;

	// �v���C���[������
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	}
	else
	{
		assert(false && "�v���C���[�̌������ʂ�����܂���ł���");
	}

	// �e��p�����[�^�ݒ�
	m_pCylinderCollider->SetPos(pPlayer->GetPos());
	m_pCylinderCollider->SetRot(pPlayer->GetRot());
	m_pCylinderCollider->SetScale(GENERATE_RANGE_RADIUS);
	m_pCylinderCollider->SetAlpha(0.25f);

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
	// �����_���͈͂̋��x��\��
	CRenderer::GetInstance()->SetDebugString("�����_���͈͂̋��x:" + to_string(m_fCoeffRaondomRange));
#endif

	// ���̐������\�b�h
	TestCreate();

	// ���̔j�����\�b�h
	TestDelete();

	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
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
	m_fCoeffRaondomRange{ 0.0f },
	m_pCylinderCollider{ nullptr }
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
	int nCntBlock = CObject::CountSpecificObject(CObject::TYPE::BLOCK);

#ifdef _DEBUG
	CRenderer::GetInstance()->SetDebugString("�u���b�N��:" + to_string(nCntBlock));
#endif

	// �u���b�N��������ɖ����Ȃ����
	while (nCntBlock < MAX_BLOCK)
	{
		// �v���C���[�^�O���擾
		if (CObject::FindSpecificObject(CObject::TYPE::PLAYER))
		{
			// �I�u�W�F�N�g���v���C���[�^�O�Ƀ_�E���L���X�g
			CPlayer* pPlayer = nullptr;
			pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

			// �������W�v�Z�p
			const float&	fDirection = pPlayer->GetDirection();	// �v���C���[�̕��p���R�s�[
			Vec3			NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// �u���b�N�p�̍��W�E�������쐬
			float			fRandomRange = 0.0f;					// �����_���ȕ��p�͈�

			// �@�v���C���[�̐i�s�����ɉ����Đ��������悤�ɕ␳

			// �j���͈͂ɂ͂ݏo�������������悤�ɒ���
			/* �������W�����_�̏ꍇ�A�����͈͂̔��a���t�B�[���h�̔��a�������Ɩ������[�v */
			do
			{
				// �����_���ɕ��p�����炷
				fRandomRange = CUtility::GetRandomValue<float>() * m_fCoeffRaondomRange;

				// �����p�̍��W������
				NewPos.x = cosf(fDirection + fRandomRange) * FIELD_RADIUS;
				NewPos.y = fabsf(CUtility::GetRandomValue<float>());
				NewPos.z = sinf(fDirection + fRandomRange) * FIELD_RADIUS;

				// �u���b�N���m�̕������o
				if (DetectAdjacentBlock(NewPos))
				{
					NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
				}

			} while (!CUtility::CylinderAndSphere(pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, NewPos, 10.0f));

			// ����������
			NewRot.y = -(fDirection + fRandomRange);

			// �u���b�N�𐶐�
			CBlock::Create(NewPos, NewRot);

			// �u���b�N�����J�E���g�A�b�v
			nCntBlock++;
		}
	}
}

//============================================================================
// �אڂ������u���b�N�����o
//============================================================================
bool CField_Manager::DetectAdjacentBlock(const Vec3& Pos)
{
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// �u���b�N�^�O�̐����J�E���g����
	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = nullptr;
			pBlock = CUtility::DownCast(pBlock, pObj);

			// �������ɂ�����u���b�N�̗אڂ����o����

			/* ����͎����I�ɂ��݂��̋����݂̂��l������ */
			const Vec3& Vec = pBlock->GetPos() - Pos;

			/* ������x�ڋ߂��Ă��܂��Ă���u���b�N�����݂���ꍇ */
			if (sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= pBlock->GetSize().x * 5.0f)
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
// ���̔j�����\�b�h
//============================================================================
void CField_Manager::TestDelete()
{
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	// �u���b�N�^�O�̐����J�E���g����
	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �v���C���[�^�O���擾
			if (CObject::FindSpecificObject(CObject::TYPE::PLAYER))
			{
				// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
				CBlock* pBlock = nullptr;
				pBlock = CUtility::DownCast(pBlock, pObj);
				
				// �I�u�W�F�N�g���v���C���[�^�O�Ƀ_�E���L���X�g
				CPlayer* pPlayer = nullptr;
				pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

				m_pCylinderCollider->SetPos(pPlayer->GetPos());
				m_pCylinderCollider->SetRot(pPlayer->GetRot());

				// �t�ɁA�~���͈͊O�̏ꍇ����
				if (!CUtility::CylinderAndSphere(pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, pBlock->GetPos(), 10.0f))
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
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

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