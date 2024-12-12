//============================================================================
//
// �t���C���[�A�w�b�_�t�@�C�� [flyer.h]
// Author : ���c����
//
//============================================================================

#ifndef _FLYER_H_
#define _FLYER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "enemy.h"

//****************************************************
// �O���錾
//****************************************************
class CBounding_Cylinder;

//****************************************************
// �t���C���[�N���X
//****************************************************
class CFlyer : public CEnemy
{
public:

	//****************************************************
	// �s���^�C�v���`
	//****************************************************
	enum class ACTION : WORD
	{
		UPDOWN = 0,	// ���~
		SLASH,		// �a��
		DEADEND,	// ���S
		MAX
	};

	// <special function>
	CFlyer();			// �R���X�g���N�^
	~CFlyer() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;					// �����ݒ�
	void	Uninit() override;					// �I���ݒ�
	void	Update() override;					// �X�V����
	void	Draw() override;					// �`�揈��
	void	SetDamage(int nDamage) override;	// �_���[�W���󂯂�

	// <static funcion>
	static CFlyer* Create();	// ����

private:

	// <function>
	void BranchAction();	// �s������
	void UpDown();			// ���~
	void Slash();			// �a��
	void DeadEnd();			// ���S

	// <data>
	ACTION m_ActionType;	// �s���^�C�v

	// <static data>
	static const JSON m_InitParam;	// ��b�p�����[�^
};

#endif // _FLYER_H_