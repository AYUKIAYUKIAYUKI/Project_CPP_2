//============================================================================
//
// �t�B�[���h�^�C�v-�_�b�V���A�w�b�_�t�@�C�� [field_type_dash.h]
// Author : ���c����
//
//============================================================================

#ifndef _FIELD_TYPE_DASH_H_
#define _FIELD_TYPE_DASH_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "field_type.h"

//****************************************************
// �t�B�[���h�^�C�v-�_�b�V���N���X
//****************************************************
class CField_Type_Dash : public CField_Type
{
public:

	// <special function>
	CField_Type_Dash();				// �R���X�g���N�^
	~CField_Type_Dash() override;	// �f�X�g���N�^

	// <function>
	void Update() override;	// �X�V����

	// <static function>
	static CField_Type_Dash* Create();	// ����
};

#endif // _FIELD_TYPE_DASH_H_