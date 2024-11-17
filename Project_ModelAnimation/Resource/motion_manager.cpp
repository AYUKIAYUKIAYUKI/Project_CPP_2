//============================================================================
// 
// ���[�V�����}�l�[�W���[ [motion_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "motion_manager.h"
#include "object_X.h"
#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�̏�����
//****************************************************
CMotion_Manager* CMotion_Manager::m_pMotionManager = nullptr;	// ���[�V�����}�l�[�W���[�̖{��

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CMotion_Manager::Update()
{
	CRenderer::SetDebugString("��������������������������������������������������������������");
	CRenderer::SetDebugString("�����[�V�������F" + to_string(m_Actor.wMaxMotion));
	CRenderer::SetDebugString("��������������������������������������������������������������");
	for (WORD wCntMotion = 0; wCntMotion < m_Actor.wMaxMotion; ++wCntMotion)
	{
		const Motion* const pMotion = &m_Actor.apMotion[wCntMotion];
		CRenderer::SetDebugString("���݂̃t���[�����@�F" + to_string(pMotion->wNowFrame));
		CRenderer::SetDebugString("���݂̃L�[���@�@�@�F" + to_string(pMotion->wNowKey));
		CRenderer::SetDebugString("���[�v�t���O�@�@�@�F" + to_string(pMotion->bLoop));
		CRenderer::SetDebugString("���L�[���@�@�@�@�@�F" + to_string(pMotion->wMaxKey));
		CRenderer::SetDebugString("��������������������������������������������������������������");

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			const MotionKey* const pKey = &pMotion->apKey[wCntMotionKey];
			CRenderer::SetDebugString("�L�[[" + to_string(wCntMotionKey) + "]�̑��t���[�����F" + to_string(pKey->wMaxFrame));
		}
	}
	CRenderer::SetDebugString("��������������������������������������������������������������");
}

//============================================================================
// ����
//============================================================================
HRESULT CMotion_Manager::Create()
{
	// ���ɐ�������Ă�����
	if (m_pMotionManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("���[�V�����}�l�[�W���[�͊��ɐ�������Ă��܂�", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// �C���X�^���X�𐶐�
	m_pMotionManager = DBG_NEW CMotion_Manager();

	// �������s
	if (m_pMotionManager == nullptr)
	{
		return E_FAIL;
	}

	// ���[�V�����}�l�[�W���[�̏����ݒ�
	if (FAILED(m_pMotionManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CMotion_Manager::Release()
{
	if (m_pMotionManager != nullptr)
	{
		// ���[�V�����}�l�[�W���[�̏I������
		m_pMotionManager->Uninit();

		// �������̉��
		delete m_pMotionManager;

		// �|�C���^�̏�����
		m_pMotionManager = nullptr;
	}
}

//============================================================================
// ���[�V�����}�l�[�W���[���擾
//============================================================================
CMotion_Manager* CMotion_Manager::GetInstance()
{
	// �{�̂����݂��Ȃ����
	if (m_pMotionManager == nullptr)
	{
		// ����
		if (FAILED(Create()))
		{
			assert(false && "���[�V�����}�l�[�W���[�̎擾�Ɏ��s");
		}
	}

	return m_pMotionManager;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CMotion_Manager::CMotion_Manager() :
	m_Actor{ {}, 0, nullptr }
{
	
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CMotion_Manager::~CMotion_Manager()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CMotion_Manager::Init()
{
	// JSON�t�@�C�����烂�[�V���������擾
	JSON Json = utility::OpenJsonFile("Data\\JSON\\motion_test.json");

	// ���p�[�c�����擾
	const WORD& MaxParts = static_cast<WORD>(Json["MaxParts"]);

	// �p�[�c�����̃p�[�c�I�u�W�F�N�g�𐶐�
	for (WORD wCntParts = 0; wCntParts < MaxParts; ++wCntParts)
	{
		m_Actor.vpModelParts.push_back(CObject_X::Create(static_cast<CX_Manager::TYPE>(Json["ModelType"][wCntParts])));
	}

	// �����[�V���������擾
	m_Actor.wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// ���[�V���������̃��[�V�������𐶐�
	m_Actor.apMotion = DBG_NEW Motion[m_Actor.wMaxMotion];

	// ���[�V�������̐ݒ�
	for (WORD wCntMotion = 0; wCntMotion < m_Actor.wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		Motion* const pMotion = &m_Actor.apMotion[wCntMotion];

		// �Đ����̃L�[�ƃt���[����������
		pMotion->wNowKey = 0;
		pMotion->wNowFrame = 0;

		// ���[�v�t���O���擾
		pMotion->bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// ���[�V�����̑��L�[�����擾
		pMotion->wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// �L�[�����̃L�[���𐶐�
		pMotion->apKey = DBG_NEW MotionKey[m_Actor.apMotion[wCntMotion].wMaxKey];

		// �L�[���̐ݒ�
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// �L�[���̃|�C���^���쐬
			MotionKey* const pKey = &pMotion->apKey[wCntMotionKey];

			// �L�[�̑��t���[�������擾
			pKey->wMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotionKey]);

			// �p�[�c�����̖ڕW�l���𐶐�
			pKey->apDest = DBG_NEW MotionDest[m_Actor.vpModelParts.size()];

			for (WORD wCntMotionDest = 0; wCntMotionDest < m_Actor.vpModelParts.size(); ++wCntMotionDest)
			{
				// �ڕW�l���̃|�C���^���쐬
				MotionDest* const pDest = &pKey->apDest[wCntMotionDest];

				// �e��p�����[�^��ݒ�
				pDest->ScaleTarget = utility::JsonConvertToVec3(Json["ScaleTarget"][wCntMotionDest]);	// �ڕW�k��
				pDest->RotTarget = utility::JsonConvertToVec3(Json["RotTarget"][wCntMotionDest]);		// �ڕW����
				pDest->PosTarget = utility::JsonConvertToVec3(Json["PosTarget"][wCntMotionDest]);		// �ڕW���W
			}
		}
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CMotion_Manager::Uninit()
{
	// �L�[�����̖ڕW�l����j��
	for (WORD wCntMotion = 0; wCntMotion < m_Actor.wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		Motion* const pMotion = &m_Actor.apMotion[wCntMotion];

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// �L�[���̃|�C���^���쐬
			MotionKey* const pKey = &pMotion->apKey[wCntMotionKey];

			// �ڕW�l���̃|�C���^�z���j��
			if (pKey->apDest != nullptr)
			{
				delete[] pKey->apDest;
				pKey->apDest = nullptr;
			}
		}
	}

	// ���[�V���������̃L�[����j��
	for (WORD wCntMotion = 0; wCntMotion < m_Actor.wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		Motion* const pMotion = &m_Actor.apMotion[wCntMotion];

		// �L�[���̃|�C���^�z���j��
		if (pMotion->apKey != nullptr)
		{
			delete[] pMotion->apKey;
			pMotion->apKey = nullptr;
		}
	}

	// ���[�V�������̃|�C���^�z���j��
	if (m_Actor.apMotion != nullptr)
	{
		delete[] m_Actor.apMotion;
		m_Actor.apMotion = nullptr;
	}
}