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
#include "fan.h"

// ���͎擾�p
#include "manager.h"

// �f�o�b�O�\���p
#include "renderer.h"

// �I�u�W�F�N�g�p
#include "block.h"
#include "object_HUD.h"
#include "player.h"

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
	// �v���C���[������
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		// �v���C���[�ւ̃|�C���^��ێ�
		m_pPlayer = CUtility::DownCast(m_pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	}
	else
	{
		assert(false && "�v���C���[�̌������ʂ�����܂���ł���");
	}

	// �v���C���[�̗̑͂𐶐�
	for (WORD i = 0; i < CPlayer::MAX_LIFE; ++i)
	{
		std::string FilePath = "Data\\JSON\\HUD\\playerlife\\" + to_string(i) + ".json";
		m_pPlayerLife[i] = CObject_HUD::Create(FilePath);
		m_pPlayerLife[i]->BindTex(CTexture_Manager::TYPE::CIRCLE);
	}

	// �v���C���[�̃Q�[�W�𐶐�
	m_pPlayerGaugeWindow = CObject_HUD::Create("Data\\JSON\\HUD\\playergauge.json");
	m_pPlayerGaugeWindow->BindTex(CTexture_Manager::TYPE::PLAYERGAUGE);

	// �v���C���[�̃Q�[�W�E�B���h�E�𐶐�
	m_pPlayerGaugeWindow = CObject_HUD::Create("Data\\JSON\\HUD\\playergaugewindow.json");
	m_pPlayerGaugeWindow->BindTex(CTexture_Manager::TYPE::PLAYERGAUGEWINDOW);

	// �~���̔���𐶐�
	m_pCylinderCollider = CObject_X::Create(static_cast<int>(CObject::LAYER::BACK));
	m_pCylinderCollider->Init();
	m_pCylinderCollider->BindModel(CModel_X_Manager::TYPE::CYLINDERCOLLIDER);
	m_pCylinderCollider->SetPos(m_pPlayer->GetPos());
	m_pCylinderCollider->SetRot(m_pPlayer->GetRot());
	m_pCylinderCollider->SetScale(GENERATE_RANGE_RADIUS);
	m_pCylinderCollider->SetAlpha(0.5f);

	// ��`�𐶐�
	m_pFan = CFan::Create();

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
	// ��`�̕��p���v���C���[�̕��p��
	m_pFan->SetDirection(m_pPlayer->GetDirection());

	// ��`�̍X�V����
	m_pFan->Update();

	// ���̐������\�b�h
	TestCreate();

	// ���̔j�����\�b�h
	TestDelete();

	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
	{
		// ���̑S�j�����\�b�h
		TestDeleteAll();
	}

	// HUD�̍X�V
	UpdateHUD();
}

//============================================================================
// �`�揈��
//============================================================================
void CField_Manager::Draw()
{
	// ��`�̕`�揈��
	m_pFan->Draw();
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
	m_pPlayer{ nullptr },
	m_pPlayerGauge{ nullptr },
	m_pPlayerGaugeWindow{ nullptr },
	m_pCylinderCollider{ nullptr },
	m_pFan{ nullptr }
{
	for (WORD i = 0; i < CPlayer::MAX_LIFE; ++i)
	{
		m_pPlayerLife[i] = nullptr;
	}
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
	// ��`��j��
	if (m_pFan != nullptr)
	{
		m_pFan->Release();	// ���
		m_pFan = nullptr;	// �|�C���^��������
	}
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
		CInputKeyboard* pKeyboard = CManager::GetKeyboard();
		CInputPad* pPad = CManager::GetPad();

		/* �X�N���[����ʓ��ŁA�ǂ���̕������ړ����Ă����� */
		//float ScreenX = 0.0f;

		/* ���O�̕��p�̕ύX�����o */
		if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
		{ // �J�������猩�č���

		}
		else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
		{ // �J�������猩�ĉE��

		}

		/* �ړ����Ă��������ɍ��킹�āA�͈͂̒[����V���Ȓn�`���o�ꂵ�Ă��� */

		// �v���C���[�^�O���擾
		if (CObject::FindSpecificObject(CObject::TYPE::PLAYER))
		{
			// �������W�v�Z�p
			const float&	fDirection = m_pPlayer->GetDirection();	// �v���C���[�̕��p���R�s�[
			Vec3			NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// �u���b�N�p�̍��W�E�������쐬
			float			fRandomRange = 0.0f;					// �����_���ȕ��p�͈�

			// �@�v���C���[�̐i�s�����ɉ����Đ��������悤�ɕ␳

			// �j���͈͂ɂ͂ݏo�������������悤�ɒ���
			/* �������W�����_�̏ꍇ�A�����͈͂̔��a���t�B�[���h�̔��a�������Ɩ������[�v */
			//do
			//{
#if 0
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
#else
				NewPos = { 0.0f,0.0f, FIELD_RADIUS };
#endif
			//} while (!CUtility::CylinderAndSphere(m_pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, NewPos, 10.0f));

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

				// �j�����@�̕ύX
#if 0
				m_pCylinderCollider->SetPos(m_pPlayer->GetPos());
				m_pCylinderCollider->SetRot(m_pPlayer->GetRot());

				// �t�ɁA�~���͈͊O�̏ꍇ����
				if (!CUtility::CylinderAndSphere(m_pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, pBlock->GetPos(), 10.0f))
				{
					pBlock->SetRelease();
				}
#else
				m_pCylinderCollider->SetPos(Vec3(FLT_MAX, FLT_MAX, FLT_MAX));

				if (m_pFan->DetectInFanRange(pBlock->GetPos()))
				{
					pBlock->SetRelease();
				}
#endif
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

//============================================================================
// HUD�̍X�V����
//============================================================================
void CField_Manager::UpdateHUD()
{
	// �̗͗ʕ���HUD���\�������悤�ɒ���
	for (WORD i = CPlayer::MAX_LIFE; i > 0; --i)
	{
		// ���W���擾
		Vec3 NewPosTarget = m_pPlayerLife[i - 1]->GetPosTarget();

		if (i > m_pPlayer->GetLife())
		{
			NewPosTarget.y = -100.0f;
		}
		else
		{
			NewPosTarget.y = 100.0f;
		}

		// ���W�𔽉f
		m_pPlayerLife[i - 1]->SetPosTarget(NewPosTarget);
	}

#if 0	// HUD�����̊m�F

	if (CManager::GetKeyboard()->GetPress(DIK_Z))
	{
		m_pPlayerGaugeWindow->SetVibration();
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_X))
	{
		m_pPlayerGaugeWindow->SetWaving();
	}

#endif
}