//============================================================================
//
// ���@�w�A�w�b�_�t�@�C�� [summoning.h]
// Author : ���c����
//
//============================================================================

#ifndef _SUMMONING_H_
#define _SUMMONING_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "motion_set.h"

//****************************************************
// ���@�w�N���X
//****************************************************
class CSummoning : public CMotion_Set
{
public:

	// <special function>
	CSummoning(LAYER Layer);		// �R���X�g���N�^
	virtual ~CSummoning() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <stati function>
	static CSummoning* Create();

private:

	// <function>
	bool Disappear();	// ����

	// <static data>
	static const JSON m_MotionData;	// ���[�V�����f�[�^
};

#endif // _SUMMONING_H_