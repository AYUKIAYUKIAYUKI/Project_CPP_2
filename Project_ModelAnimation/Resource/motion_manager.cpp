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
#include "object_X.h"
#include "manager.h"
#include "renderer.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
#define SORT_ON_EXPORT 1	// �G�N�X�|�[�g���ɐ���

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
	// ����C���|�[�g
	if (!m_bInitImport)
	{
		if (Import())
			m_bInitImport = true;

		return;
	}

	// �C���|�[�g
	if (Import())
		return;	// �C���|�[�g���X�V���Ȃ�

	// �E�B���h�E��\��
	ImVec2 Rect = { 600, -1 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ SCREEN_WIDTH - (Rect.x + 100), 10 });
	ImGui::Begin("Edit MotionList");

	// �ҏW
	Edit();

	if (m_bPlay)
	{
		// ����
		m_MotionSet->Animation();
	}

	if (m_bShowKeyEnd)
	{
		// �L�[�̍ŏI�I�ȓ�����������
		ShowKeyEnd();
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
	m_bInitImport{ false },
	m_NowSetFileName{},
	m_Json{},
	m_MotionSet{ nullptr },
	m_wSelectParts{ 0 },
	m_CopyKey{ 0, nullptr },
	m_wSelectKey{ 0 },
	m_bPartsAppeal{ false },
	m_bPlay{ false },
	m_bShowKeyEnd{ false }
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
#if 0
	// �W�F�C�\���f�[�^���擾
	m_Json = utility::OpenJsonFile("Data\\JSON\\motion_export.json");

	// ���[�V�����Z�b�g�𐶐�
	m_MotionSet = CMotion_Set::Create(m_Json);
#endif

	// �I���p�[�c���A�s�[��
	m_bPartsAppeal = true;

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
#if 0
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
#else
	// �E�B���h�E��\��
	ImVec2 Rect = { 500, 400 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ 50, 610 });
	ImGui::Begin("MotionManager Infomation");
	ImGui::Text("MaxParts:%d", m_MotionSet->m_wMaxParts);
	ImGui::SameLine();
	ImGui::Text("MaxMotion:%d", m_MotionSet->m_wMaxMotion);
	ImGui::Separator();
	ImGui::Text("PlayingMotion:%d :(%d)", m_MotionSet->m_wNowMotion, m_MotionSet->m_wMaxMotion);
	ImGui::Text("PlayingKey:%d :(%d)", m_MotionSet->m_wNowKey, m_MotionSet->GetNowMotionPtr()->wMaxKey);
	ImGui::Text("PlayingFrame:%d :(%d)", m_MotionSet->m_wNowFrame, m_MotionSet->GetNowKey()->nMaxFrame);
	for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
	{
		const CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[wCntModelParts];
		const CObject_Parts* const pParts = m_MotionSet->m_vpModelParts[wCntModelParts];
		ImGui::Separator();
		ImGui::BulletText("Parts[%d] KeyDest Infomation", wCntModelParts);
		ImGui::Text("Scale[ X:(%.2f / %.2f) X:(%.2f / %.2f) X:(%.2f / %.2f) ]", pParts->GetScale().x, pDest->ScaleTarget.x, pParts->GetScale().y, pDest->ScaleTarget.y, pParts->GetScale().z, pDest->ScaleTarget.z);
		ImGui::Text("Rot[ X:(%.2f / %.2f) X:(%.2f / %.2f) X:(%.2f / %.2f) ]", pParts->GetRot().x, pDest->RotTarget.x, pParts->GetRot().y, pDest->RotTarget.y, pParts->GetRot().z, pDest->RotTarget.z);
		ImGui::Text("Pos[ X:(%.2f / %.2f) X:(%.2f / %.2f) X:(%.2f / %.2f) ]", pParts->GetPos().x, pDest->PosTarget.x, pParts->GetPos().y, pDest->PosTarget.y, pParts->GetPos().z, pDest->PosTarget.z);
	}
	ImGui::End();
#endif
}

//============================================================================
// �C���|�[�g
//============================================================================
bool CMotion_Manager::Import()
{
	// �E�B���h�E��\��
	ImVec2 Rect = { 600, 80 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ 50, 50 });
	if (ImGui::Begin("Import Motion Set"))
	{		
		static char FilePath[256] = "";

		if (ImGui::Button("Done"))
		{
			// ���󃊃Z�b�g
			Reset();

			// ���݂̃��[�V�����Z�b�g��j��
			Uninit();

			// �W�F�C�\���f�[�^���擾
			std::string Copy = FilePath;
			m_Json = utility::OpenJsonFile("Data\\JSON\\EXPORT\\" + Copy + ".json");

			// �t�@�C�������R�s�[
			m_NowSetFileName = Copy;

			// ���[�V�����Z�b�g�𐶐�
			m_MotionSet = CMotion_Set::Create(m_Json);

			ImGui::End();
			return true;
		}
		
		ImGui::SameLine();
		ImGui::InputText("##", FilePath, 256);

		ImGui::End();
	}

	return false;
}

//============================================================================
// �ҏW
//============================================================================
void CMotion_Manager::Edit()
{
	// �Đ��؂�ւ�
	if( ImGui::Checkbox("Playing motion", &m_bPlay))
	{
		// �Đ����������Ƃ��Ƀ��[�V�����X�g�b�v�̃t���O�������Ă�����
		if (m_bPlay && m_MotionSet->m_bStop)
			m_MotionSet->m_bStop = false;
	}
	ImGui::SameLine();
	ImGui::Checkbox("Show this key end", &m_bShowKeyEnd);

	// �L�[�̍Ō�̓����������鎞�̓A�j���[�V�������Ȃ�
	if (m_bShowKeyEnd)
	{
		m_bPlay = false;
	}

	// �p�[�c���̕ҏW
	EditParts();

	// ���[�V�������̕ҏW
	EditMotion();

	// �L�[���̕ҏW
	EditKey();

	// �t���[�����̕ҏW
	EditFrame();

	// �ڕW�l���̕ҏW
	EditDest();

	// �G�N�X�|�[�g
	ImGui::Separator();
	static char FilePath[256] = "";
	std::string Copy = FilePath;
	ImGui::InputText("##", FilePath, 256);
	ImGui::SameLine();
	if (ImGui::Button("Export"))
	{
		Export(Copy);
	}
}

//============================================================================
// �p�[�c���̕ҏW
//============================================================================
void CMotion_Manager::EditParts()
{
	// �I���p�[�c�̃A�s�[���؂�ւ�
	ImGui::Separator();
	ImGui::BulletText("Select Parts");
	ImGui::Checkbox("Appeal select parts", &m_bPartsAppeal);

	// �I���p�[�c�؂�ւ�
	if (ImGui::Button("FirstParts"))
	{
		m_wSelectParts = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("PrevParts"))
	{
		m_wSelectParts > 0 ? m_wSelectParts-- : m_wSelectParts = static_cast<WORD>(m_MotionSet->m_vpModelParts.size()) - 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("NextParts"))
	{
		m_wSelectParts < m_MotionSet->m_vpModelParts.size() - 1 ? m_wSelectParts++ : m_wSelectParts = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("LastParts"))
	{
		m_wSelectParts = m_MotionSet->m_vpModelParts.size() - 1;
	}
	ImGui::SameLine();
	ImGui::Text("Select:%d", m_wSelectParts);

	// ����
	static float fMin = 0.45f, fMax = 0.6f, fCoef = 0.03f;
	static XCol Col = { 1.0f, 1.0f, 1.0f, fMin };
	static bool bAdder = false;
	bAdder ?
		Col.a += (fMax - Col.a) * fCoef :
		Col.a += (fMin - Col.a) * fCoef;
	if (Col.a > fMax - fCoef)
		bAdder = false;
	else if (Col.a < fMin + fCoef)
		bAdder = true;

	// �I���p�[�c�𓧉�
	for (WORD wCntParts = 0; wCntParts < m_MotionSet->m_vpModelParts.size(); ++wCntParts)
	{
		if (m_wSelectParts == wCntParts && m_bPartsAppeal)
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetCol(Col);
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(true);
		}
		else
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(false);
		}
	}

	// �I�t�Z�b�g���̕ҏW
	EditOffset();
}

//============================================================================
// �I�t�Z�b�g���̕ҏW
//============================================================================
void CMotion_Manager::EditOffset()
{
	// �I�𒆂̃p�[�c�̃|�C���^
	CObject_Parts* const pParts = m_MotionSet->m_vpModelParts[m_wSelectParts];

	// �I�t�Z�b�g�����R�s�[
	Vec3 ScaleOffset = m_MotionSet->m_vpModelParts[0]->GetScaleOffset(), RotOffset = pParts->GetRotOffset(), PosOffset = pParts->GetPosOffset();

	// �E�B���h�E�𐶐�
	ImVec2 Rect = { 600, 450 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ 50, 145 });
	if (ImGui::Begin("Parts Offset"))
	{
		// �k�ڃI�t�Z�b�g��ϓ�
		ImGui::Separator();
		ImGui::BulletText("ScaleOffset");
		{
			// �k�ځFX
			if (ImGui::Button("Reset:ScaleX"))
			{
				ScaleOffset.x = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Scale:X", &ScaleOffset.x, 0.01f, 0.0f);
			// �k�ځFY
			if (ImGui::Button("Reset:ScaleY"))
			{
				ScaleOffset.y = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Scale:Y", &ScaleOffset.y, 0.01f, 0.0f);
			// �k�ځFZ
			if (ImGui::Button("Reset:ScaleZ"))
			{
				ScaleOffset.z = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Scale:Z", &ScaleOffset.z, 0.01f, 0.0f);
			m_MotionSet->m_vpModelParts[0]->SetScaleOffset(ScaleOffset);
		}

		// �����I�t�Z�b�g��ϓ�
		ImGui::Separator();
		ImGui::BulletText("RotOffset");
		{
			// �����FX
			if (ImGui::Button("Reset:RotX"))
			{
				RotOffset.x = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Rot:X", &RotOffset.x, D3DX_PI * 0.01f, -D3DX_PI, D3DX_PI);
			// �����FX
			if (ImGui::Button("Reset:RotY"))
			{
				RotOffset.y = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Rot:Y", &RotOffset.y, D3DX_PI * 0.01f, -D3DX_PI, D3DX_PI);
			// �����FZ
			if (ImGui::Button("Reset:RotZ"))
			{
				RotOffset.z = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Rot:Z", &RotOffset.z, D3DX_PI * 0.01f, -D3DX_PI, D3DX_PI);
			pParts->SetRotOffset(RotOffset);
		}

		// ���W�I�t�Z�b�g��ϓ�
		ImGui::Separator();
		ImGui::BulletText("PosOffset");
		{
			// ���W�FX
			if (ImGui::Button("Reset:PosX"))
			{
				PosOffset.x = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Pos:X", &PosOffset.x, 0.01f);
			// ���W�FY
			if (ImGui::Button("Reset:PosY"))
			{
				PosOffset.y = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Pos:Y", &PosOffset.y, 0.01f);
			// ���W�FZ
			if (ImGui::Button("Reset:PosZ"))
			{
				PosOffset.z = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Pos:Z", &PosOffset.z, 0.01f);
			pParts->SetPosOffset(PosOffset);
		}

		ImGui::End();
	}
}

//============================================================================
// ���[�V�������̕ҏW
//============================================================================
void CMotion_Manager::EditMotion()
{
	// �I�����[�V�����؂�ւ�
	bool bChange = false;
	ImGui::Separator();
	ImGui::BulletText("Select Motion");
	if (ImGui::Button("FirstMotion"))
	{
		m_MotionSet->m_wNowMotion = 0;
		bChange = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("PrevMotion"))
	{
		m_MotionSet->m_wNowMotion > 0 ? m_MotionSet->m_wNowMotion-- : m_MotionSet->m_wNowMotion = m_MotionSet->m_wMaxMotion - 1;
		bChange = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("NextMotion"))
	{
		m_MotionSet->m_wNowMotion < m_MotionSet->m_wMaxMotion - 1 ? m_MotionSet->m_wNowMotion++ : m_MotionSet->m_wNowMotion = 0;
		bChange = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("LastMotion"))
	{
		m_MotionSet->m_wNowMotion = m_MotionSet->m_wMaxMotion - 1;
		bChange = true;
	}
	ImGui::SameLine();
	ImGui::Text("Select:%d", m_MotionSet->m_wNowMotion);

	// ���[�V������؂�ւ��Ă�����
	if (bChange)
	{
		m_MotionSet->m_bStop = false;
		m_MotionSet->m_wNowKey = 0;
		m_MotionSet->m_wNowFrame = 0;
	}

	// ���݂̍Đ��L�[�����L�[���𒴂��Ȃ��悤����
	if (m_MotionSet->m_wNowKey >= GetSelectMotion()->wMaxKey)
	{
		m_MotionSet->m_wNowKey = GetSelectMotion()->wMaxKey - 1;
	}

	// �I���L�[�ԍ��𒲐�
	if (m_wSelectKey >= GetSelectMotion()->wMaxKey)
	{
		m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
	}
	
	// ���[�v�t���O�؂�ւ�
	ImGui::BulletText("Edit Motion");
	ImGui::Checkbox("Motion Loop", &m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].bLoop);

	// �I�𒆂̃��[�V�����̃��[�v�t���O�𗧂Ă��Ƃ��ɁA���݂̍Đ����[�V�������X�g�b�v���Ă�����
	if (m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].bLoop && m_MotionSet->m_bStop)
		m_MotionSet->m_bStop = false;

	// �����[�V�������̐؂�ւ�
	if (ImGui::Button("--Motion") && m_MotionSet->m_wMaxMotion > 1)
	{
		// �����[�V���������f�N�������g��
		m_MotionSet->m_wMaxMotion--;
#if !SORT_ON_EXPORT
		m_Json["MaxMotion"] = m_MotionSet->m_wMaxMotion;
#endif	// _SORT_ON_EXPORT_

		// ���݂̍Đ����[�V�����������[�V�������𒴂��Ȃ��悤����
		if (m_MotionSet->m_wNowMotion >= m_MotionSet->m_wMaxMotion)
		{
			m_MotionSet->m_wNowMotion = m_MotionSet->m_wMaxMotion - 1;
		}

		// ���[�V�������̃|�C���^���쐬
		CMotion_Set::Motion* pMotion = &m_MotionSet->m_vpMotion[m_MotionSet->m_wMaxMotion];

		// �L�[�����̖ڕW�l���̃|�C���^�z���j��
		for (WORD wCntKey = 0; wCntKey < pMotion->wMaxKey; ++wCntKey)
		{
			if (pMotion->vpKey[wCntKey].apDest != nullptr)
			{
				delete[] pMotion->vpKey[wCntKey].apDest;
				pMotion->vpKey[wCntKey].apDest = nullptr;
			}
		}

		// �L�[�����폜
		pMotion->vpKey.clear();

		// �����̃��[�V���������폜
		m_MotionSet->m_vpMotion.pop_back();
	}
	ImGui::SameLine();
	if (ImGui::Button("++Motion"))
	{
		// ���[�V�������I�u�W�F�N�g��V�K�쐬
		CMotion_Set::Motion Motion;

		// �V���ȃ��[�V�����̃��[�v�t���O��ݒ�
		Motion.bLoop = false;

		// �V���ȃ��[�V�����̑��L�[����ݒ�
		Motion.wMaxKey = 1;
#if !SORT_ON_EXPORT
		m_Json["MaxKey"][m_MotionSet->m_wMaxMotion] = 1;
#endif	// _SORT_ON_EXPORT_

		// �L�[���I�u�W�F�N�g��V�K�쐬
		CMotion_Set::Key Key;

		// �V���ȃL�[�̑��t���[������ݒ�
		Key.nMaxFrame = 1;
#if !SORT_ON_EXPORT
		m_Json["MaxFrame"][Motion.wMaxKey] = 1;
#endif	// _SORT_ON_EXPORT_

		// �p�[�c�����̖ڕW�l����ǉ�����
		Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet->m_wMaxParts];

		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// �ڕW�l���̃|�C���^���쐬
			CMotion_Set::KeyDest* const pDest = &Key.apDest[wCntModelParts];

			// �V���ȃ��[�V�����̃p�[�c�̖ڕW�l��ݒ�
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };	// �ڕW�k��
			pDest->RotTarget = { VEC3_INIT };			// �ڕW����
			pDest->PosTarget = { VEC3_INIT };			// �ڕW���W
		}

		// �L�[����ǉ�
		Motion.vpKey.push_back(Key);

		// ���[�V��������ǉ�
		m_MotionSet->m_vpMotion.push_back(Motion);

		// �����[�V���������C���N�������g
		m_MotionSet->m_wMaxMotion++;
#if !SORT_ON_EXPORT
		m_Json["MaxMotion"] = m_MotionSet->m_wMaxMotion;
#endif	// _SORT_ON_EXPORT_
	}
	ImGui::SameLine();
	ImGui::Text("MaxMotion:%d", m_MotionSet->m_wMaxMotion);
}

//============================================================================
// �L�[���̕ҏW
//============================================================================
void CMotion_Manager::EditKey()
{
	// �I���L�[�؂�ւ�
	ImGui::Separator();
	ImGui::BulletText("Select Key");
	if (ImGui::Button("FirstKey"))
	{
		m_wSelectKey = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("PrevKey") || CManager::GetMouse()->GetTrigger(3))
	{
		m_wSelectKey > 0 ? m_wSelectKey-- : m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("NextKey") || CManager::GetMouse()->GetTrigger(4))
	{
		m_wSelectKey < GetSelectMotion()->wMaxKey - 1 ? m_wSelectKey++ : m_wSelectKey = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("LastKey"))
	{
		m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
	}
	ImGui::SameLine();
	ImGui::Text("Select:%d", m_wSelectKey);

#if 1
	ImGui::BulletText("Copy Key");
	{ // �L�[���ƃR�s�[�y�[�X�g

		// �E�B���h�E�ƃE�B�W�F�b�g�̕����R�s�[
		float fWidth = ImGui::GetContentRegionAvail().x, fSeparate = ImGui::GetStyle().ItemSpacing.x;

		// �{�^���̃T�C�Y
		const float fCopyPasteButtonWidth = (fWidth + -fSeparate * 1.0f) / 2.0f;

		// �R�s�[�p
		static std::vector<CMotion_Set::KeyDest> CopyAllDest;

		if (ImGui::Button("Copy##All", { fCopyPasteButtonWidth, 30.0f }))
		{
			// ��̃R�s�[�����Z�b�g
			CopyAllDest.clear();

			// ���݂̃L�[�̑S�p�[�c�̑S�ڕW���R�s�[
			for (WORD wCntParts = 0; wCntParts < m_MotionSet->m_wMaxParts; ++wCntParts)
				CopyAllDest.push_back(GetSelectKey()->apDest[wCntParts]);
		}
		ImGui::SameLine();
		if (ImGui::Button("Paste##All", { fCopyPasteButtonWidth, 30.0f }))
		{
			// ���݂̃L�[�̑S�p�[�c�̑S�ڕW���y�[�X�g
			for (WORD wCntParts = 0; wCntParts < m_MotionSet->m_wMaxParts; ++wCntParts)
				GetSelectKey()->apDest[wCntParts] = CopyAllDest[wCntParts];
		}
	}
#endif

	// ���L�[���̐؂�ւ�
	ImGui::BulletText("Edit Key");
	if (ImGui::Button("--Key") && GetSelectMotion()->wMaxKey > 1)
	{
		// ���L�[�����f�N�������g��
		GetSelectMotion()->wMaxKey--;
#if !SORT_ON_EXPORT
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;
#endif	// _SORT_ON_EXPORT_

		// ���݂̍Đ��L�[�����L�[���𒴂��Ȃ��悤����
		if (m_MotionSet->m_wNowKey >= GetSelectMotion()->wMaxKey)
		{
			m_MotionSet->m_wNowKey = GetSelectMotion()->wMaxKey - 1;
		}

		// �I���L�[�ԍ��𒲐�
		if (m_wSelectKey >= GetSelectMotion()->wMaxKey)
		{
			m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
		}

#if !SORT_ON_EXPORT
		// ��������L�[�ԍ��̃t���[���E�ڕW�l�����W�F�C�\���f�[�^�������
		m_Json["MaxFrame"].erase(GetSelectMotion()->wMaxKey);
		m_Json["ScaleTarget"].erase(GetSelectMotion()->wMaxKey);
		m_Json["RotTarget"].erase(GetSelectMotion()->wMaxKey);
		m_Json["PosTarget"].erase(GetSelectMotion()->wMaxKey);
#endif	// _SORT_ON_EXPORT_

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
	}
	ImGui::SameLine();
	if (ImGui::Button("++Key"))
	{
		// �L�[���I�u�W�F�N�g��V�K�쐬
		CMotion_Set::Key Key;

		// �V���ȃL�[�̑��t���[������ݒ�
		Key.nMaxFrame = 1;
#if !SORT_ON_EXPORT
		m_Json["MaxFrame"][GetSelectMotion()->wMaxKey] = Key.nMaxFrame;
#endif	// _SORT_ON_EXPORT_

		// �p�[�c�����̖ڕW�l����ǉ�����
		Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet->m_wMaxParts];

		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// �ڕW�l���̃|�C���^���쐬
			CMotion_Set::KeyDest* const pDest = &Key.apDest[wCntModelParts];

			// �V���ȃL�[�̃p�[�c�̖ڕW�l��ݒ肵�A�W�F�C�\���f�[�^�ɕۑ�
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };	// �ڕW�k��
			pDest->RotTarget = { VEC3_INIT };			// �ڕW����
			pDest->PosTarget = { VEC3_INIT };			// �ڕW���W
#if !SORT_ON_EXPORT
			m_Json["ScaleTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
			m_Json["RotTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
			m_Json["PosTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
#endif	// _if !SORT_ON_EXPORT_
		}

		GetSelectMotion()->vpKey.push_back(Key);

		// ���L�[�����C���N�������g
		GetSelectMotion()->wMaxKey++;
#if !SORT_ON_EXPORT
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;
#endif	// _SORT_ON_EXPORT_
	}
	ImGui::SameLine();
	ImGui::Text("MaxKey:%d", m_MotionSet->GetNowMotionPtr()->wMaxKey);
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

	// ���݂̍Đ��t���[�������t���[�����𒴂��Ȃ��悤����
	if (m_MotionSet->m_wNowFrame >= m_MotionSet->GetNowKey()->nMaxFrame)
	{
		m_MotionSet->m_wNowFrame = m_MotionSet->GetNowKey()->nMaxFrame - 1;
	}

#if !SORT_ON_EXPORT
	// ����������΃W�F�C�\���f�[�^��ύX
	if (nOldMaxFrame != GetSelectKey()->nMaxFrame)
	{
		m_Json["MaxFrame"][m_wSelectKey] = GetSelectKey()->nMaxFrame;
	}
#endif	// _SORT_ON_EXPORT_
}

//============================================================================
// �ڕW�l���̕ҏW
//============================================================================
void CMotion_Manager::EditDest()
{
	// �I�𒆂̃p�[�c�̖ڕW�l���̃|�C���^���쐬
	CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[m_wSelectParts];

	// �E�B���h�E�ƃE�B�W�F�b�g�̕����R�s�[
	float fWidth = ImGui::GetContentRegionAvail().x, fSeparate = ImGui::GetStyle().ItemSpacing.x;
	
	// �{�^���̃T�C�Y
	const float 
		fResetButtonWidth = (fWidth + -fSeparate * 3.0f) / 4.0f,
		fCopyPasteButtonWidth = (fWidth + -fSeparate * 1.0f) / 2.0f;

	// �ڕW�k�ڂ�ϓ�
	ImGui::SeparatorText("ScaleDest");
	{
		// �k�ڃ��Z�b�g
		if (ImGui::Button("Reset:X##Scale", { fResetButtonWidth, 30.0f }))
			pDest->ScaleTarget.x = 1.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Y##Scale", { fResetButtonWidth, 30.0f }))
			pDest->ScaleTarget.y = 1.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Z##Scale", { fResetButtonWidth, 30.0f }))
			pDest->ScaleTarget.z = 1.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:All##Scale", { fResetButtonWidth, 30.0f }))
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };
		ImGui::Separator();

		// �k�ڕύX
		float* ScalePtr[3] = { &pDest->ScaleTarget.x, &pDest->ScaleTarget.y, &pDest->ScaleTarget.z };
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::DragFloat3("##Scale", *ScalePtr, 0.01f, 0.0f);
		ImGui::Separator();

		// �R�s�[�y�[�X�g
		static Vec3 Copy = { 1.0f, 1.0f, 1.0f };
		if (ImGui::Button("Copy##Scale", { fCopyPasteButtonWidth, 30.0f }))
			Copy = pDest->ScaleTarget;
		ImGui::SameLine();
		if (ImGui::Button("Paste##Scale", { fCopyPasteButtonWidth, 30.0f }))
			pDest->ScaleTarget = Copy;
	}

	// �ڕW������ϓ�
	ImGui::SeparatorText("RotDest");
	{
		// �������Z�b�g
		if (ImGui::Button("Reset:X##Rot", { fResetButtonWidth, 30.0f }))
			pDest->RotTarget.x = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Y##Rot", { fResetButtonWidth, 30.0f }))
			pDest->RotTarget.y = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Z##Rot", { fResetButtonWidth, 30.0f }))
			pDest->RotTarget.z = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:All##Rot", { fResetButtonWidth, 30.0f }))
			pDest->RotTarget = VEC3_INIT;
		ImGui::Separator();

		// �����ύX
		float* RotPtr[3] = { &pDest->RotTarget.x, &pDest->RotTarget.y, &pDest->RotTarget.z };
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::DragFloat3("##Rot", *RotPtr, 0.01f, 0.0f);
		ImGui::Separator();

		// �R�s�[�y�[�X�g
		static Vec3 Copy = { 0.0f, 0.0f, 0.0f };
		if (ImGui::Button("Copy##Rot", { fCopyPasteButtonWidth, 30.0f }))
			Copy = pDest->RotTarget;
		ImGui::SameLine();
		if (ImGui::Button("Paste##Rot", { fCopyPasteButtonWidth, 30.0f }))
			pDest->RotTarget = Copy;
	}

	// �ڕW���W
	ImGui::SeparatorText("PosDest");
	{
		// ���W���Z�b�g
		if (ImGui::Button("Reset:X##Pos", { fResetButtonWidth, 30.0f }))
			pDest->PosTarget.x = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Y##Pos", { fResetButtonWidth, 30.0f }))
			pDest->PosTarget.y = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Z##Pos", { fResetButtonWidth, 30.0f }))
			pDest->PosTarget.z = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:All##Pos", { fResetButtonWidth, 30.0f }))
			pDest->PosTarget = VEC3_INIT;
		ImGui::Separator();

		// ���W�ύX
		float* PosPtr[3] = { &pDest->PosTarget.x, &pDest->PosTarget.y, &pDest->PosTarget.z };
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::DragFloat3("##Pos", *PosPtr, 0.01f, 0.0f);
		ImGui::Separator();

		// �R�s�[�y�[�X�g
		static Vec3 Copy = { 0.0f, 0.0f, 0.0f };
		if (ImGui::Button("Copy##Pos", { fCopyPasteButtonWidth, 30.0f }))
			Copy = pDest->PosTarget;
		ImGui::SameLine();
		if (ImGui::Button("Paste##Pos", { fCopyPasteButtonWidth, 30.0f }))
			pDest->PosTarget = Copy;
	}

#if !SORT_ON_EXPORT
	// �ڕW�l�𔽉f
	m_Json["ScaleTarget"][m_wSelectKey][m_wSelectParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
	m_Json["RotTarget"][m_wSelectKey][m_wSelectParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
	m_Json["PosTarget"][m_wSelectKey][m_wSelectParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
#endif	// _SORT_ON_EXPORT_
}

//============================================================================
// �L�[�̍ŏI�I�ȏ�Ԃ�������
//============================================================================
void CMotion_Manager::ShowKeyEnd()
{
	// �S�Ẵp�[�c���A���̃L�[�̖ڕW�l�ɌŒ肳���
	for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
	{
		// �ڕW�k��
		m_MotionSet->m_vpModelParts[wCntModelParts]->SetScale(m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].vpKey[m_wSelectKey].apDest[wCntModelParts].ScaleTarget);

		// �ڕW����
		m_MotionSet->m_vpModelParts[wCntModelParts]->SetRot(m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].vpKey[m_wSelectKey].apDest[wCntModelParts].RotTarget);

		// �ڕW���W
		m_MotionSet->m_vpModelParts[wCntModelParts]->SetPos(m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].vpKey[m_wSelectKey].apDest[wCntModelParts].PosTarget);
	}
}

//============================================================================
// �G�N�X�|�[�g
//============================================================================
void CMotion_Manager::Export(std::string Copy)
{
	// �t�@�C�������쐬
	std::string FileName = "Data\\JSON\\EXPORT\\" + Copy + ".json";

	// �t�@�C���������o���W�J
	std::ofstream Ofs(FileName, std::ios::out);

	if (Ofs.good())
	{
#if SORT_ON_EXPORT

		// �����o���p�W�F�C�\���I�u�W�F�N�g���쐬
		JSON Export;

		// �����[�V������
		Export["MaxMotion"] = m_MotionSet->m_wMaxMotion;

		// ���p�[�c��
		Export["MaxParts"] = m_MotionSet->m_wMaxParts;

		// ���f���^�C�v
		Export["ModelType"] = m_Json["ModelType"];

		// �e�p�[�c�̃C���f�b�N�X
		Export["ParentIdx"] = m_Json["ParentIdx"];

		// �k�ڃI�t�Z�b�g
		Export["ScaleOffset"] = { m_MotionSet->m_vpModelParts[0]->GetScaleOffset().x, m_MotionSet->m_vpModelParts[0]->GetScaleOffset().y, m_MotionSet->m_vpModelParts[0]->GetScaleOffset().z };
			
		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// �I�𒆂̃p�[�c�̃|�C���^
			CObject_Parts* const pParts = m_MotionSet->m_vpModelParts[wCntModelParts];

			// �����I�t�Z�b�g
			Export["RotOffset"][wCntModelParts] = { pParts->GetRotOffset().x, pParts->GetRotOffset().y, pParts->GetRotOffset().z };
			
			// ���W�I�t�Z�b�g
			Export["PosOffset"][wCntModelParts] = { pParts->GetPosOffset().x, pParts->GetPosOffset().y, pParts->GetPosOffset().z };
		}

		for (WORD wCntMotion = 0; wCntMotion < m_MotionSet->m_wMaxMotion; ++wCntMotion)
		{
			// ���[�v�t���O
			Export["Loop"][wCntMotion] = m_MotionSet->m_vpMotion[wCntMotion].bLoop;

			// ���L�[��
			Export["MaxKey"][wCntMotion] = m_MotionSet->m_vpMotion[wCntMotion].wMaxKey;

			for (WORD wCntKey = 0; wCntKey < m_MotionSet->m_vpMotion[wCntMotion].wMaxKey; ++wCntKey)
			{
				const CMotion_Set::Key* const pKey = &m_MotionSet->m_vpMotion[wCntMotion].vpKey[wCntKey];

				// ���t���[����
				Export["MaxFrame"][wCntMotion][wCntKey] = pKey->nMaxFrame;

				for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
				{
					const CMotion_Set::KeyDest* const pDest = &pKey->apDest[wCntModelParts];

					Export["ScaleTarget"][wCntMotion][wCntKey][wCntModelParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
					Export["RotTarget"][wCntMotion][wCntKey][wCntModelParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
					Export["PosTarget"][wCntMotion][wCntKey][wCntModelParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
				}
			}
		}
#endif	// _SORT_ON_EXPORT_

		// �W�F�C�\���f�[�^���V���A���C�Y
		Ofs << Export.dump(1, '	');	// ������ -> �C���f���g��, ������ -> �C���f���g�`��

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

	if (ImGui::Button("Reset"))
	{
		// �I��ԍ�����������
		m_wSelectParts = 0;
		m_wSelectKey = 0;

		// ��U���
		m_MotionSet->Release();

		// ���Đ���
		m_MotionSet = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\EXPORT\\" + m_NowSetFileName + ".json"));
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
	return &m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion];
}

//============================================================================
// �I�𒆂̃L�[�̃|�C���^���擾
//============================================================================
CMotion_Set::Key* const CMotion_Manager::GetSelectKey() const
{
	return &GetSelectMotion()->vpKey[m_wSelectKey];
}