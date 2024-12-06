//============================================================================
//
// ���C�t�A�w�b�_�t�@�C�� [life.h]
// Author : ���c����
//
//============================================================================

#ifndef _LIFE_H_
#define _LIFE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "item.h"

//****************************************************
// ���C�t�N���X
//****************************************************
class CLife : public CItem
{
public:

	// <special function>
	CLife();			// �R���X�g���N�^
	~CLife() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <static function>
	static CLife* Create();	// ����

private:

	// <function>
	void GainEffect() override;	// �擾���̌���
};

#endif // _LIFE_H_