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
#include "character_state.h"
#include "player.h"

//****************************************************
// �v���C���[�X�e�[�g�N���X
//****************************************************
class CPlayer_State : public CCharacter_State
{
public:

	CPlayer_State();					// �f�t�H���g�R���X�g���N�^
	CPlayer_State(CPlayer* pPlayer);	// �v���C���[�擾�R���X�g���N�^
	virtual ~CPlayer_State() override;	// �f�X�g���N�^

	virtual void Update() override = 0;	// �X�V����

private:

	CPlayer* RetrievePlayer();	// �v���C���[�|�C���^���擾
};

#endif	// _PLAYER_STATE_