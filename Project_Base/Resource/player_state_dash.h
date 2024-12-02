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

	/// <summary> �_�b�V���L���X�g���� </summary>
	static constexpr int MAX_DASH_CAST = 30;

	// <special function>
	CPlayer_State_Dash();			// �R���X�g���N�^
	~CPlayer_State_Dash() override;	// �f�X�g���N�^

	// <function>
	void Update() override;		// �X�V����
	void To_Default() override;	// �ʏ��Ԃ�
	void To_Damage() override;	// �_���[�W��Ԃ�

private:

	/// <summary> �_�b�V���p������ </summary>
	static constexpr int MAX_DASH_DURATION = 10;

	// <function>
	void SetPosTarget_Unnamed();	// �ڕW���W���_�b�V�������ɑ���

	// <data>
	bool	m_bDirection;		// ����
	int		m_nDashDuration;	// �_�b�V���p������
};

#endif	// _PLAYER_STATE_DASH_