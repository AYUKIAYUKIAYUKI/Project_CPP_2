//============================================================================
//
// �t�B�[���h�^�C�v�A�w�b�_�t�@�C�� [field_type.h]
// Author : ���c����
//
//============================================================================

#ifndef _FIELD_TYPE_H_
#define _FIELD_TYPE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �t�B�[���h�^�C�v�N���X
//****************************************************
class CField_Type
{
public:

	// <special function>
	CField_Type();			// �R���X�g���N�^
	virtual ~CField_Type();	// �f�X�g���N�^

	// <function>
	virtual void GenerateBlock(float) = 0;	// �u���b�N����
};

#endif // _FIELD_TYPE_H_