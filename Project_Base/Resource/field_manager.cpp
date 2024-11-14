//============================================================================
// 
// �t�B�[���h�}�l�[�W���[ [field_manager.cpp]79
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

// �V�[���؂�ւ��p
#include "fade.h"

// �I�u�W�F�N�g�p
#include "block.h"
#include "object_HUD.h"
#include "player.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
#define SHOW_CYLINDER_COLLIDER 0	// �~������̕\���؂�ւ�
#define CHANGE_FIELRDCREATE_STYLE 1	// �u���b�N�̐������@�؂�ւ�

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CField_Manager* CField_Manager::m_pField_Manager = nullptr;	// �t�B�[���h�}�l�[�W���[�̖{��

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
	// �}�b�v�\��
	m_pMap = CObject_HUD::Create("Data\\JSON\\HUD\\map.json");
	m_pMap->BindTex(CTexture_Manager::TYPE::MAP);

	// �v���C���[������
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		// �v���C���[�ւ̃|�C���^��ێ�
		m_pPlayer = utility::DownCast(m_pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
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

#if SHOW_CYLINDER_COLLIDER

	// �~���̔���𐶐�
	m_pCylinderCollider = CObject_X::Create(CObject::LAYER::BACK);
	m_pCylinderCollider->Init();
	m_pCylinderCollider->BindModel(CModel_X_Manager::TYPE::CYLINDERCOLLIDER);
	m_pCylinderCollider->SetPos(m_pPlayer->GetPos());
	m_pCylinderCollider->SetRot(m_pPlayer->GetRot());
	m_pCylinderCollider->SetScale(GENERATE_RANGE_RADIUS);
	m_pCylinderCollider->SetAlpha(0.5f);

#endif	// SHOW_CYLINDER_COLLIDER

	// ��`�𐶐�
	m_pFan = CFan::Create();

#if CHANGE_FIELRDCREATE_STYLE

	// �~�̐���
	TestCircle();

#endif	// CHANGE_FIELRDCREATE_STYLE

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CField_Manager::Release()
{
	if (m_pField_Manager != nullptr)
	{
		// �I������
		m_pField_Manager->Uninit();

		// �����������
		delete m_pField_Manager;

		// �|�C���^��������
		m_pField_Manager = nullptr;
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

#if !CHANGE_FIELRDCREATE_STYLE

	// ���̐������\�b�h
	TestCreate();

	// ���̔j�����\�b�h
	TestDelete();

#endif	// CHANGE_FIELRDCREATE_STYLE

	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
	{
		// ���̑S�j�����\�b�h
		TestDeleteAll();
	}

	// HUD�̍X�V
	UpdateHUD();

	if (m_pPlayer->GetLife() <= 0)
	{
		CFade::SetFade(CScene::MODE::RESULT);
	}
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
	if (m_pField_Manager == nullptr)
	{
		// ����
		m_pField_Manager->Create();
	}

	return m_pField_Manager;
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
	m_pMap{ nullptr },
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
	if (m_pField_Manager != nullptr)
	{ // ��d�����֎~
		assert(false);
	}

	// �C���X�^���X�𐶐�
	m_pField_Manager = DBG_NEW CField_Manager();
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
// �~�̐������\�b�h
//============================================================================
void CField_Manager::TestCircle()
{
	float fDirection = D3DX_PI * 0.25f;				// ���p
	float fAdder = (D3DX_PI * 2.0f) / MAX_BLOCK;	// ������

	for (WORD i = 0; i < MAX_BLOCK; ++i)
	{
		// �����p�̍��W������
		Vec3 NewPos = VEC3_INIT;
		NewPos.x = cosf(fDirection) * FIELD_RADIUS;
		NewPos.y = 10.0f;
		NewPos.z = -sinf(fDirection) * FIELD_RADIUS;
		
		// �����p�̌���������
		Vec3 NewRot = VEC3_INIT;
		NewRot.y = fDirection;

		// �u���b�N�𐶐�
		CBlock::Create(NewPos, NewRot);

		// ���p�𑝉�
		fDirection += fAdder;
	}
}

//============================================================================
// ���̐������\�b�h
//============================================================================
void CField_Manager::TestCreate()
{
	// �u���b�N�����J�E���g
	int nCntBlock = CObject::CountSpecificObject(CObject::TYPE::BLOCK);

#ifdef _DEBUG	// �u���b�N���̕\��

	CRenderer::SetDebugString("�u���b�N��:" + to_string(nCntBlock));

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
			do
			{
#if 1
				// �����_���ɕ��p�����炷
				fRandomRange = utility::GetRandomValue<float>();

				// �����p�̍��W������
				NewPos.x = cosf(fDirection + fRandomRange) * FIELD_RADIUS;
				NewPos.y = fabsf(utility::GetRandomValue<float>());
				NewPos.z = sinf(fDirection + fRandomRange) * FIELD_RADIUS;

				// �u���b�N���m�̕������o
				if (DetectAdjacentBlock(NewPos))
				{
					NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
				}
#else
				NewPos = { 0.0f,0.0f, FIELD_RADIUS };
#endif
			} while (!utility::CylinderAndSphere(m_pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, NewPos, 10.0f));

			// ����������
			NewRot.y = 0.0f;

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
bool CField_Manager::DetectAdjacentBlock(const D3DXVECTOR3& Pos)
{
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			// �������ɂ�����u���b�N�̗אڂ����o����

			/* ����͎����I�ɂ��݂��̋����݂̂��l������ */
			const Vec3& Vec = pBlock->GetPos() - Pos;

			/* ������x�ڋ߂��Ă��܂��Ă���u���b�N�����݂���ꍇ */
			if (sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= pBlock->GetSize().x * 3.0f)
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
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::MIDDLE);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �v���C���[�^�O���擾
			if (CObject::FindSpecificObject(CObject::TYPE::PLAYER))
			{
				// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
				CBlock* pBlock = nullptr;
				pBlock = utility::DownCast(pBlock, pObj);

				// �j�����@�̕ύX
#if 1
				m_pCylinderCollider->SetPos(m_pPlayer->GetPos());
				m_pCylinderCollider->SetRot(m_pPlayer->GetRot());

				// �t�ɁA�~���͈͊O�̏ꍇ����
				if (!utility::CylinderAndSphere(m_pPlayer->GetPos(), GENERATE_RANGE_RADIUS, GENERATE_RANGE_RADIUS, pBlock->GetPos(), 10.0f))
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
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::BG);

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
			NewPosTarget.y = 75.0f;
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