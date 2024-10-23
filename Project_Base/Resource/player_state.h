//============================================================================
//
// �v���C���[�X�e�[�g�A�w�b�_�t�@�C�� [player_state.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_
#define _PLAYER_STATE_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "state.h"
#include "player.h"

//****************************************************
// �v���C���[�X�e�[�g�N���X
//****************************************************
class CPlayer_State : public CState
{
public:

	CPlayer_State();							// �f�t�H���g�R���X�g���N�^
	CPlayer_State(player::CPlayer* pPlayer);	// �v���C���[�擾�R���X�g���N�^
	~CPlayer_State() override;					// �f�X�g���N�^

	virtual void Update() override = 0;	// �X�V����

	CPlayer_State* GetNextState();				// ���̃X�e�[�g���擾
	void SetNextState(CPlayer_State* pState);	// ���̃X�e�[�g��ݒ�

protected:

	player::CPlayer*	m_pPlayer;		// �v���C���[
	CPlayer_State*		m_pNextState;	// ���̃X�e�[�g
};

#endif	// _PLAYER_STATE_