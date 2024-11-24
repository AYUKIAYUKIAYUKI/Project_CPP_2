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

	// <special function>
	CPlayer_State();					// �R���X�g���N�^
	CPlayer_State(CPlayer* pPlayer);	// �v���C���[�擾�R���X�g���N�^
	virtual ~CPlayer_State() override;	// �f�X�g���N�^

	// <function>
	virtual void Update() override = 0;	// �X�V����
	virtual void To_Default();			// �ʏ��Ԃ֕ύX
	virtual void To_Dash();				// �_�b�V����Ԃ֕ύX
	virtual void To_Jump();				// �W�����v��Ԃ֕ύX
	virtual void To_Slash();			// �a����Ԃ֕ύX
	virtual void To_Damage();			// �_���[�W��Ԃ֕ύX

private:

	// <function>
	void PrintError();	// �G���[�\��

	// <Getter>
	CPlayer* RetrievePlayer();	// �v���C���[�|�C���^���擾
};

#endif	// _PLAYER_STATE_