//============================================================================
//
// �v���C���[�_���[�W�X�e�[�g�A�w�b�_�t�@�C�� [player_state_damage.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_DAMAGE_
#define _PLAYER_STATE_DAMAGE_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state.h"

//****************************************************
// �v���C���[	�_���[�W�X�e�[�g�N���X
//****************************************************
class CPlayer_State_Damage : public CPlayer_State
{
public:

	// <special function>
	CPlayer_State_Damage();				// �R���X�g���N�^
	~CPlayer_State_Damage() override;	// �f�X�g���N�^
	
	// <funciton>
	void Update() override;		// �X�V����
	void To_Default() override;	// ��Ԃ�
};

#endif	// _PLAYER_STATE_DAMAGE_