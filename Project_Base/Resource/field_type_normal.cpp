//============================================================================
//
// �t�B�[���h�^�C�v-�m�[�}�� [field_type_normal.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "field_type_normal.h"

#include "field_manager.h"
#include "field_builder.h"
#include "player.h"
#include "item.h"
#include "block.h"
#include "bright.h"
#include "enemy.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

/* �f�o�b�O�p - �u���b�N�̖��W�x�� */
extern float fBlockDensity;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CField_Type_Normal::CField_Type_Normal() :
	m_LastModel{ CX_Manager::TYPE::BLONORMAL }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CField_Type_Normal::~CField_Type_Normal()
{

}

//============================================================================
// �X�V����
//============================================================================
void CField_Type_Normal::GenerateBlock(float fEdgeDirection)
{
	// �u���b�N�̃��f���^�C�v�����O�ɑI�o
	CX_Manager::TYPE Type = CX_Manager::TYPE::BLONORMAL + (rand() % 3);

	// �����Ɏg�p������W�E������p��
	Vec3 NewPos = VEC3_INIT, NewRot = VEC3_INIT;

	{ // �����p�̍��W������

		// �@���E�̏ꏊ�͋��E���̈ʒu��
		NewPos.x = cosf(fEdgeDirection) * CField_Manager::FIELD_RADIUS;
		NewPos.z = sinf(fEdgeDirection) * CField_Manager::FIELD_RADIUS;

		// �A�����̓L���̗ǂ����l�ɂȂ�悤����
		NewPos.y = fabsf(utility::GetRandomValue<float>());
		NewPos.y = utility::RoundToAnyMultiple<float>(NewPos.y, 20, 9);

		{ // �B�u���b�N�̃^�C�v�ɉ�������ɏ�������

			// ��L�͈͂��R�s�[
			float fRangeHeight = CField_Type::BLOCK_RANGE.z;

			// �c���̃u���b�N�ȊO�͔͈͔���
			if (Type != CX_Manager::TYPE::BLOTALL)
			{
				fRangeHeight *= 0.5f;
			}

			// �u���b�N���n�ʂɖ��܂��Ă��܂�Ȃ��悤������ݒ�
			if (NewPos.y < fRangeHeight)
			{
				NewPos.y = fRangeHeight;
			}
		}
	}

	// �A�C�e���Ƃ̏d�������o
	if (DetectOverlapItem(NewPos))
	{
		return;
	}

	// �u���b�N�Ƃ̏d�������o
	if (DetectOverlapBlock(Type, NewPos))
	{
		return;
	}

	// ����������
	NewRot.y = atan2f(-NewPos.x, -NewPos.z);

	// �u���b�N�𐶐����A�u���b�N���f����ύX����
	CBlock* pBlock = CBlock::Create(NewPos, NewRot);	// �u���b�N�̃C���X�^���X����
	pBlock->BindModel(Type);							// ���f�������蓖��
	pBlock->SetSize(pBlock->GetModel()->Size);			// �o�E���f�B���O�T�C�Y�𑵂���

	// �Ō�ɐ����������f���^�C�v��ێ�
	m_LastModel = Type;

	// �u���b�N�̔j��ʂ���萔�ɒB���Ă����
	if (CField_Manager::GetInstance()->GetFieldBuilder()->GetCntDestroyBlock() % 10 == 0)
	{
		// ���ԂɓG�𐶐�
		DetectGapForSetEnemy();
	}
}

//============================================================================
// ����
//============================================================================
CField_Type_Normal* CField_Type_Normal::Create()
{
	CField_Type_Normal* pNewInstance = DBG_NEW CField_Type_Normal();

	// �������s
	if (!pNewInstance)
	{
		assert(false);
	}

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �A�C�e���Ƃ̏d�������o
//============================================================================
bool CField_Type_Normal::DetectOverlapItem(D3DXVECTOR3 SelfPos)
{
	// �A�C�e���^�C�v�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

	if (pObj)
	{
		// �I�u�W�F�N�g���A�C�e���N���X�Ƀ_�E���L���X�g
		CItem* pItem = nullptr;
		pItem = utility::DownCast(pItem, pObj);

		// �A�C�e���Ƃ̋����̍����쐬
		const Vec3& Distance = pItem->GetPos() - SelfPos;

		// �A�C�e���Ƀu���b�N���ڋ߂��Ă��܂��Ă�����
		if (Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z <= CField_Type::GetAreaNorm() * 1.5f)
		{
			// �d�����Ă���A���̃u���b�N�𐶐����Ȃ�
			return true;
		}
	}

	// �����ʉ�
	return false;
}

//============================================================================
// �u���b�N�Ƃ̏d�������o
//============================================================================
bool CField_Type_Normal::DetectOverlapBlock(CX_Manager::TYPE SelfType, D3DXVECTOR3 SelfPos)
{
	// �A�C�e���̉��ɑ��ꂪ���݂��Ă��邩����
	bool bBlockUnderItem = false;

	// �A�C�e������`�͈͓��ɑ��݂��Ă��邩����
	bool bInFanRange = false;

	// �A�C�e���^�C�v�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

	// �A�C�e���N���X�̃|�C���^��p��
	CItem* pItem = nullptr;

	if (pObj)
	{
		// �I�u�W�F�N�g���A�C�e���N���X�Ƀ_�E���L���X�g
		pItem = utility::DownCast(pItem, pObj);
	}

	// �@�A�C�e���̍��W���R�s�[
	Vec3 ItemPos = pItem->GetPos();

	// �A�A�C�e������`�͈͓��ɑ��݂��邩����
	if (CField_Manager::GetInstance()->GetFieldBuilder()->DetectInFanRange(ItemPos))
	{
		// �B�͈͓��ɃA�C�e�������邱�Ƃ��L�^
		bInFanRange = true;

		// �C�L���̗ǂ��ꏊ�ɃA�C�e���̏ꏊ���ŌŒ肷��
		ItemPos.y += -40.0f;
		ItemPos.y = utility::RoundToAnyMultiple<float>(ItemPos.y, 20, 9);
	}

	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// �u���b�N�^�C�v�̃I�u�W�F�N�g�̂Ƃ�
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�N���X�Ƀ_�E���L���X�g
			CBlock* pOther = nullptr;
			pOther = utility::DownCast(pOther, pObj);

			// �K�v�ȏ���p��
			const CX_Manager::TYPE&
				OtherType = pOther->GetModelType();	// �Ώۂ̃��f���^�C�v���擾
			const Vec3&
				Distance = pOther->GetPos() - SelfPos,	// ���݂��̋����̍�
				OtherSize = pOther->GetSize();			// �Ώۂ̃T�C�Y
			float
				DistanceNorm = (Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);	// �����̍��̑傫��

			// ���̃u���b�N�Ƃ̋�����������x�߂���Ώ����𖞂����Ȃ�
			if (DistanceNorm <= CField_Type::GetAreaNorm() * fBlockDensity)
			{
				// �d�����Ă���A���̃u���b�N�𐶐����Ȃ�
				return true;
			}

			// �A�C�e���̑���ƂȂ�u���b�N�����݂��Ă���Ȃ�
			if (ItemPos == pOther->GetPos())
			{
				bBlockUnderItem = true;
			}
		}

		// ���̃I�u�W�F�N�g��
		pObj = pObj->GetNext();
	}

	// �D�S�Ẵu���b�N�̒��ɑ���ƂȂ���̂����݂����A���A�C�e������`�͈͓��Ɏ��܂��Ă���Ƃ�
	if (!bBlockUnderItem && bInFanRange)
	{
		// �V������p��
		Vec3 NewRot = VEC3_INIT;

		// ����������
		NewRot.y = atan2f(-ItemPos.x, -ItemPos.z);

		// �u���b�N�𐶐����A�u���b�N���f����ύX����
		CBlock* pBlock = CBlock::Create(ItemPos, NewRot);	// �u���b�N�̃C���X�^���X����
		pBlock->BindModel(CX_Manager::TYPE::BLOSIDE);		// ���f�������蓖��
		pBlock->SetSize(pBlock->GetModel()->Size);			// �o�E���f�B���O�T�C�Y�𑵂���
	}

	// �����ʉ�
	return false;
}

//============================================================================
// �G�l�~�[�������o�������Ȍ��Ԃ����o
//============================================================================
void CField_Type_Normal::DetectGapForSetEnemy()
{
	/*
		�G�l�~�[�^�C�v���������ă����X�^�[�N���X�������Ƃ��̂݉����u���b�N�̐^���Ɉ�̐���
	*/

	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// ���̃I�u�W�F�N�g�̃^�C�v���R�s�[
		const CObject::TYPE Type = pObj->GetType();

		// �M���^�C�v�̃I�u�W�F�N�g�����݂��Ă�����
		if (Type == CObject::TYPE::BRIGHT)
		{
			// �G�𐶐������I��
			return;
		}

		// �G�l�~�[�^�C�v�̃I�u�W�F�N�g�����݂��Ă�����
		if (Type == CObject::TYPE::ENEMY)
		{
			// �N���X���m�F���A�����X�^�[�������ꍇ
			if (typeid(*pObj) == typeid(CMonster))
			{
				// �G�𐶐������I��
				return;
			}
		}

		// ���̃I�u�W�F�N�g��
		pObj = pObj->GetNext();
	}

	// �v���C���[�^�C�v�̃I�u�W�F�N�g���擾
	pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// �擾���s�ŏI��
	if (!pObj)
	{
		return;
	}

	// �v���C���[�N���X�Ƀ_�E���L���X�g
	const CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// �K���̍��W��ێ����Ă���
	Vec3 HoldBestPos = VEC3_INIT;

	// �����̍��̑傫����ێ����Ă���
	float fDistanceNorm = 0.0f;

	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// �u���b�N�^�C�v�̃I�u�W�F�N�g�̂Ƃ�
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�N���X�Ƀ_�E���L���X�g
			CBlock* pAnyBlock = nullptr;
			pAnyBlock = utility::DownCast(pAnyBlock, pObj);

			// ���̃u���b�N�̃��f���^�C�v���擾
			const CX_Manager::TYPE& Type = pAnyBlock->GetModelType();

			// �����u���b�N�������ꍇ
			if (Type == CX_Manager::TYPE::BLOSIDE)
			{
				// �u���b�N�̍������R�s�[
				float fHeight = pAnyBlock->GetPos().y;

				// ���̃u���b�N�̍������K���Ă��邩����
				if (fHeight >= 50.0f &&
					fHeight <= 70.0f)
				{
					// �����̍�������o��
					Vec3 Distance = pAnyBlock->GetPos() - pPlayer->GetPos();

					// �傫��������o��
					float fNorm = Distance.x * Distance.x + Distance.z * Distance.z;

					// ��藣�ꂽ�ꏊ�ɂ���ꍇ
					if (fNorm > fDistanceNorm)
					{
						// ���̑傫����ێ�
						fDistanceNorm = fNorm;

						// �K���̍��W���X�V
						HoldBestPos = { pAnyBlock->GetPos() };
						HoldBestPos.y += -20.0f;
					}
				}
			}
		}

		// ���̃I�u�W�F�N�g��
		pObj = pObj->GetNext();
	}

	// �����X�^�[�𐶐�
	CBright::Generate(HoldBestPos, CBright::CREATETYPE::MONSTER);
}