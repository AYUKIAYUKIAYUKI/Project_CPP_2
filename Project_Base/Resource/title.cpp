//============================================================================
// 
// �^�C�g�� [title.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "motion_set.h"
#include "object_parts.h"
#include "object_X.h"
#include "field_manager.h"
#include "constellation.h"
#include "sparks.h"

#include "object_3D.h"

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CTitle::Update()
{
	// ���N���X�̍X�V����
	CScene::Update();

	// �ړ����o�p�̊��Ԃ��C���N�������g
	m_nNowFrame++;

	// �������̍X�V
	UpdateEnvironment();

#ifdef _DEBUG	// �p�����[�^�m�F�p
	if (CManager::GetKeyboard()->GetTrigger(DIK_F7))
	{
		CObject::ReleaseAll();
		CScene_Manager::ChangeScene(Create());
	}
#endif // _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CTitle::Draw()
{
	// ���N���X�̕`�揈��
	CScene::Draw();
}

//============================================================================
// ���̃V�[����
//============================================================================
void CTitle::To_Next()
{
	// �Q�[���V�[���֕ύX
	CScene_Manager::ChangeScene(CGame::Create());
}

//============================================================================
// �^�C�g���𐶐�
//============================================================================
CTitle* CTitle::Create()
{
	// �^�C�g���𐶐�
	CTitle* pNew = DBG_NEW CTitle();

	if (pNew == nullptr)
	{
		assert(false && "�^�C�g���̐����Ɏ��s");
	}

	// �^�C�g���̏����ݒ�
	if (FAILED(pNew->Init()))
	{
		assert(false && "�^�C�g���̏����ݒ�Ɏ��s");
	}

	return pNew;
}

//============================================================================
// 
// protected�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CTitle::CTitle() :
	CScene{},
	m_pKESE{ nullptr },
	m_bTransition{ nullptr },
	m_pButterfly{ nullptr },
	m_vPath{},
	m_nNowFrame{ 0 },
	m_nMaxFrame{ 0 },
	m_pTree{ nullptr },
	m_pHole{ nullptr }
{
	m_vPath.clear();
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTitle::~CTitle()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CTitle::Init()
{
	//�@�t�B�[���h�}�l�[�W���[�^�C�g�����������ݒ�
	CField_Manager::GetInstance()->InitForTitle();

	// �������𐶐�
	{
		// ���ӗ̈�
		CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\around.json"));

		// ��
		m_pButterfly = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\butterfly_motion.json"));
		auto ButterflyParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\butterfly.json");
		m_pButterfly->SetRot(utility::JsonConvertToVec3(ButterflyParam["Rot"]));
		m_pButterfly->SetPos(utility::JsonConvertToVec3(ButterflyParam["Pos"]));
		m_pButterfly->SetNowMotion(1);
		for (WORD wCntPath = 0; wCntPath < ButterflyParam["Path"].size(); ++wCntPath)
			m_vPath.push_back(utility::JsonConvertToVec3(ButterflyParam["Path"][wCntPath]));
		m_nMaxFrame = ButterflyParam["MaxFrame"];

		// �J��������ύX
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetDistance(100.0f);
		pCamera->SetUpAdjust(0.0f);
		pCamera->SetPos(utility::JsonConvertToVec3(ButterflyParam["Pos"]));
		pCamera->SetPosTarget(utility::JsonConvertToVec3(ButterflyParam["Pos"]));

		// ��
		m_pTree = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\tree_motion.json"));
		auto TreeParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\tree.json");
		m_pTree->SetRot(utility::JsonConvertToVec3(TreeParam["Rot"]));
		m_pTree->SetPos(utility::JsonConvertToVec3(TreeParam["Pos"]));

		/* �ŒႷ����^�C�g�����\�� */
		CObject_3D* p = CObject_3D::Create();
		JSON Json = utility::OpenJsonFile("Data\\JSON\\debug_param.json");
		p->BindTex(CTexture_Manager::TYPE::TITLE);
		p->SetPos(utility::JsonConvertToVec3(Json["Pos"]));
		p->SetSize(utility::JsonConvertToVec3(Json["Size"]));

		/*�ŒႷ����X�^�[�g�\��*/
		m_pKESE = CObject_3D::Create();
		//JSON Json = utility::OpenJsonFile("Data\\JSON\\debug_param.json");
		m_pKESE->BindTex(CTexture_Manager::TYPE::START);
		m_pKESE->SetPos(utility::JsonConvertToVec3(Json["Pos"]));
		m_pKESE->SetPos({ m_pKESE->GetPos().x, m_pKESE->GetPos().y + -100.0f, m_pKESE->GetPos().z });
		m_pKESE->SetSize(utility::JsonConvertToVec3(Json["Size"]));
		m_pKESE->SetAlpha(0.0f);

		// ��
		//m_pHole = CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\hole.json"));
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTitle::Uninit()
{
	// ����j��
	if (m_pButterfly != nullptr)
		m_pButterfly->SetRelease();

	// ����j��
	if (m_pTree != nullptr)
		m_pTree->SetRelease();
}

//============================================================================
// �������̍X�V
//============================================================================
void CTitle::UpdateEnvironment()
{
	// ���̎��ӂɉ΂̕��𔭐�
	CSparks::AreaGenerate(m_pButterfly->GetPos());

	// ���̃��[�V��������~���Ă����獂�����[�V�����ɕύX
	if (m_pTree->GetStopState())
		m_pTree->SetNowMotion(1);

	// ���̍X�V
	UpdateButterfly();

	// �Q�[���V�[���֑J�ڂ��J�n������J�����Œ�͖���
	if (m_bTransition)
		return;

	// �ړ����o���I����Ă�����J�������Œ�
	if (m_nNowFrame >= m_nMaxFrame)
	{
		// �J�����̍��W���Œ�A�������R�s�[
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetPosTarget({ -400.0f, -50.0f, -4000.0f });
		float fDistance = pCamera->GetDistance();

		// �J�����̋����ɉ����ċ�����ύX
		if (fDistance < 300.0f)
		{ // �������߂������̓Y�[���A�E�g�ň����Ă���

			// �������f
			pCamera->SetDistance(fDistance += 5.0f);
		}
		else	
		{ // �J�����̈���������������

			/*�ŒႷ����X�^�[�g�\��*/
			{
				float fAlpha = m_pKESE->GetAlpha();
				if (fAlpha < 1.0f)
					fAlpha += 0.025f;
				m_pKESE->SetAlpha(fAlpha);
			}

			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
			{
				// HUD�p�̏����ݒ�
				CField_Manager::GetInstance()->InitForHUD();

				// ���̃V�[���ֈڍs�J�n
				m_bTransition = 1;

				// ���̃��[�V������ύX
				m_pTree->SetNowMotion(2);
			}
		}
	}
}

//============================================================================
// ���̍X�V
//============================================================================
void CTitle::UpdateButterfly()
{
	// �H�΂������[�V�����̎�
	if (m_pButterfly->GetNowMotion() == 1)
	{
		// �����G�t�F�N�g���g�U����
		CConstellation::GenerateSpread(m_pButterfly->GetPos());
	}

	// �^�C�g���̐i�s�ɍ��킹�Ē��̋�����ω�
	if (m_bTransition)
	{ // �Q�[���V�[���֑J�ڂ��J�n������

		/*�ŒႷ����X�^�[�g�\��*/
		{
			static int nNum = 0;
			++nNum;
			if (nNum > 6)
			{
				nNum = 0;
				float fAlpha = m_pKESE->GetAlpha();
				if (fAlpha >= 1.0f)
					fAlpha = 0.0f;
				else if (fAlpha <= 0.0f)
					fAlpha = 1.0f;
				else
					fAlpha = 1.0f;
				m_pKESE->SetAlpha(fAlpha);
			}
		}

		// �H�΂������[�V�����ɕύX
		if (m_pButterfly->GetNowMotion() != 1)
			m_pButterfly->SetNowMotion(1);

		// ���̌����E���W���R�s�[
		D3DXVECTOR3 NewRot = m_pButterfly->GetRot(), NewPos = m_pButterfly->GetPos();

		// �����㏸���J�n���A���̍��x�ɒB����Ɖ�ʉ��֔�ї���
		if (NewPos.y < m_vPath[1].y - 5.0f)
		{
			/*m_Path[1]��z�͂��̎��̂�Rot.x�̐ݒ�l�Ƃ��Ĉ����A�ߖ�ł�*/
			NewRot.y += (m_vPath[1].z - NewRot.y) * 0.025f;
			NewPos.x += (m_vPath[1].x - NewPos.x) * 0.025f;
			NewPos.y += (m_vPath[1].y - NewPos.y) * 0.025f;
		}
		else
		{ // ��ʉ��֔�s��

			// �����̍��W���R�s�[
			D3DXVECTOR3 PosTarget = { 0.0f, 0.0f, 0.0f };

			// �����ւ̐i�s�x�ŋ�����ύX
			if (PosTarget.z - NewPos.z > CField_Manager::FIELD_RADIUS * 1.5f)
			{ // �����t�B�[���h�̔��a��艓���ꍇ

				// �����E���W�𓺑��̕����֐ڋ�
				NewRot.y += (D3DX_PI - NewRot.y) * 0.1f;
				NewPos.x += (PosTarget.x - NewPos.x) * 0.005f;
				NewPos.y += (PosTarget.y - NewPos.y) * 0.005f;
				NewPos.z += (PosTarget.z - NewPos.z) * 0.005f;

				// �J�����Ǐ]
				CCamera* pCamera = CManager::GetManager()->GetCamera();
				pCamera->SetPosTarget(NewPos);

				// �J�����������R�s�[
				float fDistance = pCamera->GetDistance();

				// �����������̓Y�[���C���Ŕ����Ă���
				if (fDistance > 75.0f)
					pCamera->SetDistance(fDistance += -5.0f);
			}
			else
			{ // �����t�B�[���h�͈̔͂قǂɋ߂Â�����

				// �Q�[���V�[����
				To_Next();
				return;
			}
		}

		// ���̌����E���W�ݒ�
		m_pButterfly->SetRot(NewRot);
		m_pButterfly->SetPos(NewPos);
	}
	else if (m_nNowFrame < m_nMaxFrame)
	{ // ����̉H�΂������Ԓ�

		// �����ڕW�n�_�֔�s
		D3DXVECTOR3 NewPos = m_pButterfly->GetPos();
		float fFrameCoef = static_cast<float>(m_nMaxFrame - m_nNowFrame);
		NewPos += (m_vPath[0] - NewPos) / fFrameCoef;

		// ����ɁA��тȂ���㉺�ɗh���
		NewPos.y += utility::GetRandomValue<float>() * 0.01f;
		m_pButterfly->SetPos(NewPos);

		// �J�����͂�����x�Ǐ] (���S��������ƃu���u����)
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		D3DXVECTOR3 CameraPos = pCamera->GetPos();
		pCamera->SetPos({ NewPos.x, CameraPos.y, NewPos.z });
		pCamera->SetPosTarget(NewPos);
	}
	else if (m_nNowFrame == m_nMaxFrame)
	{ // �ړ�������

		// �����Ƃǂ܂胂�[�V�����ɕύX
		m_pButterfly->SetNowMotion(0);
	}
}