//============================================================================
//
// �v���C���[�ʏ�X�e�[�g�A�w�b�_�t�@�C�� [player_state_default.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_DEFAULT_
#define _PLAYER_STATE_DEFAULT_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state.h"

//****************************************************
// �v���C���[�ʏ�X�e�[�g�N���X
//****************************************************
class CPlayer_State_Default : public CPlayer_State
{
public:

	// <special function>
	CPlayer_State_Default();			// �R���X�g���N�^
	~CPlayer_State_Default() override;	// �f�X�g���N�^

	// <function>
	void Update() override;		// �X�V����
	void To_Dash() override;	// �_�b�V����Ԃ�
	void To_Jump() override;	// �W�����v��Ԃ�
	void To_Slash() override;	// �a����Ԃ�
	void To_Damage() override;	// �_���[�W��Ԃ�

private:

	// <function>
	void Control();	// ����
};

#endif	// _PLAYER_STATE_DEFAULT_