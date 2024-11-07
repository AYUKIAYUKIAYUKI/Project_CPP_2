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
	static constexpr float AMOUNT_JUMPACCEL = 5.0f;

	// <special function>
	CPlayer_State_Jump();			// �f�t�H���g�R���X�g���N�^
	~CPlayer_State_Jump() override;	// �f�X�g���N�^

	// <function>
	void Update() override;	// �X�V����

private:

	// <function>
	void Control();		// ����
	void To_Default();	// �X�e�[�g - �ʏ��Ԃ�
};

#endif	// _PLAYER_STATE_JUMP_