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
		m_MotionSet->Animation();
	}

	// ���Z�b�g
	Reset();

	// �f�o�b�O�\��
	PrintDebug();
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
	m_Json{},
	m_MotionSet{ nullptr },
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
	// �W�F�C�\���f�[�^���擾
	m_Json = utility::OpenJsonFile("Data\\JSON\\motion_export.json");

	// ���[�V�����Z�b�g�𐶐�
	m_MotionSet = CMotion_Set::Create(m_Json);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CMotion_Manager::Uninit()
{
	if (m_MotionSet != nullptr)
	{
		// ���[�V�����Z�b�g�̉������
		m_MotionSet->Release();

		// �|�C���^��������
		m_MotionSet = nullptr;
	}
}

//============================================================================
// �f�o�b�O�\��
//============================================================================
void CMotion_Manager::PrintDebug()
{
	CRenderer::SetDebugString("��������������������������������������������������������������");
	CRenderer::SetDebugString("���p�[�c���F" + to_string(m_MotionSet->m_wMaxParts));
	CRenderer::SetDebugString("�����[�V�������F" + to_string(m_MotionSet->m_wMaxMotion));
	CRenderer::SetDebugString("��������������������������������������������������������������");
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet->m_wMaxMotion; ++wCntMotion)
	{
		const CMotion_Set::Motion* const pMotion = &m_MotionSet->m_apMotion[wCntMotion];
		CRenderer::SetDebugString("<�Đ��ҋ@���[�V�������>");
		CRenderer::SetDebugString("���݂̃t���[�����@�F" + to_string(m_MotionSet->m_wNowFrame));
		CRenderer::SetDebugString("���݂̃L�[���@�@�@�F" + to_string(m_MotionSet->m_wNowKey));
		CRenderer::SetDebugString("���[�v�t���O�@�@�@�F" + to_string(pMotion->bLoop));
		CRenderer::SetDebugString("���L�[���@�@�@�@�@�F" + to_string(pMotion->wMaxKey));
		CRenderer::SetDebugString("�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[");
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			const CMotion_Set::Key* const pKey = &pMotion->vpKey[wCntMotionKey];
			CRenderer::SetDebugString("�L�[[" + to_string(wCntMotionKey) + "]�̑��t���[�����F" + to_string(pKey->wMaxFrame));
		}
	}
	CRenderer::SetDebugString("��������������������������������������������������������������");
	CRenderer::SetDebugString("<�ҏW���>");
	CRenderer::SetDebugString("���݂̑I���p�[�c�@�@�F" + to_string(m_wSelectParts));
	CRenderer::SetDebugString("���W�ҏW�̋��x�@�@�@�F" + to_string(m_fPosEditCoef));
	CRenderer::SetDebugString("���݂̑I�����[�V�����F" + to_string(m_wSelectMotion));
	CRenderer::SetDebugString("���݂̑I���L�[�@�@�@�F" + to_string(m_wSelectKey));
	for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_vpModelParts.size(); ++wCntModelParts)
	{
		const CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[wCntModelParts];
		CRenderer::SetDebugString("Scale�F" + utility::ToPrecision(pDest->ScaleTarget.x) + "�F" + utility::ToPrecision(pDest->ScaleTarget.y) + "�F" + utility::ToPrecision(pDest->ScaleTarget.z) + "�F"
			"Rot�F" + utility::ToPrecision(pDest->RotTarget.x) + "�F" + utility::ToPrecision(pDest->RotTarget.y) + "�F" + utility::ToPrecision(pDest->RotTarget.z) + "�F"
			"Pos�F" + utility::ToPrecision(pDest->PosTarget.x) + "�F" + utility::ToPrecision(pDest->PosTarget.y) + "�F" + utility::ToPrecision(pDest->PosTarget.z));
	}
	CRenderer::SetDebugString("��������������������������������������������������������������");
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
		m_wSelectParts > 0 ? m_wSelectParts-- : m_wSelectParts = static_cast<WORD>(m_MotionSet->m_vpModelParts.size()) - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
	{
		m_wSelectParts < m_MotionSet->m_vpModelParts.size() - 1 ? m_wSelectParts++ : m_wSelectParts = 0;
	}

	// �I���p�[�c�𓧉�
	for (WORD wCntParts = 0; wCntParts < m_MotionSet->m_vpModelParts.size(); ++wCntParts)
	{
		if (m_wSelectParts == wCntParts)
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetCol({ 1.0f, 0.5f, 0.25f, 0.25f });
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(true);
		}
		else
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(false);
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
		m_wSelectMotion > 0 ? m_wSelectMotion-- : m_wSelectMotion = m_MotionSet->m_wMaxMotion - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_4))
	{
		m_wSelectMotion < m_MotionSet->m_wMaxMotion - 1 ? m_wSelectMotion++ : m_wSelectMotion = 0;
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

	// ���L�[���̐؂�ւ�
	if (CManager::GetKeyboard()->GetTrigger(DIK_F2) && GetSelectMotion()->wMaxKey > 1)
	{
		// ���L�[�����f�N�������g
		GetSelectMotion()->wMaxKey--;
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;

		// �L�[���̃|�C���^���쐬
		CMotion_Set::Key* const pKey = &GetSelectMotion()->vpKey[GetSelectMotion()->wMaxKey];

		// �ڕW�l���̃|�C���^�z���j��
		if (pKey->apDest != nullptr)
		{
			delete[] pKey->apDest;
			pKey->apDest = nullptr;
		}

		// �����̃L�[�����폜
		GetSelectMotion()->vpKey.pop_back();

		// �I���L�[�ԍ��𒲐�
		if (m_wSelectKey >= GetSelectMotion()->wMaxKey)
		{
			m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
		}
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F3))
	{
		// �L�[���I�u�W�F�N�g��V�K�쐬
		CMotion_Set::Key Key;

		// �L�[�̑��t���[������ݒ�
		Key.wMaxFrame = 1;
		m_Json["MaxFrame"][GetSelectMotion()->wMaxKey] = Key.wMaxFrame;

		// �p�[�c�����̖ڕW�l���𐶐�
		Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet->m_wMaxParts];

		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// �ڕW�l���̃|�C���^���쐬
			CMotion_Set::KeyDest* const pDest = &Key.apDest[wCntModelParts];

			// �e��p�����[�^��ݒ�
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };	// �ڕW�k��
			m_Json["ScaleTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
			pDest->RotTarget = { VEC3_INIT };	// �ڕW����
			m_Json["RotTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
			pDest->PosTarget = { VEC3_INIT };	// �ڕW���W
			m_Json["PosTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
		}

		GetSelectMotion()->vpKey.push_back(Key);

		// ���L�[�����C���N�������g
		GetSelectMotion()->wMaxKey++;
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;
	}
}

//============================================================================
// �L�[���̍Ċm��
//============================================================================
void CMotion_Manager::ResizeKey()
{
	
}

//============================================================================
// �t���[�����̕ҏW
//============================================================================
void CMotion_Manager::EditFrame()
{
	// ���t���[������
	if (CManager::GetKeyboard()->GetTrigger(DIK_7) && m_MotionSet->GetNowKey()->wMaxFrame > 1)
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
		// �I��ԍ�����������
		m_wSelectParts = 0;
		m_wSelectMotion = 0;
		m_wSelectKey = 0;

		// ��U���
		m_MotionSet->Release();

		// ���Đ���
		m_MotionSet = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\motion_export.json"));
	}
}

//============================================================================
// �I�𒆂̃p�[�c�̃|�C���^���擾
//============================================================================
CObject_Parts* const CMotion_Manager::GetSelectParts() const
{
	return m_MotionSet->m_vpModelParts[m_wSelectParts];
}

//============================================================================
// �I�𒆂̃��[�V�����̃|�C���^���擾
//============================================================================
CMotion_Set::Motion* const CMotion_Manager::GetSelectMotion() const
{
	return &m_MotionSet->m_apMotion[m_wSelectMotion];
}

//============================================================================
// �I�𒆂̃L�[�̃|�C���^���擾
//============================================================================
CMotion_Set::Key* const CMotion_Manager::GetSelectKey() const
{
	return &GetSelectMotion()->vpKey[m_wSelectKey];
}