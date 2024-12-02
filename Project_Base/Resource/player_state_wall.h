//============================================================================
//
// �v���C���[�ǂ���X�e�[�g�A�w�b�_�t�@�C�� [player_state_wall.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_WALL_
#define _PLAYER_STATE_WALL_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state.h"

//****************************************************
// �v���C���[�ǂ���X�e�[�g�N���X
//****************************************************
class CPlayer_State_Wall : public CPlayer_State
{
public:

	// <special function>
	CPlayer_State_Wall();			// �R���X�g���N�^
	~CPlayer_State_Wall() override;	// �f�X�g���N�^

	// <function>
	void Update() override;		// �X�V����
	void To_Default() override;	// �ʏ��Ԃ�
	void To_Jump() override;	// �W�����v��Ԃ�
	void To_Damage() override;	// �_���[�W��Ԃ�

private:

	// <function>
	void AdjustGravity();	// �d�͂̕␳

	// <data>
	int	m_nWallDuration;	// �p������
};

#endif	// _PLAYER_STATE_SLASH_