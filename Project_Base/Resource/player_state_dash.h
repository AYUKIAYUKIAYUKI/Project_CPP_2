//============================================================================
//
// �v���C���[�_�b�V���X�e�[�g�A�w�b�_�t�@�C�� [player_state_dash.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_DASH_
#define _PLAYER_STATE_DASH_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state.h"

//****************************************************
// �v���C���[�_�b�V���X�e�[�g�N���X
//****************************************************
class CPlayer_State_Dash : public CPlayer_State
{
public:

	CPlayer_State_Dash();			// �f�t�H���g�R���X�g���N�^
	~CPlayer_State_Dash() override;	// �f�X�g���N�^

	void Update() override;	// �X�V����

private:

	/// <summary> �_�b�V���p������ </summary>
	static constexpr int MAX_DASH_DURATION = 45;

	int m_nDashDuration;	// �_�b�V���p������

	void To_Default();	// ���ɖ߂�
};

#endif	// _PLAYER_STATE_DASH_