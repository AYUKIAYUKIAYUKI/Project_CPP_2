//============================================================================
//
// �v���C���[�A�w�b�_�t�@�C�� [player.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// ��d�C���N���[�h�h�~

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
// �v���C���[�N���X
//****************************************************
class CPlayer : public CCharacter
{
public:

	/// <summary> �v���C���[�̍ő�̗� </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> �v���C���[�̃f�t�H���g�ړ����x </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.0035f;

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float CORRECT_COEF = 0.5f;

	// <special function>
	CPlayer();	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��
	void	SetDamage(int nDamage, float fImpact);	// �_���[�W���󂯂�

	// <getter>
	float GetRadius() const;								// ���a���擾
	float GetHeight() const;								// �������擾
	const CBounding_Cylinder* const GetBndCylinder() const;	// �~���o�E���f�B���O���擾
	const CPlayer_State* const GetNowState() const;			// ���݂̃X�e�[�g���擾

	// <static function>
	static CPlayer* Create();	// ����

private:

	// <function>
	void AdjustHeight() override;	// ������␳
	void HitCheck();				// �����蔻��

	// <data>
	CBounding_Cylinder*	m_pBndCylinder;	// �~���o�E���f�B���O
	CPlayer_State*		m_pState;		// �X�e�[�g
};

#endif // _PLAYER_H_