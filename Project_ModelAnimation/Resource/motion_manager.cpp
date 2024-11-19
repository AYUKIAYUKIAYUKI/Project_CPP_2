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
#include "object_Parts.h"
#include "manager.h"
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
	// �ҏW
	Edit();

	if (m_bPlay)
	{
		// ����
		m_MotionSet.Animation();
	}

#if 1
	CRenderer::SetDebugString("��������������������������������������������������������������");
	CRenderer::SetDebugString("�����[�V�������F" + to_string(m_MotionSet.m_wMaxMotion));
	CRenderer::SetDebugString("��������������������������������������������������������������");
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet.m_wMaxMotion; ++wCntMotion)
	{
		const CMotion_Set::Motion* const pMotion = &m_MotionSet.m_apMotion[wCntMotion];
		CRenderer::SetDebugString("��������������������������������������������������������������");
		CRenderer::SetDebugString("<�Đ��ҋ@���[�V�������>");
		CRenderer::SetDebugString("���݂̃t���[�����@�F" + to_string(m_MotionSet.m_wNowFrame));
		CRenderer::SetDebugString("���݂̃L�[���@�@�@�F" + to_string(m_MotionSet.m_wNowKey));
		CRenderer::SetDebugString("���[�v�t���O�@�@�@�F" + to_string(pMotion->bLoop));
		CRenderer::SetDebugString("���L�[���@�@�@�@�@�F" + to_string(pMotion->wMaxKey));
		CRenderer::SetDebugString("�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[");
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			const CMotion_Set::Key* const pKey = &pMotion->apKey[wCntMotionKey];
			CRenderer::SetDebugString("�L�[[" + to_string(wCntMotionKey) + "]�̑��t���[�����F" + to_string(pKey->wMaxFrame));
		}
	}
	CRenderer::SetDebugString("��������������������������������������������������������������");
	CRenderer::SetDebugString("���݂̑I���p�[�c�@�@�F" + to_string(m_wSelectParts));
	CRenderer::SetDebugString("���W�ҏW�̋��x�@�@�@�F" + to_string(m_fPosEditCoef));
	CRenderer::SetDebugString("���݂̑I�����[�V�����F" + to_string(m_wSelectMotion));
	CRenderer::SetDebugString("���݂̑I���L�[�@�@�@�F" + to_string(m_wSelectKey));
	for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet.m_vpModelParts.size(); ++wCntModelParts)
	{
		const CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[wCntModelParts];
		CRenderer::SetDebugString("Scale�F" + utility::ToPrecision(pDest->ScaleTarget.x) + "�F" + utility::ToPrecision(pDest->ScaleTarget.y) + "�F" + utility::ToPrecision(pDest->ScaleTarget.z) + "�F"
			"Rot�F" + utility::ToPrecision(pDest->RotTarget.x) + "�F" + utility::ToPrecision(pDest->RotTarget.y) + "�F" + utility::ToPrecision(pDest->RotTarget.z) + "�F"
			"Pos�F" + utility::ToPrecision(pDest->PosTarget.x) + "�F" + utility::ToPrecision(pDest->PosTarget.y) + "�F" + utility::ToPrecision(pDest->PosTarget.z));
	}
	CRenderer::SetDebugString("��������������������������������������������������������������");
#endif

	// ���Z�b�g
	Reset();
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
	m_MotionSet{/* 0, 0, 0, {}, 0, nullptr */},
	m_wSelectParts{ 0 },
	m_fPosEditCoef{ 0.0f },
	m_wSelectMotion{ 0 },
	m_wSelectKey{ 0 },
	m_bPlay{ false }
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
	//m_Json = utility::OpenJsonFile("Data\\JSON\\player_motion.json");
	m_Json = utility::OpenJsonFile("Data\\JSON\\motion_export.json");

	// ���p�[�c�����擾
	const WORD& MaxParts = static_cast<WORD>(m_Json["MaxParts"]);

	// �p�[�c�����̃p�[�c�I�u�W�F�N�g���s���Đ���
	for (WORD wCntParts = 0; wCntParts < MaxParts; ++wCntParts)
	{
		m_MotionSet.m_vpModelParts.push_back(CObject_Parts::Create(static_cast<CX_Manager::TYPE>(m_Json["ModelType"][wCntParts]), nullptr));
	}

	// �������ꂽ�p�[�c�ɑ΂��A�e��ݒ���s��
	for (WORD wCntParts = 0; wCntParts < MaxParts; ++wCntParts)
	{
		// �e�p�[�c�̃C���f�b�N�X
		const SHORT& shParentIdx = static_cast<SHORT>(m_Json["ParentIdx"][wCntParts]);

		// �p�[�c�̃|�C���^���R�s�[
		CObject_Parts* pParts = m_MotionSet.m_vpModelParts[wCntParts];

		if (shParentIdx == -1)
		{
			pParts->SetParent(nullptr);
		}
		else
		{
			pParts->SetParent(m_MotionSet.m_vpModelParts.at(shParentIdx));
		}

		// �I�t�Z�b�g�l��ݒ�
		pParts->SetPosOffset(utility::JsonConvertToVec3(m_Json["PosOffset"][wCntParts]));
	}

	// �����[�V���������擾
	m_MotionSet.m_wMaxMotion = static_cast<WORD>(m_Json["MaxMotion"]);

	// ���[�V���������̃��[�V�������𐶐�
	m_MotionSet.m_apMotion = DBG_NEW CMotion_Set::Motion[m_MotionSet.m_wMaxMotion];

	// ���[�V�������̐ݒ�
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet.m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		CMotion_Set::Motion* const pMotion = &m_MotionSet.m_apMotion[wCntMotion];

		// ���[�v�t���O���擾
		pMotion->bLoop = static_cast<bool>(m_Json["Loop"][wCntMotion]);

		// ���[�V�����̑��L�[�����擾
		pMotion->wMaxKey = static_cast<WORD>(m_Json["MaxKey"][wCntMotion]);

		// �L�[�����̃L�[���𐶐�
		pMotion->apKey = DBG_NEW CMotion_Set::Key[pMotion->wMaxKey];

		// �L�[���̐ݒ�
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// �L�[���̃|�C���^���쐬
			CMotion_Set::Key* const pKey = &pMotion->apKey[wCntMotionKey];

			// �L�[�̑��t���[�������擾
			pKey->wMaxFrame = static_cast<WORD>(m_Json["MaxFrame"][wCntMotionKey]);

			// �p�[�c�����̖ڕW�l���𐶐�
			pKey->apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet.m_vpModelParts.size()];

			for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet.m_vpModelParts.size(); ++wCntModelParts)
			{
				// �ڕW�l���̃|�C���^���쐬
				CMotion_Set::KeyDest* const pDest = &pKey->apDest[wCntModelParts];

				// �e��p�����[�^��ݒ�
				pDest->ScaleTarget = utility::JsonConvertToVec3(m_Json["ScaleTarget"][wCntMotionKey][wCntModelParts]);	// �ڕW�k��
				pDest->RotTarget = utility::JsonConvertToVec3(m_Json["RotTarget"][wCntMotionKey][wCntModelParts]);		// �ڕW����
				pDest->PosTarget = utility::JsonConvertToVec3(m_Json["PosTarget"][wCntMotionKey][wCntModelParts]);		// �ڕW���W
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
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet.m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		CMotion_Set::Motion* const pMotion = &m_MotionSet.m_apMotion[wCntMotion];

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// �L�[���̃|�C���^���쐬
			CMotion_Set::Key* const pKey = &pMotion->apKey[wCntMotionKey];

			// �ڕW�l���̃|�C���^�z���j��
			if (pKey->apDest != nullptr)
			{
				delete[] pKey->apDest;
				pKey->apDest = nullptr;
			}
		}
	}

	// ���[�V���������̃L�[����j��
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet.m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		CMotion_Set::Motion* const pMotion = &m_MotionSet.m_apMotion[wCntMotion];

		// �L�[���̃|�C���^�z���j��
		if (pMotion->apKey != nullptr)
		{
			delete[] pMotion->apKey;
			pMotion->apKey = nullptr;
		}
	}

	// ���[�V�������̃|�C���^�z���j��
	if (m_MotionSet.m_apMotion != nullptr)
	{
		delete[] m_MotionSet.m_apMotion;
		m_MotionSet.m_apMotion = nullptr;
	}

	// �p�[�c�p�I�u�W�F�N�g�̏���
	for (auto it : m_MotionSet.m_vpModelParts)
	{
		// �j���\��
		it->SetRelease();
	}
}

//============================================================================
// �ҏW
//============================================================================
void CMotion_Manager::Edit()
{
	// �Đ��؂�ւ�
	if (CManager::GetKeyboard()->GetTrigger(DIK_F5))
	{
		m_bPlay = !m_bPlay;
	}

	// �p�[�c���̕ҏW
	EditParts();

	// ���[�V�������̕ҏW
	EditMotion();

	// �L�[���̕ҏW
	EditKey();

	// �t���[�����̕ҏW
	EditFrame();

	// �G�N�X�|�[�g
	if (CManager::GetKeyboard()->GetTrigger(DIK_F1))
	{
		Export();
	}
}

//============================================================================
// �p�[�c���̕ҏW
//============================================================================
void CMotion_Manager::EditParts()
{
	// �I���p�[�c�؂�ւ�
	if (CManager::GetKeyboard()->GetTrigger(DIK_1))
	{
		m_wSelectParts > 0 ? m_wSelectParts-- : m_wSelectParts = static_cast<WORD>(m_MotionSet.m_vpModelParts.size()) - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
	{
		m_wSelectParts < m_MotionSet.m_vpModelParts.size() - 1 ? m_wSelectParts++ : m_wSelectParts = 0;
	}

	// �I���p�[�c�𓧉�
	for (WORD wCntParts = 0; wCntParts < m_MotionSet.m_vpModelParts.size(); ++wCntParts)
	{
		if (m_wSelectParts == wCntParts)
		{
			m_MotionSet.m_vpModelParts[wCntParts]->SetCol({ 1.0f, 0.5f, 0.25f, 0.25f });
			m_MotionSet.m_vpModelParts[wCntParts]->SetUseCol(true);
		}
		else
		{
			m_MotionSet.m_vpModelParts[wCntParts]->SetUseCol(false);
		}
	}

	// �ڕW�l���̕ҏW
	EditDest();
}

//============================================================================
// �ڕW�l���̕ҏW
//============================================================================
void CMotion_Manager::EditDest()
{
	// �I�𒆂̃p�[�c�̖ڕW�l���̃|�C���^���쐬
	CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[m_wSelectParts];

	if (CManager::GetKeyboard()->GetTrigger(DIK_U))
	{
		pDest->RotTarget.x += D3DX_PI * 0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_O))
	{
		pDest->RotTarget.x += D3DX_PI * -0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_I))
	{
		pDest->RotTarget.y += D3DX_PI * 0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_K))
	{
		pDest->RotTarget.y += D3DX_PI * -0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_J))
	{
		pDest->RotTarget.z += D3DX_PI * 0.05f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_L))
	{
		pDest->RotTarget.z += D3DX_PI * -0.05f;
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_R))
	{
		m_fPosEditCoef += -0.1f;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_Y))
	{
		m_fPosEditCoef += 0.1f;
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_F))
	{
		pDest->PosTarget.x += -m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_H))
	{
		pDest->PosTarget.x += m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_T))
	{
		pDest->PosTarget.y += m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_G))
	{
		pDest->PosTarget.y += -m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_V))
	{
		pDest->PosTarget.z += -m_fPosEditCoef;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_N))
	{
		pDest->PosTarget.z += m_fPosEditCoef;
	}

	// �ڕW�l�𔽉f
	m_Json["ScaleTarget"][m_wSelectKey][m_wSelectParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
	m_Json["RotTarget"][m_wSelectKey][m_wSelectParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
	m_Json["PosTarget"][m_wSelectKey][m_wSelectParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
}

//============================================================================
// ���[�V�������̕ҏW
//============================================================================
void CMotion_Manager::EditMotion()
{
	// �I�����[�V�����؂�ւ�
	if (CManager::GetKeyboard()->GetTrigger(DIK_3))
	{
		m_wSelectMotion > 0 ? m_wSelectMotion-- : m_wSelectMotion = m_MotionSet.m_wMaxMotion - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_4))
	{
		m_wSelectMotion < m_MotionSet.m_wMaxMotion - 1 ? m_wSelectMotion++ : m_wSelectMotion = 0;
	}
}

//============================================================================
// �L�[���̕ҏW
//============================================================================
void CMotion_Manager::EditKey()
{
	// �I���L�[�؂�ւ�
	if (CManager::GetKeyboard()->GetTrigger(DIK_5))
	{
		m_wSelectKey > 0 ? m_wSelectKey-- : m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_6))
	{
		m_wSelectKey < GetSelectMotion()->wMaxKey - 1 ? m_wSelectKey++ : m_wSelectKey = 0;
	}

	/*
	// ���L�[���̐؂�ւ�
	if (CManager::GetKeyboard()->GetTrigger(DIK_F2))
	{
		GetSelectMotion()->wMaxKey--;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F3))
	{
		GetSelectMotion()->wMaxKey++;
	}
	else if (CManager::GetKeyboard()->GetRelease(DIK_F2) || CManager::GetKeyboard()->GetRelease(DIK_F3))
	{
		// �L�[���̍Ċm��
		ResizeKey();
	}
	*/
}

//============================================================================
// �L�[���̍Ċm��
//============================================================================
void CMotion_Manager::ResizeKey()
{
	/* �������Ȃ��ł�����ƃR�s�[���邱�ƁI */
}

//============================================================================
// �t���[�����̕ҏW
//============================================================================
void CMotion_Manager::EditFrame()
{
	// ���t���[������
	if (CManager::GetKeyboard()->GetTrigger(DIK_7) && m_MotionSet.GetNowKey()->wMaxFrame > 1)
	{
		GetSelectKey()->wMaxFrame--;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_8))
	{
		GetSelectKey()->wMaxFrame++;
	}
	else if (CManager::GetKeyboard()->GetRelease(DIK_7) || CManager::GetKeyboard()->GetRelease(DIK_8))
	{
		m_Json["MaxFrame"][m_wSelectKey] = GetSelectKey()->wMaxFrame;
	}
}

//============================================================================
// �G�N�X�|�[�g
//============================================================================
void CMotion_Manager::Export()
{
	// �t�@�C�������쐬
	std::string FileName = "Data\\JSON\\motion_export.json";

	// �t�@�C���������o���W�J
	std::ofstream Ofs(FileName, std::ios::out);

	if (Ofs.good())
	{
		// �W�F�C�\���f�[�^���V���A���C�Y
		Ofs << m_Json.dump(1, '	');	// ������ -> �C���f���g��, ������ -> �C���f���g�`��

		CRenderer::SetTimeString("���[�V�����f�[�^[" + FileName + "]�������o���܂���", 120);
	}
	else
	{
		assert(false && "�t�@�C���̏������݂Ɏ��s");
	}
}

//============================================================================
// ���Z�b�g
//============================================================================
void CMotion_Manager::Reset()
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_F7))
	{
		// ��U���
		Release();

		// ���Đ���
		Create();
	}
}

//============================================================================
// �I�𒆂̃p�[�c�̃|�C���^���擾
//============================================================================
CObject_Parts* const CMotion_Manager::GetSelectParts() const
{
	return m_MotionSet.m_vpModelParts[m_wSelectParts];
}

//============================================================================
// �I�𒆂̃��[�V�����̃|�C���^���擾
//============================================================================
CMotion_Set::Motion* const CMotion_Manager::GetSelectMotion() const
{
	return &m_MotionSet.m_apMotion[m_wSelectMotion];
}

//============================================================================
// �I�𒆂̃L�[�̃|�C���^���擾
//============================================================================
CMotion_Set::Key* const CMotion_Manager::GetSelectKey() const
{
	return &GetSelectMotion()->apKey[m_wSelectKey];
}