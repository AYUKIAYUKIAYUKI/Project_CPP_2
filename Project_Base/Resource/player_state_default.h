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

	CPlayer_State_Default();			// �f�t�H���g�R���X�g���N�^
	~CPlayer_State_Default() override;	// �f�X�g���N�^

	void Update() override;	// �X�V����

private:

	void Control();						// ����
	void SetRotTargetToMoveDirection();	// �ڕW�������ړ������ɐݒ�
	void SetPosTargetByDirection();		// ���p����ڕW���W��ݒ�
	void To_Dash(bool bDirection);		// �_�b�V��������
};

#endif	// _PLAYER_STATE_DEFAULT_