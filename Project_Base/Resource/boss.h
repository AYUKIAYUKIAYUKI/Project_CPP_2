//============================================================================
//
// �{�X�A�w�b�_�t�@�C�� [boss.h]
// Author : ���c����
//
//============================================================================

#ifndef _BOSS_H_
#define _BOSS_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "character.h"

//****************************************************
// �O���錾
//****************************************************
class CBounding_Cylinder;
class CPlayer_State;

//****************************************************
// �{�X�N���X
//****************************************************
class CBoss : public CCharacter
{
public:

	//****************************************************
	// �s���^�C�v���`
	//****************************************************
	enum class ACTION : WORD
	{
		HOLDCENTER = 0,	// �����őҋ@
		DIRECTATTACK,	// �̓�����
		WALLATTACK,		// �Ǎ��
		SUMMONENEMY,	// �G����
		DAMAGEBACK,		// �_���[�W��炢
		DEADEND,		// ���S
		MAX
	};
	
	/// <summary> �{�X�̍ő�̗� </summary>
	static constexpr int MAX_LIFE = 7;

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float CORRECT_COEF = 0.1f;

	// <special function>
	CBoss();	// �R���X�g���N�^
	~CBoss();	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;		// �����ݒ�
	void	Uninit() override;		// �I���ݒ�
	void	Update() override;		// �X�V����
	void	Draw() override;		// �`�揈��
	void	SetDamage(int nDamage);	// �_���[�W���󂯂�

	// <getter>
	float GetRadius() const;								// ���a���擾
	float GetHeight() const;								// �������擾
	const CBounding_Cylinder* const GetBndCylinder() const;	// �~���V�����_�[���擾

	// <static function>
	static CBoss* Create();	// ����

private:

	// <function>
	void SetNextAction();	// ���̍s��������
	void BranchAction();	// �s������
	void HoldCenter();		// ���S�őҋ@
	void DirectAttack();	// �̓�����
	void WallAttack();		// �Ǎ��
	void SummonEnemy();		// �G����
	void DamageBack();		// �_���[�W��炢
	void DeadEnd();			// ���S
	bool HitCheck();		// �Փˌ��o

	// <data>
	ACTION				m_ActionType;		// �s���^�C�v
	int					m_nCntActionCast;	// �s���L���X�g�J�E���g
	int					m_nDuration;		// �ėp����
	CBounding_Cylinder* m_pBndCylinder;		// �~���o�E���f�B���O
};

#endif // _PLAYER_H_