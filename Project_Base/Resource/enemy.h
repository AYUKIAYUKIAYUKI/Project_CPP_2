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
// �G�l�~�[�N���X
//****************************************************
class CEnemy : public CCharacter
{
public:

	/// <summary> �G�l�~�[�ő�̗� </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> �G�l�~�[�̃f�t�H���g�ړ����x </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.003f;

	CEnemy();	// �f�t�H���g�R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	static CEnemy* Create();	// ����

private:

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	void AttackOnPlayer();	// �v���C���[�֍U��
};

#endif // _PLAYER_H_