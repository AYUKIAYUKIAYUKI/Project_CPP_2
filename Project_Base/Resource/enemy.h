//============================================================================
//
// �G�l�~�[�A�w�b�_�t�@�C�� [enemy.h]
// Author : ���c����
//
//============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "character.h"

//****************************************************
// �O���錾
//****************************************************
class CBounding_Cylinder;

//****************************************************
// �G�l�~�[�N���X
//****************************************************
class CEnemy : public CCharacter
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

	/// <summary> �G�l�~�[�ő�̗� </summary>
	static constexpr int MAX_LIFE = 1;

	/// <summary> �G�l�~�[�̃f�t�H���g�ړ����x </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.002f;

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float CORRECT_COEF = 0.1f;

	// <special function>
	CEnemy();	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;					// �����ݒ�
	void	Uninit() override;					// �I���ݒ�
	void	Update() override;					// �X�V����
	void	Draw() override;					// �`�揈��
	void	SetDamage(int nDamage) override;	// �_���[�W���󂯂�

	// <getter>
	float GetRadius() const;								// ���a���擾
	float GetHeight() const;								// �������擾
	const CBounding_Cylinder* const GetBndCylinder() const;	// �o�E���f�B���O�V�����_�[���擾

	// <static funcion>
	static CEnemy* Create();	// ����

private:

	// <function>
	void SetNextAction();			// ���̍s��������
	void BranchAction();			// �s������
	void Hold();					// �����~�܂�
	void Coming();					// �����Ă���
	bool StopBlockSide();			// �u���b�N�̋߂��Ŏ~�܂�
	void GoBack();					// ����������
	void DeadEnd();					// ���S
	void AdjustHeight() override;	// ������␳
	bool HitCheck();				// �Փˌ��o

	// <data>
	ACTION				m_ActionType;		// �s���^�C�v
	int					m_nCntActionCast;	// �s���L���X�g�J�E���g
	CBounding_Cylinder* m_pBndCylinder;		// �~���o�E���f�B���O
};

#endif // _PLAYER_H_