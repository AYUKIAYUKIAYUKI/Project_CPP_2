//============================================================================
//
// �����X�^�[�A�w�b�_�t�@�C�� [monster.h]
// Author : ���c����
//
//============================================================================

#ifndef _MONSTER_H_
#define _MONSTER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "enemy.h"

//****************************************************
// �O���錾
//****************************************************
class CBounding_Cylinder;

//****************************************************
// �����X�^�[�N���X
//****************************************************
class CMonster : public CEnemy
{
public:

	//****************************************************
	// �s���^�C�v���`
	//****************************************************
	enum class ACTION : WORD
	{
		HOLD = 0,	// �����~�܂�
		COMING,		// �����Ă���
		GOBACK,		// ����������
		DEADEND,	// ���S
		MAX
	};

	// <special function>
	CMonster();				// �R���X�g���N�^
	~CMonster() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;					// �����ݒ�
	void	Uninit() override;					// �I���ݒ�
	void	Update() override;					// �X�V����
	void	Draw() override;					// �`�揈��
	void	SetDamage(int nDamage) override;	// �_���[�W���󂯂�

	// <static funcion>
	static CMonster* Create();	// ����

private:

	// <function>
	void FakeGravity();		// �^���d��
	void BranchAction();	// �s������
	void Hold();			// �����~�܂�
	void Coming();			// �����Ă���
	bool StopBlockSide();	// �u���b�N�̋߂��Ŏ~�܂�
	void GoBack();			// ����������
	void DeadEnd();			// ���S

	// <data>
	ACTION m_ActionType;		// �s���^�C�v
	int	   m_nCntActionCast;	// �s���L���X�g�J�E���g
	float  m_fFakeVelY;			// �^�������x

	// <static data>
	static const JSON m_InitParam;	// ��b�p�����[�^
};

#endif // _MONSTER_H_