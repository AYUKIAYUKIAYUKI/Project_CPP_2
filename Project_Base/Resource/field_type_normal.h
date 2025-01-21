//============================================================================
//
// �t�B�[���h�^�C�v-�m�[�}���A�w�b�_�t�@�C�� [field_type_normal.h]
// Author : ���c����
//
//============================================================================

#ifndef _FIELD_TYPE_NORMAL_H_
#define _FIELD_TYPE_NORMAL_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "field_type.h"
#include "X_manager.h"

//****************************************************
// �t�B�[���h�^�C�v-�m�[�}���N���X
//****************************************************
class CField_Type_Normal : public CField_Type
{
public:

	// <special function>
	CField_Type_Normal();			// �R���X�g���N�^
	~CField_Type_Normal() override;	// �f�X�g���N�^

	// <function>
	void GenerateBlock(float) override;	// �X�V����

	// <static function>
	static CField_Type_Normal* Create();	// ����

private:

	// <fuction>
	bool DetectOverlapItem(D3DXVECTOR3);					// �A�C�e���Ƃ̏d�������o
	bool DetectOverlapBlock(CX_Manager::TYPE, D3DXVECTOR3);	// �u���b�N�̏d�������o

	// <data>
	CX_Manager::TYPE m_LastModel;	// �Ō�̃��f��
};

#endif // _FIELD_TYPE_NORMAL_H_