//============================================================================
//
// �S�[�X�g�A�w�b�_�t�@�C�� [ghost.h]
// Author : ���c����
//
//============================================================================

#ifndef _GHOST_H_
#define _GHOST_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "enemy.h"

//****************************************************
// �O���錾
//****************************************************
class CBounding_Cylinder;

//****************************************************
// �S�[�X�g�N���X
//****************************************************
class CGhost : public CEnemy
{
public:

	//****************************************************
	// �s���^�C�v���`
	//****************************************************
	enum class ACTION : WORD
	{
		HOLD = 0,	// �Y��
		FLY,		// ���ł���
		DEADEND,	// ���S
		MAX
	};

	// <special function>
	CGhost();			// �R���X�g���N�^
	~CGhost() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;					// �����ݒ�
	void	Uninit() override;					// �I���ݒ�
	void	Update() override;					// �X�V����
	void	Draw() override;					// �`�揈��
	void	SetDamage(int nDamage) override;	// �_���[�W���󂯂�

	// <static funcion>
	static CGhost* Create();	// ����

private:

	// <function>
	void BranchAction();	// �s������
	void Hold();			// �Y��
	void Fly();				// ���ł���
	void DeadEnd();			// ���S

	// <data>
	ACTION m_ActionType;		// �s���^�C�v

	// <static data>
	static const JSON m_InitParam;	// ��b�p�����[�^
};

#endif // _ENEMY_H_