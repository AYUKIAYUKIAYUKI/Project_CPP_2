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
#include "block.h"

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

	// �Ō�̐������f���Ɠ���Ȃ�
	while (Type == m_LastModel)
	{
		// �Ē��I
		Type = CX_Manager::TYPE::BLONORMAL + (rand() % 3);
	}

	// �����Ɏg�p������W�E������p��
	Vec3 NewPos = VEC3_INIT, NewRot = VEC3_INIT;

	// �����p�̍��W������
	NewPos.x = cosf(fEdgeDirection) * CField_Manager::FIELD_RADIUS;
	NewPos.y = fabsf(utility::GetRandomValue<float>());
	NewPos.z = sinf(fEdgeDirection) * CField_Manager::FIELD_RADIUS;

	// �z�u�̈��S�������o�A�����ɔ����Ă���Ȃ琶�����Ȃ�
	if (!DetectLayoutSafety(Type, NewPos))
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
// �z�u�̈��S�������o
//============================================================================
bool CField_Type_Normal::DetectLayoutSafety(CX_Manager::TYPE Type, D3DXVECTOR3 Pos)
{
	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		/* �u���b�N�^�C�v�̃I�u�W�F�N�g�̂Ƃ� */
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�N���X�Ƀ_�E���L���X�g
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
				return false;
			}
		}

		// ���̃I�u�W�F�N�g��
		pObj = pObj->GetNext();
	}

	return true;
}