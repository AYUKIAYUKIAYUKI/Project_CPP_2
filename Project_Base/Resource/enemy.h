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

	/// <summary> �G�l�~�[�ő�̗� </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> �G�l�~�[�̃f�t�H���g�ړ����x </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.003f;

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	// <special function>
	CEnemy();	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <getter>
	float GetRadius() const override;	// ���a���擾
	float GetHeight() const override;	// �������擾

	// <static funcion>
	static CEnemy* Create();	// ����

private:

	// <function>
	void AttackOnPlayer();	// �v���C���[�֍U��

	// <data>
	CBounding_Cylinder* m_pBndCylinder;		// �o�E���f�B���O�V�����_�[
};

#endif // _PLAYER_H_