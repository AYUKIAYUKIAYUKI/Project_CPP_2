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

	/// <summary> �{�X�̍ő�̗� </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float CORRECT_COEF = 0.5f;

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

	// <data>
	CBounding_Cylinder* m_pBndCylinder;		// �~�������_�[
};

#endif // _PLAYER_H_