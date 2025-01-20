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

	/// <summary> �u���b�N��L�͈� </summary>
	static D3DXVECTOR3 BLOCK_RANGE;

	// <special function>
	CField_Type();			// �R���X�g���N�^
	virtual ~CField_Type();	// �f�X�g���N�^

	// <function>
	virtual void GenerateBlock(float) = 0;	// �u���b�N����

	// <static function>
	static float GetAreaNorm();	// ��L�͈͂̑傫�����擾

private:

	// <static function>
	static D3DXVECTOR3 SetBlockRange();	// �u���b�N��L�͈͂�ݒ�
};

#endif // _FIELD_TYPE_H_