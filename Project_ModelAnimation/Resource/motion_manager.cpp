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
	// �E�B���h�E��\��
	ImVec2 Rect = { 600, 800 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ SCREEN_WIDTH - (Rect.x + 100), 100, });
	ImGui::Begin("Motion_Manager");

	// �ҏW
	Edit();

	if (m_bPlay)
	{
		// ����
		m_MotionSet->Animation();
	}

	// ���Z�b�g
	Reset();

	// �E�B���h�E�\���I��
	ImGui::End();

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
			CRenderer::SetDebugString("�L�[[" + to_string(wCntMotionKey) + "]�̑��t���[�����F" + to_string(pKey->nMaxFrame));
		}
	}
	CRenderer::SetDebugString("��������������������������������������������������������������");
	CRenderer::SetDebugString("<�ҏW���>");
	CRenderer::SetDebugString("���݂̑I���p�[�c�@�@�F" + to_string(m_wSelectParts));
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
	ImGui::Checkbox("Playing Motion", &m_bPlay);

	// �p�[�c���̕ҏW
	EditParts();

	// ���[�V�������̕ҏW
	EditMotion();

	// �L�[���̕ҏW
	EditKey();

	// �t���[�����̕ҏW
	EditFrame();

	// �G�N�X�|�[�g
	ImGui::Separator();
	if (ImGui::Button("Export Edit Data"))
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
	ImGui::Separator();
	ImGui::BulletText("Select Parts");
	if (ImGui::Button("First"))
	{
		m_wSelectParts = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Prev"))
	{
		m_wSelectParts > 0 ? m_wSelectParts-- : m_wSelectParts = static_cast<WORD>(m_MotionSet->m_vpModelParts.size()) - 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("Next"))
	{
		m_wSelectParts < m_MotionSet->m_vpModelParts.size() - 1 ? m_wSelectParts++ : m_wSelectParts = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Last"))
	{
		m_wSelectParts = m_MotionSet->m_vpModelParts.size() - 1;
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

	// �ڕW�k�ڂ�ϓ�
	ImGui::Separator();
	ImGui::BulletText("ScaleDest");
	{
		// �k�ځFX
		if (ImGui::Button("Reset:ScaleX"))
		{
			pDest->ScaleTarget.x = 1.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Scale:X", &pDest->ScaleTarget.x, 1.0f, 1.0f);
		// �k�ځFY
		if (ImGui::Button("Reset:ScaleY"))
		{
			pDest->ScaleTarget.y = 1.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Scale:Y", &pDest->ScaleTarget.y, 1.0f, 1.0f);
		// �k�ځFZ
		if (ImGui::Button("Reset:ScaleZ"))
		{
			pDest->ScaleTarget.z = 1.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Scale:Z", &pDest->ScaleTarget.z, 1.0f, 1.0f);
	}

	// �ڕW������ϓ�
	ImGui::Separator();
	ImGui::BulletText("RotDest");
	{
		// �����FX
		if (ImGui::Button("Reset:RotX"))
		{
			pDest->RotTarget.x = 0.0f;
		}
		ImGui::SameLine();
		ImGui::SliderFloat("Rot:X", &pDest->RotTarget.x, -D3DX_PI, D3DX_PI);
		// �����FX
		if (ImGui::Button("Reset:RotY"))
		{
			pDest->RotTarget.y = 0.0f;
		}
		ImGui::SameLine();
		ImGui::SliderFloat("Rot:Y", &pDest->RotTarget.y, -D3DX_PI, D3DX_PI);
		// �����FZ
		if (ImGui::Button("Reset:RotZ"))
		{
			pDest->RotTarget.z = 0.0f;
		}
		ImGui::SameLine();
		ImGui::SliderFloat("Rot:Z", &pDest->RotTarget.z, -D3DX_PI, D3DX_PI);
	}

	// �ڕW���W
	ImGui::Separator();
	ImGui::BulletText("ScaleDest");
	{
		// ���W�FX
		if (ImGui::Button("Reset:PosX"))
		{
			pDest->PosTarget.x = 0.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Pos:X", &pDest->PosTarget.x, 1.0f, 1.0f);
		// ���W�FY
		if (ImGui::Button("Reset:PosY"))
		{
			pDest->PosTarget.y = 0.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Pos:Y", &pDest->PosTarget.y, 1.0f, 1.0f);
		// ���W�FZ
		if (ImGui::Button("Reset:PosZ"))
		{
			pDest->PosTarget.z = 0.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Pos:Z", &pDest->PosTarget.z, 1.0f, 1.0f);
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
		// ���L�[�����f�N�������g���A�W�F�C�\���f�[�^�ɕۑ�
		GetSelectMotion()->wMaxKey--;
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;

		// ��������L�[�ԍ��̃t���[���E�ڕW�l�����W�F�C�\���f�[�^�������
		m_Json["MaxFrame"].erase(GetSelectMotion()->wMaxKey);
		m_Json["ScaleTarget"].erase(GetSelectMotion()->wMaxKey);
		m_Json["RotTarget"].erase(GetSelectMotion()->wMaxKey);
		m_Json["PosTarget"].erase(GetSelectMotion()->wMaxKey);

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

		// �V���ȃL�[�̑��t���[������ݒ肵�A�W�F�C�\���f�[�^�ɕۑ�
		Key.nMaxFrame = 1;
		m_Json["MaxFrame"][GetSelectMotion()->wMaxKey] = Key.nMaxFrame;

		// �p�[�c�����̖ڕW�l����ǉ�����
		Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet->m_wMaxParts];

		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// �ڕW�l���̃|�C���^���쐬
			CMotion_Set::KeyDest* const pDest = &Key.apDest[wCntModelParts];

			// �V���ȃL�[�̃p�[�c�̖ڕW�l��ݒ肵�A�W�F�C�\���f�[�^�ɕۑ�
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };	// �ڕW�k��
			m_Json["ScaleTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
			pDest->RotTarget = { VEC3_INIT };	// �ڕW����
			m_Json["RotTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
			pDest->PosTarget = { VEC3_INIT };	// �ڕW���W
			m_Json["PosTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
		}

		GetSelectMotion()->vpKey.push_back(Key);

		// ���L�[�����C���N�������g�A�W�F�C�\���f�[�^�ɕۑ�
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
	const int nOldMaxFrame = GetSelectKey()->nMaxFrame;

	// ���t���[������
	ImGui::Separator();
	ImGui::BulletText("Frame");
	ImGui::InputInt("MaxFrame", &GetSelectKey()->nMaxFrame, 1, 1);

	// �t���[�����̉�����ݒ�
	if (GetSelectKey()->nMaxFrame < 1)
	{
		GetSelectKey()->nMaxFrame = 1;
	}

	// ����������΃W�F�C�\���f�[�^��ύX
	if (nOldMaxFrame != GetSelectKey()->nMaxFrame)
	{
		m_Json["MaxFrame"][m_wSelectKey] = GetSelectKey()->nMaxFrame;
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
	// �G�N�X�|�[�g�{�^���̉�
	ImGui::SameLine();

	if (ImGui::Button("Reset Edit Data"))
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