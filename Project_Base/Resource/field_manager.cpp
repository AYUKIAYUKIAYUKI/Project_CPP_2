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

// �I�u�W�F�N�g�p
#include "player.h"
#include "block.h"

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
	// ���̃��\�b�h
	TestMethod();

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
void CField_Manager::TestMethod()
{
	// �u���b�N�����J�E���g
	int nCntBlock = 0;
	constexpr int nMaxBlock = 50;

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
		if (nCntBlock >= nMaxBlock)
		{
			return;
		}

		pObj = pObj->GetNext();
	}

	while (nCntBlock < nMaxBlock)
	{
		// �v���C���[�^�O���擾
		if (CObject::FindObject(CObject::TYPE::PLAYER))
		{
			// �I�u�W�F�N�g���v���C���[�^�O�Ƀ_�E���L���X�g
			CPlayer* pPlayer = nullptr;
			pPlayer = CUtility::DownCast(pPlayer, CObject::FindObject(CObject::TYPE::PLAYER));

			// �v���C���[�̍��W����p�x���v�Z
			D3DXVECTOR3 NewPos = pPlayer->GetPos();
			float f�p�x = atan2f(NewPos.z, NewPos.x), f���f�� = 150.0f, f���� = CUtility::GetRandomValue<float>() * 0.01f;
			NewPos.x = cosf(f�p�x + f����) * f���f��;
			NewPos.y = fabsf(CUtility::GetRandomValue<float>());
			NewPos.z = sinf(f�p�x + f����) * f���f��;

			// ����������
			D3DXVECTOR3 NewRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			NewRot.y = -(f�p�x + f����);

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