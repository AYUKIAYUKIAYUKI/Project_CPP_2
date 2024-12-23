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
#include "sound.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "motion_set.h"
#include "object_parts.h"
#include "object_X.h"
#include "object_TextMesh.h"
#include "field_manager.h"
#include "HUD_manager.h"
#include "constellation.h"
#include "sparks.h"

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
	else if (CManager::GetKeyboard()->GetTrigger(DIK_SPACE))
	{
		CHUD_Manager::Create();
		To_Next();
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
	m_bTransition{ nullptr },
	m_pButterfly{ nullptr },
	m_vPath{},
	m_nNowFrame{ 0 },
	m_nMaxFrame{ 0 },
	m_nCntStart{ 0 },
	m_pFakePlayer{ nullptr },
	m_pTree{ nullptr }
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
	//�@�t�B�[���h�}�l�[�W���[�̃^�C�g�����������ݒ�
	CField_Manager::GetInstance()->InitForTitle();

	{ // �������𐶐�

		// ���ӗ̈�
		CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\around.json"));

		// ��
		m_pButterfly = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\BUTTERFLY\\butterfly_motion.json"));
		auto ButterflyParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\BUTTERFLY\\butterfly_param.json");
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
		pCamera->ChangeTrackPlayer(false);

		// ��
		m_pTree = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\TREE\\tree_motion.json"));
		auto TreeParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\TREE\\tree_param.json");
		m_pTree->SetRot(utility::JsonConvertToVec3(TreeParam["Rot"]));
		m_pTree->SetPos(utility::JsonConvertToVec3(TreeParam["Pos"]));

		// �^�C�g���\��
		CObject_TextMesh::Create(utility::OpenJsonFile("Data\\JSON\\TEXTMESH\\title.json"));

		// ����\��
		CObject_TextMesh::Create(utility::OpenJsonFile("Data\\JSON\\TEXTMESH\\enter.json"));
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
	{
		m_pButterfly->SetRelease();
		m_pButterfly = nullptr;
	}

	// �t�F�C�N�v���C���[��j��
	if (m_pFakePlayer != nullptr)
	{
		m_pFakePlayer->SetRelease();
		m_pFakePlayer = nullptr;
	}

	// ����j��
	if (m_pTree != nullptr)
	{
		m_pTree->SetRelease();
		m_pTree = nullptr;
	}
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
	{
		m_pTree->SetNowMotion(1);
	}

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
		if (fDistance < DISTANCE_SHOW_TITLE - 10.0f)
		{ // �������߂������̓Y�[���A�E�g�ň����Ă���

			fDistance += (DISTANCE_SHOW_TITLE - fDistance) * 0.05f;
			pCamera->SetDistance(fDistance);
		}
		else	
		{ // �J�����̈���������������

			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
			{
				// HUD�}�l�[�W���[�̐���
				CHUD_Manager::Create();

				// ���̃V�[���ֈڍs�J�n
				m_bTransition = 1;

				// �H�΂������[�V�����ɕύX
				if (m_pButterfly->GetNowMotion() != 1)
					m_pButterfly->SetNowMotion(1);

				// ���̃��[�V������ύX
				if(m_pTree->GetNowMotion() != 2)
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
	// �^�C�g���̐i�s�ɍ��킹�Ē��̋�����ω�
	if (m_bTransition)
	{ // �Q�[���V�[���֑J�ڂ��J�n������

		// ���̌����E���W���R�s�[
		D3DXVECTOR3 NewRot = m_pButterfly->GetRot(), NewPos = m_pButterfly->GetPos();

		// �J�������擾
		CCamera* pCamera = CManager::GetManager()->GetCamera();

		// �J�������W�����p
		D3DXVECTOR3 AdjustCameraPos = { 0.0f, 0.0f, 0.0f };

		// �ڕW�J�����������i�[
		float fTargetDistance = 0.0f;

		// �J������U��������
		pCamera->SetVibration(0.001f);

		// �����G�t�F�N�g���g�U����
		CConstellation::GenerateSpread(m_pButterfly->GetPos());

		// �����㏸���J�n���A���̍��x�ɒB����Ɖ�ʉ��֔�ї���
		if (NewPos.y < m_vPath[1].y - 5.0f)
		{
			/*m_Path[1]��z�͂��̎��̂�Rot.x�̐ݒ�l�Ƃ��Ĉ����A�ߖ�ł�*/
			NewRot.y += (m_vPath[1].z - NewRot.y) * 0.025f;
			NewPos.x += (m_vPath[1].x - NewPos.x) * 0.025f;
			NewPos.y += (m_vPath[1].y - NewPos.y) * 0.025f;

			// �J�����������Z�b�g
			fTargetDistance = 100.0f;
		}
		else
		{ // ��ʉ��֔�s��

			// �ڕW�n�_�̍��W���쐬
			D3DXVECTOR3 TargetPoint = { 0.0f, 60.0f, -CField_Manager::FIELD_RADIUS };

			// �ڕW�l�ւ̕�ԋ��x��ݒ�
			float fCoef = 0.0f;

			// �����ւ̐i�s�x�ŋ�����ύX
			if (TargetPoint.z != NewPos.z)
			{ // �����t�B�[���h�̔��a��艓���ꍇ

				if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 1.1f)
				{ // �ڋߒ��̏I��
					
					// �X�^�[�g�J�E���g���C���N�������g
					++m_nCntStart;

					// ���J�E���g�o�߂�
					if (m_nCntStart > 210)
					{	
						// �Q�[���V�[����
						To_Next();

						return;
					}

					// �v���C���[�Ƀ_�b�V�����[�V�������Z�b�g
					if (m_pFakePlayer->GetNowMotion() != 6 &&
						m_pFakePlayer->GetNowMotion() != 8)
					{
						m_pFakePlayer->SetNowMotion(6);
					}

					// �_�b�V�����[�V�����̍Đ����I�����Ă������s���[�V�������Z�b�g
					if (m_pFakePlayer->GetStopState())
					{
						m_pFakePlayer->SetNowMotion(8);
					}

					// ���[�V�����ɉ����ăp�����[�^���Z�b�g
					else if (m_pFakePlayer->GetNowMotion() == 6 &&
						m_nCntStart < 10)
					{
						// ��ԋ��x���Z�b�g
						fCoef = 0.001f;
					}
					else
					{
						// �J�������W����
						AdjustCameraPos.y = -45.0f + (TargetPoint.z - NewPos.z) * 0.1f;

						// ��ԋ��x���Z�b�g
						fCoef = 0.015f;
					}

					// �J��������
					fTargetDistance = 50.0f;
				}
				else if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 1.25f)
				{
					// �J�������ƂĂ��U��
					pCamera->SetVibration(0.003f);

					// �J��������
					fTargetDistance = 30.0f;

					// �J�������W����
					AdjustCameraPos.y = -10.0f;

					// ��ԋ��x���Z�b�g
					fCoef = 0.001f;
				}
				else if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 1.5f)
				{
					// �J���������ƐU��
					pCamera->SetVibration(0.002f);

					// �J��������
					fTargetDistance = 35.0;

					// �J�������W����
					AdjustCameraPos.y = -5.0f;

					// ��ԋ��x���Z�b�g
					fCoef = 0.0025f;
				}
				else if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 2.0f)
				{
					// �J�����������U��
					pCamera->SetVibration(0.001f);

					// �J��������
					fTargetDistance = 40.0f;

					// �J�������W����
					AdjustCameraPos.y = -2.5f;

					// ��ԋ��x���Z�b�g
					fCoef = 0.004f;
				}
				else if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 5.0f)
				{ // �ڋߒ��̏���

					// ���̃��[�V������ϐg���[�V�����ɕύX
					if (m_pButterfly->GetNowMotion() != 2)
					{
						m_pButterfly->SetNowMotion(2);
					}

					// �t�F�C�N�v���C���[�̋���
					if (m_pButterfly->GetStopState() && !m_pFakePlayer)
					{ // ���������āA�t�F�C�N�v���C���[���������Ȃ�

						// �t�F�C�N�v���C���[�̐���
						m_pFakePlayer = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\player_motion.json"));
						m_pFakePlayer->SetNowMotion(2);
					}
					else if (m_pFakePlayer)
					{ // �t�F�C�N�v���C���[������

						// �J�������ƂĂ��U��
						pCamera->SetVibration(0.0025f);

						// �t�F�C�N�v���C���[���o�ꃂ�[�V�������I���������s���[�V�����ɕύX
						if (m_pFakePlayer->GetStopState())
							m_pFakePlayer->SetNowMotion(3);

						// �t�H�O�͈̔͂�ύX
						CRenderer::CorrectFogEnd(550.0f);
					}

					// �J�����������Z�b�g
					fTargetDistance = 50.0f;

					// ��ԋ��x���Z�b�g
					fCoef = 0.005f;
				}
				else
				{ // ��ї����Ă���

					// �t�H�O�͈̔͂�ύX
					CRenderer::CorrectFogEnd(1500.0f);

					// �J�����������Z�b�g
					fTargetDistance = 50.0f;

					// ��ԋ��x���Z�b�g
					fCoef = 0.005f;
				}

				// ���̌����E���W�𓺑��̕����֐ڋ�
				NewRot.y += (D3DX_PI - NewRot.y) * 0.1f;
				NewPos.x += (TargetPoint.x - NewPos.x) * fCoef;
				NewPos.y += (TargetPoint.y - NewPos.y) * fCoef;
				NewPos.z += (TargetPoint.z - NewPos.z) * fCoef;
			}

			// �t�F�C�N�v���C���[����������Ă�����
			if (m_pFakePlayer)
			{
				// ���������킹��
				m_pFakePlayer->SetRot(m_pButterfly->GetRot());
				
				// ���W�����킹��
				D3DXVECTOR3 Pos = m_pButterfly->GetPos();
				Pos.z += 3.0f;
				m_pFakePlayer->SetPos(Pos);
			}
		}

		// ���̌����E���W�ݒ�
		m_pButterfly->SetRot(NewRot);
		m_pButterfly->SetPos(NewPos);

		// �J�������W�𒲐����A�v���C���[�ɒǏ]������
		pCamera->SetPosTarget(NewPos + AdjustCameraPos);

		// �J������������
		float fDistance = pCamera->GetDistance();
		fDistance += (fTargetDistance - fDistance) * 0.05f;
		pCamera->SetDistance(fDistance);

		// �}�b�v�V���{���̓��ꋓ��
		/* �����t�B�[���h����ǂꂭ�炢����Ă邩��n�� */
		CHUD_Manager::GetInstance()->SpecialMapSymbol((NewPos.z + CField_Manager::FIELD_RADIUS * 1.1f) / 30.0f);
	}
	else if (m_nNowFrame < m_nMaxFrame)
	{ // ����̉H�΂������Ԓ�

		// �����G�t�F�N�g���g�U����
		CConstellation::GenerateSpread(m_pButterfly->GetPos());

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