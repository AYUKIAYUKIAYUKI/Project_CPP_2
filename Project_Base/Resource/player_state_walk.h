//============================================================================
//
// �v���C���[���s�X�e�[�g�A�w�b�_�t�@�C�� [player_state_walk.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_WALK_
#define _PLAYER_STATE_WALK_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state.h"

//****************************************************
// �v���C���[���s�X�e�[�g�N���X
//****************************************************
class CPlayer_State_Walk : public CPlayer_State
{
public:

	CPlayer_State_Walk();			// �f�t�H���g�R���X�g���N�^
	~CPlayer_State_Walk() override;	// �f�X�g���N�^

	void Update() override;	// �X�V����
};

#endif	// _PLAYER_STATE_WALK_