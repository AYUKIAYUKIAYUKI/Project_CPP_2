//============================================================================
//
// �H�A�w�b�_�t�@�C�� [wings.h]
// Author : ���c����
//
//============================================================================

#ifndef _WINGS_H_
#define _WINGS_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "item.h"

//****************************************************
// �H�N���X
//****************************************************
class CWings : public CItem
{
public:

	// <special function>
	CWings();			// �R���X�g���N�^
	~CWings() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <static function>
	static CWings* Create();	// ����

private:

	// <function>
	void GainEffect() override;	// �擾���̌���
};

#endif // _WINGS_H_