//============================================================================
//
// �p���[�X�g�[���A�w�b�_�t�@�C�� [powerstone.h]
// Author : ���c����
//
//============================================================================

#ifndef _POWERSTONE_H_
#define _POWERSTONE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "item.h"

//****************************************************
// �p���[�X�g�[���N���X
//****************************************************
class CPowerStone : public CItem
{
public:

	// <special function>
	CPowerStone();			// �R���X�g���N�^
	~CPowerStone() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <static function>
	static CPowerStone* Create();	// ����

private:

	// <function>
	void GainEffect() override;	// �擾���̌���
};

#endif // _POWERSTONE_H_