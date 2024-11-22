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

// �V�[���؂�ւ��p
#include "fade.h"

// �I�u�W�F�N�g�p
#include "block.h"
#include "collision.h"
#include "object_HUD.h"
#include "player.h"
#include "sparks.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
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
	// �}�b�v�\�����쐬
	m_pMap = CObject_HUD::Create("Data\\JSON\\HUD\\map.json");
	m_pMap->BindTex(CTexture_Manager::TYPE::MAP);

	// �v���C���[���������ێ����Ă���
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		// �v���C���[�ւ̃|�C���^��ێ�
		m_pPlayer = utility::DownCast(m_pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	}
	else
	{
		assert(false && "�v���C���[�̌������ʂ�����܂���ł���");
	}

	// �v���C���[�̗͕̑\���𐶐�
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

	// ��`�𐶐�
	m_pFan = CFan::Create();

#if CHANGE_FIELRDCREATE_STYLE

	// �~�̐���
	DEBUG_CIRCLE();

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
	// �΂̕��𐶐�
	CSparks::AutoGenerate();

	// ��`�̕��p���v���C���[�̕��p��
	m_pFan->SetDirection(m_pPlayer->GetDirection());

	// ��`�̍X�V����
	m_pFan->Update();

#if !CHANGE_FIELRDCREATE_STYLE

	// �u���b�N�̎�������
	GenerateBlock();

	// �u���b�N�̎����폜
	DestroyBlock();

#endif	// CHANGE_FIELRDCREATE_STYLE

	// ���̑S�j�����\�b�h
	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
	{
		DestroyAllBlock();
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
// �R���X�g���N�^
//============================================================================
CField_Manager::CField_Manager() :
	m_pMap{ nullptr },
	m_pPlayer{ nullptr },
	m_pPlayerGauge{ nullptr },
	m_pPlayerGaugeWindow{ nullptr },
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
// �u���b�N�̎�������
//============================================================================
void CField_Manager::GenerateBlock()
{
	// �u���b�N�����J�E���g
	int nCntBlock = CObject::CountSpecificObject(CObject::TYPE::BLOCK);

#ifdef _DEBUG	// �u���b�N���̕\��
	CRenderer::SetDebugString("�u���b�N��:" + to_string(nCntBlock));
#endif	// _DEBUG

	// �u���b�N��������ɖ����Ȃ����
	while (nCntBlock < MAX_BLOCK)
	{
		// �������W�v�Z�p
		const float&	fDirection = m_pPlayer->GetDirection();	// �v���C���[�̕��p���R�s�[
		Vec3			NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// �u���b�N�p�̍��W�E�������쐬
		float			fRandomRange = 0.0f;					// �����_���ȕ��p�͈�

		// �j���͈͂ɂ͂ݏo�������������悤�ɒ���
		/* �������W�����_�̏ꍇ�A�����͈͂̔��a���t�B�[���h�̔��a�������Ɩ������[�v */
		do
		{
			// �����_���ɕ��p�����炷
			fRandomRange = utility::GetRandomValue<float>();

			// �����p�̍��W������
			NewPos.x = cosf(fDirection + fRandomRange) * FIELD_RADIUS;
			NewPos.y = fabsf(utility::GetRandomValue<float>());
			NewPos.z = sinf(fDirection + fRandomRange) * FIELD_RADIUS;

			//// �u���b�N���m�̕������o
			//if (DetectAdjacentBlock(NewPos))
			//{
			//	NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
			//}

		} while (!m_pFan->DetectInFanRange(NewPos));

		// ����������
		NewRot.y = -(fDirection + fRandomRange) + D3DX_PI * -0.5f;

		// �u���b�N�𐶐�
		CBlock::Create(NewPos, NewRot);

		// �u���b�N�����J�E���g�A�b�v
		nCntBlock++;
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

			/* ����͎����I�ɂ��݂��̋����݂̂��l������ */
			const Vec3& Vec = pBlock->GetPos() - Pos;

			/* ������x�ڋ߂��Ă��܂��Ă���u���b�N�����݂���ꍇ */
			if (sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= pBlock->GetSize().x)
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
// �u���b�N�̎����폜
//============================================================================
void CField_Manager::DestroyBlock()
{
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::MIDDLE);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			// ��`�͈͓̔��Ƀu���b�N��������Δj��
			if (!m_pFan->DetectInFanRange(pBlock->GetPos()))
			{
				pBlock->SetRelease();
			}
		}

		pObj = pObj->GetNext();
	}
}

//============================================================================
// �S�u���b�N�̍폜
//============================================================================
void CField_Manager::DestroyAllBlock()
{
	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::MIDDLE);

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

//============================================================================
// �f�o�b�O�T�[�N��
//============================================================================
void CField_Manager::DEBUG_CIRCLE()
{
	float fDirection = D3DX_PI;						// ���p
	float fAdder = (D3DX_PI * 2.0f) / MAX_BLOCK;	// ������

	for (WORD i = 0; i < MAX_BLOCK; ++i)
	{
		// �����p�̍��W������
		Vec3 NewPos = VEC3_INIT;
		NewPos.x = cosf(fDirection) * FIELD_RADIUS;
		NewPos.y = 40.0f - utility::GetRandomValue<float>() * 0.15f;
		NewPos.z = -sinf(fDirection) * FIELD_RADIUS;

		// �����p�̌���������
		Vec3 NewRot = VEC3_INIT;
		//NewRot.y = utility::GetRandomValue<float>();
		NewRot.y = fDirection;

		// �u���b�N�𐶐�
		CBlock::Create(NewPos, NewRot);

		// ���p�𑝉�
		fDirection += fAdder;
	}
}