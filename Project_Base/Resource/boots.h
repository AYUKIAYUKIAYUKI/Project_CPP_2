//============================================================================
//
// �u�[�c�A�w�b�_�t�@�C�� [boots.h]
// Author : ���c����
//
//============================================================================

#ifndef _BOOTS_H_
#define _BOOTS_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "item.h"

//****************************************************
// �u�[�c�N���X
//****************************************************
class CBoots : public CItem
{
public:

	// <special function>
	CBoots();			// �R���X�g���N�^
	~CBoots() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <static function>
	static CBoots* Create();	// ����

private:

	// <function>
	void GainEffect() override;	// �擾���̌���
};

#endif // _BOOTS_H_