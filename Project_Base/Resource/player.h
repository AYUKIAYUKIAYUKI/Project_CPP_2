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
#include "character_state_manager.h"

//****************************************************
// �v���C���[�N���X
//****************************************************
class CPlayer : public CCharacter
{
public:

	/// <summary> �v���C���[�̍ő�̗� </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> �v���C���[�̃f�t�H���g�ړ����x </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.005f;

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	CPlayer();	// �f�t�H���g�R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	HRESULT Init() override;		// �����ݒ�
	void	Uninit() override;		// �I���ݒ�
	void	Update() override;		// �X�V����
	void	Draw() override;		// �`�揈��
	void	To_Damage(int nDamage);	// �_���[�W��Ԃ�

	static CPlayer* Create();	// ����

private:

	CCharacter_State_Manager* m_pStateManager;	// �X�e�[�g�}�l�[�W���[
};

#endif // _PLAYER_H_