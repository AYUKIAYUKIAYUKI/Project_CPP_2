//============================================================================
//
// �v���C���[�W�����v�X�e�[�g�A�w�b�_�t�@�C�� [player_state_jump.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_JUMP_
#define _PLAYER_STATE_JUMP_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state.h"

//****************************************************
// �v���C���[�W�����v�X�e�[�g�N���X
//****************************************************
class CPlayer_State_Jump : public CPlayer_State
{
public:

	/// <summary> �W�����v��񓖂���̑��������x </summary>
	static constexpr float AMOUNT_JUMPACCEL = 8.0f;

	// <special function>
	CPlayer_State_Jump();			// �f�t�H���g�R���X�g���N�^
	~CPlayer_State_Jump() override;	// �f�X�g���N�^

	// <function>
	void Update() override;	// �X�V����

private:

	/// <summary> �ő剄���P�\ </summary>
	static constexpr int MAX_JUMPREMAIN_DURATION = 20;

	// <function>
	void Control();			// ����
	void AdjustGravity();	// �d�͂̕␳
	void To_Default();		// �X�e�[�g - �ʏ��Ԃ�

	// <data>
	int		m_nJumpRemainDuration;	// ���͉�������
	bool	m_bEndRemain;			// ��������
};

#endif	// _PLAYER_STATE_JUMP_