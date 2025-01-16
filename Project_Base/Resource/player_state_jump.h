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

	// <special function>
	CPlayer_State_Jump();			// �R���X�g���N�^
	~CPlayer_State_Jump() override;	// �f�X�g���N�^

	// <function>
	void Update() override;		// �X�V����
	void To_Default() override;	// �ʏ��Ԃ�
	void To_Dash() override;	// �_�b�V����Ԃ�
	void To_Wall() override;	// �ǂ����Ԃ�
	void To_Slash() override;	// �a����Ԃ�
	void To_Damage() override;	// �_���[�W��Ԃ�

	// <static data>
	static float AMOUNT_JUMPACCEL;	// �W�����v��񓖂���̑��������x

private:

	// <function>
	void Control();			// ����
	void AdjustGravity();	// �d�͂̕␳

	// <data>
	int		m_nJumpRemainDuration;	// ���͉�������
	bool	m_bEndRemain;			// ��������

	// <static data>
	static JSON m_JumpParam;	// �W�����v�p�p�����[�^
};

#endif	// _PLAYER_STATE_JUMP_