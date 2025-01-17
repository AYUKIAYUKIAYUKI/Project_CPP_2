//============================================================================
// 
// �t�B�[���h�r���_�[ [field_builder.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "field_builder.h"
#include "field_type.h"
#include "field_type_normal.h"
#include "field_manager.h"
#include "player.h"
#include "fan.h"

// �V�X�e������
#include "manager.h"
#include "renderer.h"
#include "sound.h"

// �����I�u�W�F�N�g
#include "block.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"
#include "life.h"
#include "wings.h"
#include "boots.h"

/* �C���p */
#define SAFE 0

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CField_Builder::Update()
{
	// ��`�͈͂̍X�V
	UpdateFan();

	// �r���_�[�̍X�V
	UpdateBuilder();

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebug();
#endif // _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CField_Builder::Draw()
{
#ifdef _DEBUG
	// ��`�͈͂̕`��
	m_pFan->Draw();
#endif // _DEBUG
}

//============================================================================
// �W�����v�����񐔂̃C���N�������g
//============================================================================
void CField_Builder::IncrementCntJump()
{
	++m_ActionData.nCntJump;
}

//============================================================================
// �_�b�V�������񐔂̃C���N�������g
//============================================================================
void CField_Builder::IncrementCntDash()
{
	++m_ActionData.nCntDash;
}

//============================================================================
// �U�������񐔂̃C���N�������g
//============================================================================
void CField_Builder::IncrementCntSlash()
{
	++m_ActionData.nCntSlash;
}

//============================================================================
// �u���b�N�̔j�󐔂��擾
//============================================================================
int CField_Builder::GetCntDestroyBlock()
{
	return m_nCntDestroyBlock;
}

//============================================================================
// �v���C���[���Z�b�g
//============================================================================
void CField_Builder::SetSyncPlayer(const CPlayer* const pPlayer)
{
	m_pSyncPlayer = pPlayer;
}

//============================================================================
// ����
//============================================================================
CField_Builder* CField_Builder::Create()
{
	// �C���X�^���X�𐶐�
	CField_Builder* pNewInstance = DBG_NEW CField_Builder();

	// �������s
	if (pNewInstance == nullptr)
	{
		assert(false && "�t�B�[���h�r���_�[�̐����Ɏ��s");
	}

	// �t�B�[���h�r���_�[�̏����ݒ�
	if (FAILED(pNewInstance->Init()))
	{
		assert(false && "�t�B�[���h�r���_�[�̏����ݒ�G���[");
	}

	return pNewInstance;
}

//============================================================================
// ���
//============================================================================
void CField_Builder::Release()
{
	if (this != nullptr)
	{
		// �I������
		Uninit();

		// �����������
		delete this;
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CField_Builder::CField_Builder() :
	m_upFieldType{ nullptr },
	m_nCntDestroyBlock{ 0 },
	m_pSyncPlayer{ nullptr },
	m_pFan{ nullptr }
{
	// �A�N�V�����f�[�^�̏�����
	m_ActionData.nCntDash = 0;
	m_ActionData.nCntJump = 0;
	m_ActionData.nCntSlash = 0;
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CField_Builder::~CField_Builder()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CField_Builder::Init()
{
	// �ŏ��̃t�B�[���h�^�C�v�𐶐�
	m_upFieldType = std::unique_ptr<CField_Type>(std::make_unique<CField_Type_Normal>().release());

	// ��`�͈͂𐶐�
	m_pFan = CFan::Create();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CField_Builder::Uninit()
{
	// ��`�͈͂�j��
	if (m_pFan != nullptr)
	{
		m_pFan->Release();	// ���
		m_pFan = nullptr;	// �|�C���^��������
	}
}

//============================================================================
// ��`�̍X�V
//============================================================================
void CField_Builder::UpdateFan()
{
	// �v���C���[��񂪃Z�b�g����Ă��Ȃ���Ώ������s��Ȃ�
	if (m_pSyncPlayer == nullptr)
		return;

	// �v���C���[�̌��݂̕��p���`�̕��p�ɂ���
	m_pFan->SetDirection(m_pSyncPlayer->GetDirection());

	// ��`�͈͂̍X�V����
	m_pFan->Update();
}

//============================================================================
// �r���_�[�̍X�V
//============================================================================
void CField_Builder::UpdateBuilder()
{
	// �u���b�N�̔j��J�E���g������ɒB���Ă����珈�����s��Ȃ�
	if (m_nCntDestroyBlock >= CField_Manager::MAX_DESTROY_BLOCK)
		return;

	// �t�B�[���h�^�C�v�̕���
	//BranchFieldType();

	// �A�C�e���̎�������
	GenerateItem();

	// �v���C���[�̖ڕW���W�ւ̃x�N�g�����쐬
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	// �v���C���[��������x�ړ����Ă��邱�Ƃ����m�������̂�
	if (Norm.x * Norm.x + Norm.z * Norm.z > 0.1f)
	{
		// �u���b�N�̎�������
		GenerateBlock();
	}

	// �j�󔻒�
	DestroyCheck();

#ifdef _DEBUG	
	// �u���b�N��S�폜
	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
		DestroyAllBlock();
#endif // _DEBUG
}

//============================================================================
// �t�B�[���h�^�C�v�̕���
//============================================================================
void CField_Builder::BranchFieldType()
{
	/* �����͉��炩�̕����ݒ�\��ł� */
}

//============================================================================
// �A�C�e���̎�������
//============================================================================
void CField_Builder::GenerateItem()
{
	// ���ɃA�C�e����1�ȏ㑶�݂��Ă���Ώ��������Ȃ�
	if (CObject::FindSpecificObject(CObject::TYPE::ITEM))
		return;

	// �A�C�e���p�|�C���^
	CItem* pItem = nullptr;

	// �A�C�e���o���e�[�u��
	if (!m_pSyncPlayer->IsEnabledWings())
	{
		// ���V���L��������Ă��Ȃ���ΉH�𐶐�
		pItem = CWings::Create();
	}
	else if (!m_pSyncPlayer->IsEnabledBoots())
	{
		// ���L��������Ă��Ȃ���΃u�[�c�𐶐�
		pItem = CBoots::Create();
	}
	
	// ���ʕt�^�A�C�e����������������Ă��Ȃ��Ȃ�
	if (!pItem)
	{
		// ���C�t�𐶐�
		pItem = CLife::Create();
	}

	do { // ���̕��p�ɂ�������W���A��`�͈͓��ł���Ε��p���Ē��I����

		// ���p�������_���ɐݒ�
		pItem->SetDirection(fabsf(utility::GetRandomValue<float>()));

	} while (m_pFan->DetectInFanRange(pItem->GetPos()));

	// Y���W�������_���ɐݒ�
	float fHeight = fabsf(utility::GetRandomValue<float>());
	if (fHeight >= 125.0f)
	{
		fHeight = 125.0f;
	}
	pItem->SetPosY(fHeight);

	// �`��O�Ɉ�x�X�V
	pItem->Update();

	// �A�C�e���o������炷
	CSound::GetInstance()->Play(CSound::LABEL::IAPPEAR);
}

//============================================================================
// /* �ύX�\�� */
//============================================================================
void CField_Builder::GenerateBlock()
{
	// �������W�v�Z�p ((���p + ��`���̊p�x)�̏ꏊ�������|�C���g)
	float fDirection = m_pSyncPlayer->GetDirection();	// �v���C���[�̌��݂̕��p���R�s�[
	float fRange = m_pFan->GetRange();					// ��`�͈͂̕����R�s�[
	Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;		// �u���b�N�p�̍��W�E�������쐬

	// ���ݍ��W�ƖڕW���W�ɑ΂����_����̕����x�N�g�����쐬
	Vec3 OldVec = m_pSyncPlayer->GetPos() - VEC3_INIT, NewVec = m_pSyncPlayer->GetPosTarget() - VEC3_INIT;
	D3DXVec3Normalize(&OldVec, &OldVec);
	D3DXVec3Normalize(&NewVec, &NewVec);

	// 2�{�̕����x�N�g���̊O�ς��쐬
	float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

	// ���Ɉړ����Ă���ꍇ�p�x�𔽓]������
	if (fCross < 0.0f)
		fRange = -fRange;

	// �j���͈͂ɂ͂ݏo�������������悤�ɒ���
	/* �������W�����_�̏ꍇ�A�����͈͂̔��a���t�B�[���h�̔��a�������Ɩ������[�v */
	do
	{
		// �����p�̍��W������
		NewPos.x = cosf(fDirection + fRange) * CField_Manager::FIELD_RADIUS;
		NewPos.y = fabsf(utility::GetRandomValue<float>());
		NewPos.z = sinf(fDirection + fRange) * CField_Manager::FIELD_RADIUS;

		// �u���b�N���m�̕������o
		if (DetectNearBlock(NewPos))
		{
			return;
		}

	} while (!m_pFan->DetectInFanRange(NewPos));

	// ����������
	NewRot.y = atan2f(-NewPos.x, -NewPos.z);

	// �u���b�N�𐶐�
	CBlock::Create(NewPos, NewRot);
}

//============================================================================
// �אڂ������u���b�N�����o
//============================================================================
bool CField_Builder::DetectNearBlock(D3DXVECTOR3 Pos)
{
	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			// ���݂��̋��������߂�
			const Vec3& Vec = pBlock->GetPos() - Pos;

			// �u���b�N�̃T�C�Y���R�s�[
			const Vec3& Size = pBlock->GetSize();

			// �u���b�N�̃T�C�Y���炢�ɋ߂Â��Ă�����
			if ((Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= (Size.x * Size.x + Size.y * Size.y + Size.z * Size.z) * 10.0f)
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
// �j�󔻒�
//============================================================================
void CField_Builder::DestroyCheck()
{
	// �ʏ탌�C���[�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// �u���b�N�^�C�v�̃I�u�W�F�N�g�Ȃ�
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = utility::DownCast<CBlock, CObject>(pObj);

			// ��`�͈͓̔��Ƀu���b�N���������
			if (!m_pFan->DetectInFanRange(pBlock->GetPos()))
			{
				// ���̃u���b�N�͔j��
				pBlock->SetRelease();

				// �u���b�N�̔j��ʂ��C���N�������g
				++m_nCntDestroyBlock;

				// �u���b�N�̔j��ʂ��ő�J�E���g�ɒB������
				if (m_nCntDestroyBlock >= CField_Manager::MAX_DESTROY_BLOCK)
				{
#if SAFE
					// ������U�����[�V�����ɕύX
					m_pStatue->SetNowMotion(0);
#endif // SAFE

					// �S�u���b�N��j��
					DestroyAllBlock();

					// �����������I��
					return;
				}
			}
		}

		// ���̃I�u�W�F�N�g���擾
		pObj = pObj->GetNext();
	}
}

//============================================================================
// �S�u���b�N�̍폜
//============================================================================
void CField_Builder::DestroyAllBlock()
{
	// �ʏ탌�C���[�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// �u���b�N�^�C�v�̃I�u�W�F�N�g�Ȃ�
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// ���̂܂ܔj���\��
			pObj->SetRelease();
		}

		// ���̃I�u�W�F�N�g��
		pObj = pObj->GetNext();
	}
}

//============================================================================
// �f�o�b�O�\��
//============================================================================
void CField_Builder::PrintDebug()
{
	// �O�ϊm�F�p
	Vec3 OldVec = m_pSyncPlayer->GetPos() - VEC3_INIT, NewVec = m_pSyncPlayer->GetPosTarget() - VEC3_INIT;
	D3DXVec3Normalize(&OldVec, &OldVec);
	D3DXVec3Normalize(&NewVec, &NewVec);
	float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

	// �m�����m�F�p
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Field Builder Data")) {
		ImGui::Text("DestroyBlock:%d", m_nCntDestroyBlock);
		ImGui::Text("CountJump:%d", m_ActionData.nCntJump);
		ImGui::Text("CountDash:%d", m_ActionData.nCntDash);
		ImGui::Text("FieldType:%s", typeid(m_upFieldType).name());
		ImGui::Text("Cross:%f", fCross);
		ImGui::Text("Norm:%f", Norm.x * Norm.x + Norm.z * Norm.z);
		ImGui::End();
	}
}