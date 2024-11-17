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
	CRenderer::SetDebugString("�����[�V�������F" + to_string(m_Test.wMaxMotion));
	CRenderer::SetDebugString("��������������������������������������������������������������");
	for (WORD wCntMotion = 0; wCntMotion < m_Test.wMaxMotion; ++wCntMotion)
	{
		const Motion* const pMotion = &m_Test.apMotion[wCntMotion];
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
CMotion_Manager::CMotion_Manager()
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

	// �p�[�c�I�u�W�F�N�g�𐶐�
	for (WORD wCntParts = 0; wCntParts < MaxParts; ++wCntParts)
	{
		m_Test.vpModelParts.push_back(CObject_X::Create(static_cast<CX_Manager::TYPE>(Json["ModelType"][wCntParts])));
	}

	// �����[�V���������擾
	m_Test.wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// ���[�V�������𐶐�
	m_Test.apMotion = DBG_NEW Motion[m_Test.wMaxMotion];

	// ���[�V�������̐ݒ�
	for (WORD wCntMotion = 0; wCntMotion < m_Test.wMaxMotion; ++wCntMotion)
	{
		// �Đ����̃L�[�ƃt���[����������
		m_Test.apMotion[wCntMotion].wNowKey = 0;
		m_Test.apMotion[wCntMotion].wNowFrame = 0;

		// ���[�v�t���O���擾
		m_Test.apMotion[wCntMotion].bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// ���[�V�����̑��L�[�����擾
		m_Test.apMotion[wCntMotion].wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// �L�[���𐶐�
		m_Test.apMotion[wCntMotion].apKey = DBG_NEW MotionKey[m_Test.apMotion[wCntMotion].wMaxKey];

		// �L�[���̐ݒ�
		for (WORD wCntMotionKey = 0; wCntMotionKey < m_Test.apMotion[wCntMotion].wMaxKey; ++wCntMotionKey)
		{
			// �L�[�̑��t���[�������擾
			m_Test.apMotion[wCntMotion].apKey[wCntMotionKey].wMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotionKey]);
		}
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CMotion_Manager::Uninit()
{
	// �L�[���̃|�C���^�z���j��
	for (WORD wCntMotion = 0; wCntMotion < m_Test.wMaxMotion; ++wCntMotion)
	{
		if (m_Test.apMotion[wCntMotion].apKey != nullptr)
		{
			delete[] m_Test.apMotion[wCntMotion].apKey;
			m_Test.apMotion[wCntMotion].apKey = nullptr;
		}
	}

	// ���[�V�������̃|�C���^�z���j��
	if (m_Test.apMotion != nullptr)
	{
		delete[] m_Test.apMotion;
		m_Test.apMotion = nullptr;
	}
}